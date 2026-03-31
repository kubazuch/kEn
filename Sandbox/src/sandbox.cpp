#include <imgui/imgui.h>

#include <algorithm>
#include <cstdint>
#include <memory>

#include <mEn/features/type_ptr.hpp>
#include <mEn/functions/matrix_projection.hpp>
#include <mEn/functions/matrix_transform.hpp>
#include <mEn/functions/trigonometric.hpp>
#include <mEn/fwd.hpp>
#include <mEn/vec3.hpp>

#include <kEn.hpp>  //NOLINT
#include <kEn/core/application.hpp>
#include <kEn/core/key_codes.hpp>
#include <kEn/core/layer.hpp>
#include <kEn/core/log.hpp>
#include <kEn/core/timestep.hpp>
#include <kEn/core/window.hpp>
#include <kEn/event/application_events.hpp>
#include <kEn/event/event.hpp>
#include <kEn/event/key_events.hpp>
#include <kEn/event/mouse_events.hpp>
#include <kEn/renderer/device.hpp>
#include <kEn/renderer/framebuffer.hpp>
#include <kEn/renderer/material.hpp>
#include <kEn/renderer/render_context.hpp>
#include <kEn/renderer/renderer.hpp>
#include <kEn/renderer/shader.hpp>
#include <kEn/renderer/texture_format.hpp>
#include <kEn/scene/camera/camera.hpp>
#include <kEn/scene/core_components.hpp>
#include <kEn/scene/game_object.hpp>
#include <kEn/scene/light.hpp>
#include <kEn/scene/mesh/model.hpp>

namespace {

class DemoLayer : public kEn::Layer {
 public:
  DemoLayer() : Layer("Demo"), device_(kEn::device()) {}

  void on_attach() override {
    dispatcher_.subscribe(this, &DemoLayer::on_key_pressed);
    dispatcher_.subscribe(this, &DemoLayer::on_mouse_scroll);
    dispatcher_.subscribe(this, &DemoLayer::on_window_resize);

    const kEn::Window& win = kEn::Application::instance().main_window();

    vp_w_ = win.width();
    vp_h_ = win.height();

    // --- Camera ---
    camera_ = std::make_shared<kEn::PerspectiveCamera>(
        mEn::radians(fov_), static_cast<float>(vp_w_) / static_cast<float>(vp_h_), 0.01F, 200.F);
    camera_obj_.transform().set_local_pos({0.F, 1.5F, 5.F});
    camera_obj_.add_components({
        camera_,
        std::make_shared<kEn::FreeLookComponent>(0.1F),
        std::make_shared<kEn::FreeMoveComponent>(5.F, true),
    });

    // --- Spot light (torch) - child of camera, follows it ---
    spot_light_                     = std::make_shared<kEn::SpotLight>();
    spot_light_->color              = {1.F, 1.F, 0.9F};
    spot_light_->atten              = {.constant = 1.F, .linear = 0.09F, .quadratic = 0.032F};
    spot_light_->inner_cutoff_angle = 0.20F;
    spot_light_->outer_cutoff_angle = 0.35F;
    spot_light_obj_.add_component(spot_light_);
    camera_obj_.add_child(spot_light_obj_);

    // --- Model ---
    model_obj_.transform().set_local_pos({0.F, 0.F, 0.F});
    auto backpack = kEn::Model::load("backpack/backpack.obj", {}, true);
    model_obj_.add_component(std::make_shared<kEn::ModelComponent>(backpack));

    // --- Directional light that always aims at the model (LookAt) ---
    dir_light_        = std::make_shared<kEn::DirectionalLight>();
    dir_light_->color = {0.8F, 0.85F, 1.F};
    dir_light_obj_.transform().set_local_pos({5.F, 8.F, 3.F});
    dir_light_obj_.add_components({
        dir_light_,
        std::make_shared<kEn::LookAtComponent>(model_obj_),
    });

    // --- Point light ---
    point_light_        = std::make_shared<kEn::PointLight>();
    point_light_->color = {1.F, 0.85F, 0.5F};
    point_light_->atten = {.constant = 1.F, .linear = 0.09F, .quadratic = 0.032F};
    point_light_obj_.transform().set_local_pos({-2.F, 2.F, 1.F});
    point_light_obj_.add_component(point_light_);

    // --- Floor (scaled cube with programmatic white texture) ---
    floor_obj_.transform().set_local_pos({0.F, -1.8F, 0.F});
    floor_obj_.transform().set_local_scale({10.F, 0.2F, 10.F});
    auto floor_model = kEn::Model::load("cube/cube.obj");
    floor_obj_.add_component(std::make_shared<kEn::ModelComponent>(floor_model));

    // --- Register lights (once -- SceneData never clears them) ---
    kEn::Renderer::add_light(point_light_);
    kEn::Renderer::add_light(dir_light_);
    kEn::Renderer::add_light(spot_light_);
    kEn::Renderer::set_ambient(ambient_color_);

    // --- Shader ---
    phong_shader_ = device_.create_shader("phong");

    // --- Shadow map (depth-only, 2048x2048) ---
    kEn::FramebufferSpec shadow_spec;
    shadow_spec.width                        = kShadowMapSize;
    shadow_spec.height                       = kShadowMapSize;
    shadow_spec.attachments.depth_attachment = kEn::TextureFormat::Depth32F;

    shadow_map_fb_ = device_.create_framebuffer(shadow_spec);
    shadow_shader_ = device_.create_shader("shadow");

    // --- Framebuffer ---
    kEn::FramebufferSpec fb_spec;
    fb_spec.width       = vp_w_;
    fb_spec.height      = vp_h_;
    fb_spec.attachments = {.color_attachments = {kEn::TextureFormat::RGBA8},
                           .depth_attachment  = kEn::TextureFormat::Depth24Stencil8};

    framebuffer_ = device_.create_framebuffer(fb_spec);

    // --- Pipeline state objects ---
    depth_state_       = device_.create_depth_state();
    raster_front_cull_ = device_.create_raster_state({.cull_mode = kEn::CullMode::Front});
    raster_back_cull_  = device_.create_raster_state();
    raster_wireframe_  = device_.create_raster_state({.fill_mode = kEn::FillMode::Wireframe});

    KEN_INFO("DemoLayer attached");
  }

