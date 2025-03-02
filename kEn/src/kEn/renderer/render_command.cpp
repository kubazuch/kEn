#include <kEn/renderer/render_command.hpp>
#include <kenpch.hpp>

namespace kEn {
std::unique_ptr<renderer_api> render_command::renderer_api_ = renderer_api::create();
}