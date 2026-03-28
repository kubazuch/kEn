#include <imgui/imgui.h>

#include <algorithm>
#include <cstdint>
#include <memory>

#include <mEn/features/type_ptr.hpp>
#include <mEn/functions/trigonometric.hpp>
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

    // --- Register lights (once — SceneData never clears them) ---
    kEn::Renderer::add_light(point_light_);
    kEn::Renderer::add_light(dir_light_);
    kEn::Renderer::add_light(spot_light_);
    kEn::Renderer::set_ambient(ambient_color_);

    // --- Shader ---
    phong_shader_ = device_.create_shader("phong");

    // --- Framebuffer ---
    kEn::FramebufferSpec fb_spec;
    fb_spec.width       = vp_w_;
    fb_spec.height      = vp_h_;
    fb_spec.attachments = {.color_attachments = {kEn::TextureFormat::RGBA8},
                           .depth_attachment  = kEn::TextureFormat::Depth24Stencil8};
    framebuffer_        = device_.create_framebuffer(fb_spec);

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
    framebuffer_->bind_for_rendering();
    device_.command().set_clear_color({0.08F, 0.08F, 0.12F, 1.F});
    device_.command().clear();
    device_.command().depth_testing(true);

    kEn::Renderer::begin_scene(camera_);
    kEn::Renderer::prepare(*phong_shader_);

    floor_obj_.render_all(*phong_shader_, alpha);
    model_obj_.render_all(*phong_shader_, alpha);

    kEn::Renderer::end_scene();

    device_.command().bind_default_framebuffer();
    const kEn::Window& win = kEn::Application::instance().main_window();
    device_.command().set_viewport(0, 0, win.width(), win.height());
    device_.command().set_clear_color({0.2F, 0.2F, 0.2F, 1.F});
    device_.command().clear();
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
      const auto tex_id = static_cast<ImTextureID>(framebuffer_->native_color_attachment_handle(0));
      ImGui::Image(tex_id, size, ImVec2(0, 1), ImVec2(1, 0));
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
        device_.command().set_wireframe(wireframe_);
      }
      if (ImGui::ColorEdit3("Ambient", mEn::value_ptr(ambient_color_))) {
        kEn::Renderer::set_ambient(ambient_color_);
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
      device_.command().set_wireframe(wireframe_);
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
  std::shared_ptr<kEn::Framebuffer> framebuffer_;

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
