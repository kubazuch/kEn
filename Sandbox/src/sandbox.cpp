#include <imgui/imgui.h>

#include <chrono>
#include <kEn.hpp>
#include <kEn/core/assert.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/renderer/buffer.hpp>
#include <kEn/renderer/render_command.hpp>
#include <kEn/renderer/vertex_array.hpp>
#include <kEn/scene/camera/camera.hpp>
#include <kEn/scene/game_object.hpp>
#include <memory>

class FizzbuzzLayer : public kEn::Layer {
 public:
  FizzbuzzLayer() : Layer("FizzBuzz") {
    camera_ = std::make_shared<kEn::PerspectiveCamera>(mEn::radians(70.F), 1.0F, 0.01F, 100.F);
    object_ = std::make_shared<kEn::GameObject>(mEn::Vec3{0, 0, 2});
    object_->add_component(camera_);

    float vertices[4 * (3 + 4)] = {
        -0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F, 1.0F,  //
        0.5F,  -0.5F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F,  //
        -0.5F, 0.5F,  0.0F, 0.0F, 0.0F, 1.0F, 1.0F,  //
        0.5F,  0.5F,  0.0F, 0.0F, 0.0F, 1.0F, 1.0F,  //
    };

    unsigned int indices[2 * 3] = {
        0, 1, 2,  //
        1, 3, 2   //
    };

    vertex_array_      = kEn::VertexArray::create();
    auto vertex_buffer = kEn::VertexBuffer::create(vertices, sizeof vertices);
    {
      kEn::BufferLayout layout = {{kEn::shader_data_types::float3, "a_Position"},
                                  {kEn::shader_data_types::float4, "a_Color"}};

      vertex_buffer->set_layout(layout);
    }

    auto index_buffer = kEn::IndexBuffer::create(indices, 6);

    vertex_array_->add_vertex_buffer(vertex_buffer);
    vertex_array_->set_index_buffer(index_buffer);

    shader_ = kEn::Shader::create("test");
  }

  void on_update(kEn::duration_t delta, kEn::duration_t time) override {
    const float seconds_delta = std::chrono::duration<float>(delta).count();
    const float seconds_time  = std::chrono::duration<float>(time).count();

    shader_->bind();
    shader_->set_float("iTime", seconds_time);
    // camera_.set_rotation(glm::rotate(camera_.rotation(), (float) delta, { 0, 1.0F, 0.0F }));
    transform_.rotate({0, 1, 0}, seconds_delta);
    transform_.set_local_pos({0, 0, sin(seconds_time)});
  }

  void on_render() override {
    kEn::RenderCommand::set_clear_color({1.0F, 0.0F, 1.0F, 1.0F});
    kEn::RenderCommand::clear();

    kEn::Renderer::begin_scene(camera_);
    { kEn::Renderer::submit(*shader_, *vertex_array_, transform_); }
    kEn::Renderer::end_scene();
  }

  void on_attach() override { KEN_DEBUG("Attached!"); }

  void on_detach() override { KEN_DEBUG("Detached!"); }

  void on_imgui() override {
    ImGui::Begin("Fizzbuzz!");
    ImGui::Text("Fizz or buzz? That is the question...");

    const auto pos = kEn::Input::get_mouse_pos();
    ImGui::Text("Mouse pos: %.1f, %.1f", pos.x, pos.y);
    ImGui::End();
  }

 private:
  std::shared_ptr<kEn::GameObject> object_;
  std::shared_ptr<kEn::Camera> camera_;
  kEn::Transform transform_;

  std::shared_ptr<kEn::VertexArray> vertex_array_;
  std::shared_ptr<kEn::Shader> shader_;
};

class Sandbox : public kEn::Application {
 public:
  Sandbox() {
    srand(time(NULL));
    push_layer(new FizzbuzzLayer());
  }
};

kEn::Application* kEn::create_application() { return new Sandbox(); }
