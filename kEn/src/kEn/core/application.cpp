#include "kenpch.h"
#include "application.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "assert.h"
#include "kEn/event/application_events.h"

#ifdef KEN_PLATFORM_WIN
#	include "platform/win/win_window.h"
#endif

namespace kEn
{
	application* application::instance_ = nullptr;

	application::application()
	{
		KEN_CORE_ASSERT(!instance_, "App already exists!");
		instance_ = this;

		window_ = std::unique_ptr<window>(window::create());
		window_->set_event_handler([this](auto& event) { window_event_handler(event); });

		dispatcher_ = std::make_unique<event_dispatcher>();
		dispatcher_->subscribe<window_close_event>(KEN_EVENT_SUBSCRIBER(on_window_close));
		dispatcher_->subscribe<window_resize_event>(KEN_EVENT_SUBSCRIBER(on_window_resize));

		imgui_layer_ = new imgui_layer();
		push_overlay(imgui_layer_);

		//TODO: remove
		{
			float vertices[4 * (3 + 4)] = {
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			};

			unsigned int indices[6] = { 0, 1, 2, 1, 2, 3 };

			vertex_array_ = vertex_array::create();

			vertex_buffer_ = vertex_buffer::create(vertices, sizeof vertices);
			{
				buffer_layout layout = {
					{shader_data_types::float3, "a_Position"},
					{shader_data_types::float4, "a_Color"}
				};

				vertex_buffer_->set_layout(layout);
			}

			index_buffer_ = index_buffer::create(indices, 6);

			vertex_array_->add_vertex_buffer(vertex_buffer_);
			vertex_array_->set_index_buffer(index_buffer_);

			std::string vertex_src = R"(
				#version 330 core
				
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
				
				out vec3 v_Pos;

				void main() {
					v_Pos = a_Position;
					gl_Position = vec4(a_Position*2, 1.0);
				}
			)";

			// https://www.shadertoy.com/view/mtyGWy
			std::string fragment_src1 = R"(
				#version 330 core
				
				layout(location = 0) out vec4 color;
				
				in vec3 v_Pos;

				uniform float iTime;
				uniform vec2 iMouse;

				vec3 palette( float t ) {
				    vec3 a = vec3(0.5, 0.5, 0.5);
				    vec3 b = vec3(0.5, 0.5, 0.5);
				    vec3 c = vec3(1.0, 1.0, 1.0);
				    vec3 d = vec3(0.263,0.416,0.557);

				    return a + b*cos( 6.28318*(c*t+d) );
				}

				void main() {
					vec2 uv = vec2(v_Pos.x * 16.0/9.0, v_Pos.y)*2;
					vec2 uv0 = uv;
					vec3 finalColor = vec3(0.0);

					for (float i = 0.0; i < 4.0; i++) {
				        uv = fract(uv * 1.5) - 0.5;

				        float d = length(uv) * exp(-length(uv0));

				        vec3 col = palette(length(uv0) + i*.4 + iTime*.4);

				        d = sin(d*8. + iTime)/8.;
				        d = abs(d);

				        d = pow(0.01 / d, 1.2);

				        finalColor += col * d;
				    }
					
					color = vec4(finalColor, 1);
				}
			)";

			// https://www.shadertoy.com/view/ftt3R7
			std::string fragment_src2 = R"(
				#version 330 core
				
				layout(location = 0) out vec4 color;
				
				in vec3 v_Pos;
				
				#define NUM_LAYERS 10.

				uniform float iTime;
				uniform vec2 iMouse;

				mat2 Rot(float a) {
				  float c = cos(a), s = sin(a);
				  return mat2(c, -s, s, c);
				}

				float Star(vec2 uv, float flare) {
				    float col = 0.;
				    float d = length(uv);
				    float m = .02/d;
				    
				    float rays = max(0., 1. - abs(uv.x * uv.y * 1000.));
				    m += rays * flare;
				    uv *= Rot(3.1415/4.);
				    rays = max(0., 1. - abs(uv.x * uv.y * 1000.));
				    m += rays * .3 * flare;
				    
				    m *= smoothstep(1., .2, d);

				    return m;
				}

