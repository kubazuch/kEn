#include "kenpch.h"
#include "light.h"

#include "imgui.h"
#include "shader.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"

namespace kEn
{
	inline void attenuation::load(const std::string& name, shader& shader) const
	{
		//if (constant.is_dirty())
		{
			shader.set_float(name + ".constant", constant);
			constant.clear_dirty();
		}
		//if (linear.is_dirty())
		{
			shader.set_float(name + ".linear", linear);
			linear.clear_dirty();
		}
		//if (quadratic.is_dirty())
		{
			shader.set_float(name + ".quadratic", quadratic);
			quadratic.clear_dirty();
		}
	}

	void directional_light::imgui(bool subsection)
	{
		if (subsection)
		{
			if (ImGui::TreeNode("Directional light"))
			{
				imgui( false);
				ImGui::TreePop();
			}

			return;
		}

		if (ImGui::ColorEdit3("Color##light", glm::value_ptr(color.get())))
		{
			color.set_dirty();
		}
		glm::vec3 front = transform.front();
		ImGui::InputFloat3("Dir##light", glm::value_ptr(front));
	}

	void directional_light::load(const std::string& name, shader& shader) const
	{
		//if (color.is_dirty())
		{
			shader.set_float3(name + ".color", color);
			color.clear_dirty();
		}

		shader.set_float3(name + ".dir", transform.front());
	}

	void point_light::imgui(bool subsection)
	{
		if (subsection)
		{
			if (ImGui::TreeNode("Point light"))
			{
				imgui(false);
				ImGui::TreePop();
			}

			return;
		}

		if (ImGui::ColorEdit3("Color##light", glm::value_ptr(color.get())))
		{
			color.set_dirty();
		}
		if (ImGui::DragFloat3("Pos##light", glm::value_ptr(transform.pos().get()), 0.01f))
		{
			transform.set_pos(transform.pos());
		}

		if (ImGui::SliderFloat("Constant", &atten.constant.get(), 0, 2))
		{
			atten.constant.set_dirty();
		}

		if (ImGui::SliderFloat("Linear", &atten.linear.get(), 0, 2))
		{
			atten.linear.set_dirty();
		}

		if (ImGui::SliderFloat("Quadratic", &atten.quadratic.get(), 0, 2))
		{
			atten.quadratic.set_dirty();
		}
	}

	void point_light::load(const std::string& name, shader& shader) const
	{
		//if (color.is_dirty())
		{
			shader.set_float3(name + ".color", color);
			color.clear_dirty();
		}

		atten.load(name + ".atten", shader);

		shader.set_float3(name + ".pos", transform.transformed_pos());
	}

	void spot_light::imgui(bool subsection)
	{
		if (subsection)
		{
			if (ImGui::TreeNode("Point light"))
			{
				imgui(false);
				ImGui::TreePop();
			}

			return;
		}

		if (ImGui::ColorEdit3("Color##light", glm::value_ptr(color.get())))
		{
			color.set_dirty();
		}
		if (ImGui::DragFloat3("Pos##light", glm::value_ptr(transform.pos().get()), 0.01f))
		{
			transform.set_pos(transform.pos());
		}

		if (ImGui::SliderFloat("Constant", &atten.constant.get(), 0, 2))
		{
			atten.constant.set_dirty();
		}

		if (ImGui::SliderFloat("Linear", &atten.linear.get(), 0, 2))
		{
			atten.linear.set_dirty();
		}

		if (ImGui::SliderFloat("Quadratic", &atten.quadratic.get(), 0, 2))
		{
			atten.quadratic.set_dirty();
		}


		if (ImGui::SliderFloat("Outer angle", &outer_cutoff_angle.get(), 0, glm::pi<float>()/2))
		{
			outer_cutoff_angle.set_dirty();
		}

		if (ImGui::SliderFloat("Inner angle", &inner_cutoff_angle.get(), 0, glm::pi<float>()/2))
		{
			inner_cutoff_angle.set_dirty();
		}
	}

	void spot_light::load(const std::string& name, shader& shader) const
	{
		//if (color.is_dirty())
		{
			shader.set_float3(name + ".color", color);
			color.clear_dirty();
		}

		atten.load(name + ".atten", shader);

		shader.set_float3(name + ".pos", transform.transformed_pos());
		shader.set_float3(name + ".dir", glm::normalize(transform.front()));

		//if (inner_cutoff_angle.is_dirty())
		{
			shader.set_float(name + ".cutoff", glm::cos(inner_cutoff_angle));
			inner_cutoff_angle.clear_dirty();
		}

		//if (outer_cutoff_angle.is_dirty())
		{
			shader.set_float(name + ".outerCutoff", glm::cos(outer_cutoff_angle));
			outer_cutoff_angle.clear_dirty();
		}
	}
}
