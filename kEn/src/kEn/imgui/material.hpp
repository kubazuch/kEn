#pragma once

#include <kEn/core/core.hpp>

/** @file
 *  @ingroup ken
 */

namespace kEn {

class Material;

namespace ui {

/**
 * @brief Render a Dear ImGui inspector panel for @p mat.
 *
 * Extracted from @ref Material to avoid coupling the core rendering type to
 * the ImGui library. Include only in editor/debug translation units.
 *
 * @param mat Material to inspect and potentially modify.
 */
void Material(kEn::Material& mat);

}  // namespace ui
}  // namespace kEn
