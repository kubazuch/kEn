#pragma once

#include <kEn/core/core.hpp>

/**
 * @file model.hpp
 * @brief Dear ImGui inspector widgets for @ref Mesh and @ref Model.
 * @ingroup ken
 */

namespace kEn {

class Mesh;
class Model;

namespace ui {

/**
 * @brief Render a collapsing-header ImGui widget for @p mesh.
 *
 * Extracted from @ref Mesh to avoid coupling the core GPU primitive to the
 * ImGui library.  Include only in editor/debug translation units.
 *
 * @param mesh Mesh to inspect; its material properties are editable.
 */
void Mesh(kEn::Mesh& mesh);

/**
 * @brief Render a tree-node ImGui widget listing all meshes in @p model.
 *
 * Extracted from @ref Model to avoid coupling the asset type to the ImGui
 * library.  Include only in editor/debug translation units.
 *
 * @param model Model to inspect; mesh material properties are editable.
 */
void Model(kEn::Model& model);

}  // namespace ui
}  // namespace kEn
