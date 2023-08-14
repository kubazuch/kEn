#include <kEn.h>
#include <kEn/core/assert.h>

#include <imgui/imgui.h>

class fizzbuzz_layer : public kEn::layer
{
public:
	fizzbuzz_layer() : layer("FizzBuzz")
	{
		float vertices[4 * (3 + 4)] = {
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		};

		unsigned int indices[6] = { 0, 1, 2, 1, 2, 3 };

		vertex_array_ = kEn::vertex_array::create();
		auto vertex_buffer_ = kEn::vertex_buffer::create(vertices, sizeof vertices);
		{
			kEn::buffer_layout layout = {
				{kEn::shader_data_types::float3, "a_Position"},
				{kEn::shader_data_types::float4, "a_Color"}
			};

			vertex_buffer_->set_layout(layout);
		}

		auto index_buffer_ = kEn::index_buffer::create(indices, 6);

		vertex_array_->add_vertex_buffer(vertex_buffer_);
		vertex_array_->set_index_buffer(index_buffer_);


		std::string vertex_src = R"(
				#version 330 core
				
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;
				
				out vec3 v_Pos;

				struct test {
					float f;
					vec3 pos;
					int[4] es;
				};

				uniform test[3] lena;

				void main() {
					v_Pos = a_Position;
					gl_Position = vec4(a_Position*2 + lena[2].pos * lena[1].es[2] * lena[0].es[1], 1.0);
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
				            //color = color * vec3(1., .25, 1.+size);

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

				    color = vec4(col, 1.0);
				}
			)";


			shader_ = kEn::shader::create("basic", vertex_src, fragment_src2);
	}

	void on_update(double delta, double time) override
	{
		shader_->bind();
		shader_->set_float("iTime", time);
	}

	void on_render() override
	{
		kEn::render_command::set_clear_color({ 1.0f, 0.0f, 1.0f, 1.0f });
		kEn::render_command::clear();

		kEn::renderer::begin_scene();
		{
			shader_->bind();

			kEn::renderer::submit(vertex_array_);
		}
		kEn::renderer::end_scene();
	}

	void on_attach() override
	{
		KEN_DEBUG("Attached!");
	}

	void on_detach() override
	{
		KEN_DEBUG("Detached!");
	}

	void on_imgui() override
	{
		ImGui::Begin("Fizzbuzz!");
		ImGui::Text("Fizz or buzz? That is the question...");

		const auto pos = kEn::input::get_mouse_pos();
		ImGui::Text("Mouse pos: %.1f, %.1f", pos.x, pos.y);
		ImGui::End();
	}

private:
	float time_ = 0;

	std::shared_ptr<kEn::vertex_array> vertex_array_;
	std::shared_ptr<kEn::shader> shader_;
};

class sandbox : public kEn::application
{
public:
	sandbox()
	{
		srand(time(NULL));
		push_layer(new fizzbuzz_layer());
	}

};

kEn::application* kEn::create_application()
{
	return new sandbox();
}
