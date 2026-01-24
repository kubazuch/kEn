#include "render_command.hpp"

#include <memory>

#include <kEn/renderer/renderer_api.hpp>

namespace kEn {
std::unique_ptr<RendererApi> RenderCommand::renderer_api_ = RendererApi::create();
}