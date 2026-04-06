#pragma once

#include <kEn/core/core.hpp>

/**
 * @file light.hpp
 * @brief Dear ImGui inspector widgets for light components.
 * @ingroup ken
 */

namespace kEn {

class DirectionalLight;
class PointLight;
class SpotLight;

namespace ui {

/**
 * @brief Render an ImGui inspector widget for @p light.
 *
 * Extracted from @ref DirectionalLight to avoid coupling the scene layer to
 * the ImGui library.  Include only in editor/debug translation units.
 *
 * @param light Light to inspect; color is editable.
 */
void DirectionalLight(kEn::DirectionalLight& light);

/**
 * @brief Render an ImGui inspector widget for @p light.
 *
 * Extracted from @ref PointLight to avoid coupling the scene layer to the
 * ImGui library.  Include only in editor/debug translation units.
 *
 * @param light Light to inspect; color, position, and attenuation are editable.
 */
void PointLight(kEn::PointLight& light);

/**
 * @brief Render an ImGui inspector widget for @p light.
 *
 * Extracted from @ref SpotLight to avoid coupling the scene layer to the
 * ImGui library.  Include only in editor/debug translation units.
 *
 * @param light Light to inspect; color, position, attenuation, and cutoff angles are editable.
 */
void SpotLight(kEn::SpotLight& light);

}  // namespace ui
}  // namespace kEn