				float Hash21(vec2 p) {
				  p = fract(p * vec2(123.34, 456.21));
				  p += dot(p, p+45.32);
				  
				  return fract(p.x*p.y);
				}

				vec3 StarLayer(vec2 uv) {
				    vec3 col = vec3(0.);
				    
				    vec2 gv = fract(uv) - 0.5;
				    vec2 id = floor(uv);
				    
				    for(int y = -1; y <= 1; y++ ) {
				        for(int x = -1; x <= 1; x++) {
				            vec2 offs = vec2(x, y);

				            float n = Hash21(id + offs);
				            float size = fract(n*345.32);
				            
				            vec2 p = vec2(n, fract(n*34.));
				            
				            float star = Star(gv - offs - p + .5, smoothstep(.8, 1., size) * .6);
				            
				            vec3 hueShift = fract(n*2345.2)*vec3(.2, .3, .9)*123.2;

				            vec3 color = sin(hueShift) * .5 + .5;
				            color = color * vec3(1., .25, 1.+size);

				            star *= sin(iTime*3.+n*6.2831)*.4+1.;
				            col += star * size * color;
				        }
				    }
				    
				    return col;

				}

				vec2 N(float angle) {
				  return vec2(sin(angle), cos(angle));
				}

				void main() {
					vec2 uv = vec2(v_Pos.x * 16.0/9.0, v_Pos.y)*2;
					vec2 iResolution = vec2(1280.0, 720.0);
					vec2 M = (iMouse - iResolution.xy*.5)/iResolution.y;
				    float t = iTime * .01;
				    
				    uv.x = abs(uv.x);
				    uv.y += tan((5./6.) * 3.1415) * .5;

				    vec2 n = N((5./6.) * 3.1415);
				    float d = dot(uv - vec2(.5, 0.), n);
				    uv -= n * max(0., d) * 2.;

				    // col += smoothstep(.01, .0, abs(d));

				    n = N((2./3.) * 3.1415);
				    float scale = 1.;
				    uv.x += 1.5 / 1.25;
				    for(int i=0; i<5; i++) {
				        scale *= 1.25;
				        uv *= 1.25;
				        uv.x -= 1.5;

				        uv.x = abs(uv.x);
				        uv.x -= 0.5;
				        uv -= n * min(0., dot(uv, n)) * 2.;
				    }

					uv += M;
				
				    uv *= Rot(t);
				    vec3 col = vec3(0.);
				    
				    float layers = 10.;
				    
				    for(float i=0.; i < 1.; i+=1./NUM_LAYERS) {
				        float depth = fract(i+t);
				        float scale = mix(20., .5, depth);
				        float fade = depth * smoothstep(1., .9, depth);
				        col += StarLayer(uv * scale + i * 453.2) * fade;
				    }

				    color = vec4(col,1.0);
				}
			)";

			shader_ = shader::create("basic", vertex_src, fragment_src2);
		}
	}

	void application::push_layer(layer* layer)
	{
		layer_stack_.push_layer(layer);
	}

	void application::push_overlay(layer* overlay)
	{
		layer_stack_.push_overlay(overlay);
	}

	void application::run()
	{
		while (running_)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			//TODO: remove
			{
				vertex_array_->bind();
				shader_->bind();
				glDrawElements(GL_TRIANGLES, index_buffer_->get_count(), GL_UNSIGNED_INT, nullptr);
				shader_->unbind();
			}

			for (layer* layer : layer_stack_)
				layer->on_update();

			imgui_layer_->begin();
			for (layer* layer : layer_stack_)
				layer->on_imgui();
			imgui_layer_->end();

			window_->on_update();
		}
	}

	void application::window_event_handler(base_event& e)
	{
		dispatcher_->dispatch(e);

		for (auto it = layer_stack_.rbegin(); it != layer_stack_.rend(); ++it)
		{
			if (e.handled)
				break;
			(*it)->on_event(e);
		}
	}

	bool application::on_window_close(window_close_event& e)
	{
		running_ = false;
		return true;
	}

	bool application::on_window_resize(window_resize_event& e)
	{
		// TODO: Renderer abstraction!
		glViewport(0, 0, e.width(), e.height());
		return true;
	}
}
