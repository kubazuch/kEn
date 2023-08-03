#pragma once
#include "kEn/core/layer.h"

namespace kEn
{
	class imgui_layer : public layer
	{
	public:
		imgui_layer();
		~imgui_layer() override = default;

		void on_attach() override;
		void on_detach() override;
		void on_update() override {}
		void on_imgui() override;
		void on_event(base_event& event) override {}

		void begin();
		void end();

	private:

	};
}
