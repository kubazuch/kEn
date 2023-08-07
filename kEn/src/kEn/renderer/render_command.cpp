#include "kenpch.h"
#include "render_command.h"

namespace kEn
{
	std::unique_ptr<renderer_api> render_command::renderer_api_ = renderer_api::create();
}