  void on_detach() override { KEN_INFO("DemoLayer detached"); }

  void on_update(kEn::Timestep delta, kEn::Timestep time) override {
    camera_obj_.update_all(delta, time);
    model_obj_.update_all(delta, time);
    floor_obj_.update_all(delta, time);
    point_light_obj_.update_all(delta, time);
    dir_light_obj_.update_all(delta, time);
  }

  void on_render(double alpha) override {
    // --- Compute light-space matrix from directional light transform ---
    constexpr mEn::Mat4 kLightProj = mEn::ortho(-12.F, 12.F, -12.F, 12.F, 0.1F, 30.F);

    const mEn::Vec3 light_pos  = dir_light_obj_.transform().world_pos();
    const mEn::Mat4 light_view = mEn::lookAt(light_pos, mEn::Vec3{0.F}, mEn::Vec3{0.F, 1.F, 0.F});

    // --- Shadow pass ---
    device_.context().set_render_target(*shadow_map_fb_);
    device_.context().set_viewport(0, 0, kShadowMapSize, kShadowMapSize);
    shadow_map_fb_->clear_depth();
    device_.context().set_depth_state(*depth_state_);
    device_.context().set_raster_state(*raster_front_cull_);
    kEn::Renderer::begin_scene(light_pos, light_view, kLightProj);
    floor_obj_.render_all(*shadow_shader_, alpha);
    model_obj_.render_all(*shadow_shader_, alpha);
    kEn::Renderer::end_scene();

    // --- Main pass ---
    device_.context().set_render_target(*framebuffer_);
    device_.context().set_viewport(0, 0, vp_w_, vp_h_);
    device_.context().set_clear_color({0.08F, 0.08F, 0.12F, 1.F});
    device_.context().clear();
    device_.context().set_depth_state(*depth_state_);
    device_.context().set_raster_state(*raster_back_cull_);

    kEn::Renderer::begin_scene(*camera_);
    kEn::Renderer::prepare(*phong_shader_);

    device_.context().bind_attachment(kShadowMapSlot, kEn::ShaderStage::Fragment, *shadow_map_fb_->depth_attachment());
    phong_shader_->set_uniform("u_ShadowMap", static_cast<int>(kShadowMapSlot));
    phong_shader_->set_uniform("u_LightVP", kLightProj * light_view);
    phong_shader_->set_uniform("u_ShadowBias", shadow_bias_);
    phong_shader_->set_uniform("u_ShadowsEnabled", shadows_enabled_);

    floor_obj_.render_all(*phong_shader_, alpha);
    model_obj_.render_all(*phong_shader_, alpha);

    kEn::Renderer::end_scene();

    device_.context().bind_default_framebuffer();
    const kEn::Window& win = kEn::Application::instance().main_window();
    device_.context().set_viewport(0, 0, win.width(), win.height());
    device_.context().set_clear_color({0.2F, 0.2F, 0.2F, 1.F});
    device_.context().clear();
  }

