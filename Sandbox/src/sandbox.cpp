#include <imgui/imgui.h>

#include <kEn.hpp>
#include <kEn/core/assert.hpp>
#include <kEn/core/transform.hpp>
#include <kEn/scene/camera/camera.hpp>
#include <kEn/scene/game_object.hpp>
#include <memory>

class fizzbuzz_layer : public kEn::layer {
 public:
  fizzbuzz_layer() : layer("FizzBuzz") {
    // camera_ = kEn::orthographic_camera(-1.f, 1.f, -1.f, 1.f);
    camera_ = std::make_shared<kEn::perspective_camera>(glm::radians(70.f), 1.0f, 0.01f, 100.f);
    object_ = std::make_shared<kEn::game_object>(glm::vec3{0, 0, 2});
    object_->add_component(camera_);

    float vertices[4 * (3 + 4)] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };

    unsigned int indices[6] = {0, 1, 2, 1, 2, 3};

    vertex_array_       = kEn::vertex_array::create();
    auto vertex_buffer_ = kEn::vertex_buffer::create(vertices, sizeof vertices);
    {
      kEn::buffer_layout layout = {{kEn::shader_data_types::float3, "a_Position"},
                                   {kEn::shader_data_types::float4, "a_Color"}};

      vertex_buffer_->set_layout(layout);
    }

    auto index_buffer_ = kEn::index_buffer::create(indices, 6);

    vertex_array_->add_vertex_buffer(vertex_buffer_);
    vertex_array_->set_index_buffer(index_buffer_);

    shader_ = kEn::shader::create("test");
  }

  void on_update(double delta, double time) override {
    shader_->bind();
    shader_->set_float("iTime", time);
    // camera_.set_rotation(glm::rotate(camera_.rotation(), (float) delta, { 0, 1.0f, 0.0f }));
    transform_.rotate({0, 1, 0}, (float)delta);
    transform_.set_local_pos({0, 0, sin(time)});
  }

  void on_render() override {
    kEn::render_command::set_clear_color({1.0f, 0.0f, 1.0f, 1.0f});
    kEn::render_command::clear();

    kEn::renderer::begin_scene(camera_);
    { kEn::renderer::submit(*shader_, *vertex_array_, transform_); }
    kEn::renderer::end_scene();
  }

  void on_attach() override { KEN_DEBUG("Attached!"); }

  void on_detach() override { KEN_DEBUG("Detached!"); }

  void on_imgui() override {
    ImGui::Begin("Fizzbuzz!");
    ImGui::Text("Fizz or buzz? That is the question...");

    const auto pos = kEn::input::get_mouse_pos();
    ImGui::Text("Mouse pos: %.1f, %.1f", pos.x, pos.y);
    ImGui::End();
  }

 private:
  float time_ = 0;

  std::shared_ptr<kEn::game_object> object_;
  std::shared_ptr<kEn::camera> camera_;
  kEn::transform transform_;

  std::shared_ptr<kEn::vertex_array> vertex_array_;
  std::shared_ptr<kEn::shader> shader_;
};

class sandbox : public kEn::application {
 public:
  sandbox() {
    srand(time(NULL));
    push_layer(new fizzbuzz_layer());
  }
};

kEn::application* kEn::create_application() { return new sandbox(); }
