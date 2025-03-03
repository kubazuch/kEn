#include <kEn/renderer/render_command.hpp>
#include <kEn/renderer/renderer_api.hpp>
#include <kenpch.hpp>

namespace kEn {
std::unique_ptr<RendererApi> RenderCommand::renderer_api_ = RendererApi::create();
}