  void on_imgui() override {
    ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

    // --- Viewport ---
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (ImGui::Begin("Viewport")) {
      viewport_focused_ = ImGui::IsWindowFocused();
      viewport_hovered_ = ImGui::IsWindowHovered();

      if (viewport_focused_ || viewport_hovered_) {
        ImGuiIO& io            = ImGui::GetIO();
        io.WantCaptureMouse    = false;
        io.WantCaptureKeyboard = false;
      }

      const ImVec2 size = ImGui::GetContentRegionAvail();
      const auto new_w  = static_cast<uint32_t>(size.x);
      const auto new_h  = static_cast<uint32_t>(size.y);
      if (new_w > 0 && new_h > 0 && (new_w != vp_w_ || new_h != vp_h_)) {
        vp_w_ = new_w;
        vp_h_ = new_h;
        framebuffer_->resize(vp_w_, vp_h_);
        camera_->set_projection(mEn::radians(fov_), static_cast<float>(vp_w_) / static_cast<float>(vp_h_), 0.01F,
                                200.F);
      }
      const auto tex_id = framebuffer_->color_attachment(0).imgui_id();
      ImGui::Image(tex_id, size, {0, 1}, {1, 0});
    }
    ImGui::End();
    ImGui::PopStyleVar();

    // --- Scene controls ---
    if (ImGui::Begin("Scene Controls")) {
      ImGui::SeparatorText("Camera");
      const mEn::Vec3 cam_pos = camera_obj_.transform().world_pos();
      ImGui::Text("Position: %.2f  %.2f  %.2f", static_cast<double>(cam_pos.x), static_cast<double>(cam_pos.y),
                  static_cast<double>(cam_pos.z));
      ImGui::TextDisabled("F = enter FPS mode   Escape = exit FPS mode");
      if (ImGui::SliderFloat("FOV", &fov_, 20.F, 120.F)) {
        camera_->set_projection(mEn::radians(fov_), static_cast<float>(vp_w_) / static_cast<float>(vp_h_), 0.01F,
                                200.F);
      }

      ImGui::SeparatorText("Rendering");
      if (ImGui::Checkbox("Wireframe (F1)", &wireframe_)) {
        device_.context().set_raster_state(wireframe_ ? *raster_wireframe_ : *raster_back_cull_);
      }
      if (ImGui::ColorEdit3("Ambient", mEn::value_ptr(ambient_color_))) {
        kEn::Renderer::set_ambient(ambient_color_);
      }
      ImGui::Checkbox("Shadows", &shadows_enabled_);
      if (shadows_enabled_) {
        ImGui::SliderFloat("Shadow Bias", &shadow_bias_, 0.0001F, 0.05F, "%.4f");
      }
    }
    ImGui::End();

    // --- Light inspector ---
    if (ImGui::Begin("Lights")) {
      if (ImGui::CollapsingHeader("Point Light")) {
        point_light_->imgui();
      }
      if (ImGui::CollapsingHeader("Directional Light")) {
        dir_light_->imgui();
        const float w = ImGui::GetContentRegionAvail().x;
        const float h =
            w * static_cast<float>(shadow_map_fb_->spec().height) / static_cast<float>(shadow_map_fb_->spec().width);
        const auto tex_id = shadow_map_fb_->depth_attachment()->imgui_id();
        ImGui::Image(tex_id, {w, h}, {0, 1}, {1, 0});
      }
      if (ImGui::CollapsingHeader("Spot Light (torch)")) {
        spot_light_->imgui();
      }
    }
    ImGui::End();

    // --- Model inspector ---
    if (ImGui::Begin("Model Inspector")) {
      model_obj_.imgui_all();
    }
    ImGui::End();
  }

  bool on_event(kEn::BaseEvent& event) override {
    if (!dispatcher_.dispatch(event)) {
      // Propagate to scene objects so components (FreeLook, Camera) handle window resize etc.
      camera_obj_.on_event(event);
    }
    return false;
  }

 private:
  bool on_key_pressed(kEn::KeyPressedEvent& event) {
    if (event.key() == kEn::key::f1) {
      wireframe_ = !wireframe_;
      device_.context().set_raster_state(wireframe_ ? *raster_wireframe_ : *raster_back_cull_);
    }
    return false;
  }

  bool on_mouse_scroll(kEn::MouseScrollEvent& event) {
    fov_ -= event.offset().y * 2.F;
    fov_ = std::clamp(fov_, 20.F, 120.F);
    camera_->set_projection(mEn::radians(fov_), static_cast<float>(vp_w_) / static_cast<float>(vp_h_), 0.01F, 200.F);
    return false;
  }

  bool on_window_resize(kEn::WindowResizeEvent& event) {
    vp_w_ = event.width();
    vp_h_ = event.height();
    return false;
  }

  kEn::EventDispatcher dispatcher_;
  kEn::Device& device_;

  // Parent declared before child so it outlives it (members destroyed in reverse declaration order)
  kEn::GameObject camera_obj_;
  kEn::GameObject spot_light_obj_;  // child of camera_obj_
  kEn::GameObject model_obj_;
  kEn::GameObject floor_obj_;
  kEn::GameObject point_light_obj_;
  kEn::GameObject dir_light_obj_;

  std::shared_ptr<kEn::PerspectiveCamera> camera_;
  std::shared_ptr<kEn::PointLight> point_light_;
  std::shared_ptr<kEn::DirectionalLight> dir_light_;
  std::shared_ptr<kEn::SpotLight> spot_light_;

  std::shared_ptr<kEn::Shader> phong_shader_;
  std::shared_ptr<kEn::Shader> shadow_shader_;
  std::shared_ptr<kEn::Framebuffer> framebuffer_;
  std::shared_ptr<kEn::Framebuffer> shadow_map_fb_;

  std::shared_ptr<kEn::DepthState> depth_state_;
  std::shared_ptr<kEn::RasterState> raster_front_cull_;
  std::shared_ptr<kEn::RasterState> raster_back_cull_;
  std::shared_ptr<kEn::RasterState> raster_wireframe_;
  float shadow_bias_    = 0.005F;
  bool shadows_enabled_ = true;

  static constexpr uint32_t kShadowMapSize = 2048;
  static constexpr uint32_t kShadowMapSlot = 15;

  bool viewport_focused_   = false;
  bool viewport_hovered_   = false;
  bool wireframe_          = false;
  mEn::Vec3 ambient_color_ = {0.05F, 0.05F, 0.08F};
  float fov_               = 70.F;
  uint32_t vp_w_           = 1280;
  uint32_t vp_h_           = 720;
};

class Sandbox : public kEn::Application {
 public:
  Sandbox() : kEn::Application({.title = "Sandbox", .enable_debug = true}) {
    push_layer(std::make_unique<DemoLayer>());
  }
};

}  // namespace

kEn::Application* kEn::create_application(std::vector<std::string_view> /*args*/) { return new Sandbox(); }  // NOLINT
