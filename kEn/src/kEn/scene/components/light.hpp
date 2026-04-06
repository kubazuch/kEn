#pragma once

/**
 * @file light.hpp
 * @brief Light components and attenuation for the kEn scene system.
 * @ingroup ken
 */

#include <string_view>

#include <mEn/vec3.hpp>

#include <kEn/core/core.hpp>
#include <kEn/scene/component.hpp>

namespace kEn {

class Shader;

/**
 * @brief Quadratic attenuation model parameterized by an effective radius.
 *
 * Stores precomputed linear and quadratic coefficients derived from the radius.
 * Construct via the named constructors; the struct is immutable after creation.
 *
 * The attenuation factor evaluated by the shader is:
 * @f[ f(d) = \frac{1}{1 + l \cdot d + q \cdot d^2} @f]
 *
 * where @f$ l @f$ and @f$ q @f$ are the linear and quadratic coefficients.
 */
struct Attenuation {
  /**
   * @brief Constructs an @ref Attenuation from an effective radius.
   *
   * The effective radius @p radius is the distance at which the attenuation
   * factor drops below a visually negligible threshold.
   * Coefficients: @f$ l = 4.5 / r,\; q = 75 / r^2 @f$.
   *
   * @param radius Effective light radius in world units; must be positive.
   */
  [[nodiscard]] static Attenuation from_radius(float radius) {
    return {radius, 4.5F / radius, 75.0F / (radius * radius)};
  }

  /**
   * @brief Constructs an @ref Attenuation from a half-life distance.
   *
   * The half-life @p half_life is the distance at which @f$ f(h) = 0.5 @f$.
   * Coefficients: @f$ l = 0.4 / h,\; q = 0.6 / h^2 @f$.
   * The corresponding effective radius is approximately @f$ r \approx 11h @f$.
   *
   * @param half_life Distance at which the light drops to half power; must be positive.
   */
  [[nodiscard]] static Attenuation from_half_life(float half_life) {
    return {11.0F * half_life, 0.4F / half_life, 0.6F / (half_life * half_life)};
  }

  /** @brief Returns the effective radius used to derive the coefficients. */
  [[nodiscard]] float radius() const { return radius_; }

  /** @brief Returns the approximate half-life distance (@f$ r / 11 @f$). */
  [[nodiscard]] float half_life() const { return radius_ / 11.0F; }

  /** @brief Returns the linear attenuation coefficient @f$ l @f$. */
  [[nodiscard]] float linear() const { return linear_; }

  /** @brief Returns the quadratic attenuation coefficient @f$ q @f$. */
  [[nodiscard]] float quadratic() const { return quadratic_; }

 private:
  Attenuation(float radius, float linear, float quadratic) : radius_(radius), linear_(linear), quadratic_(quadratic) {}

  float radius_    = 0.0F;
  float linear_    = 0.0F;
  float quadratic_ = 0.0F;
};

/**
 * @brief Abstract base for all light components.
 *
 * Carries a RGB @ref color and requires subclasses to implement
 * @ref load (shader upload) and @ref clone (deep copy for serialization).
 * The direction or position of the light is provided by the owning
 * @ref GameObject's @ref Transform.
 */
class BaseLight : public GameComponent {
 public:
  BaseLight()           = default;
  ~BaseLight() override = default;

  /** @brief Returns a deep copy of this light component. */
  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override = 0;

  /**
   * @brief Uploads this light's parameters to @p shader under the given @p name prefix.
   *
   * The prefix corresponds to the GLSL struct instance name,
   * e.g. @c "u_DirLight" maps to @c u_DirLight.color, @c u_DirLight.dir, etc.
   *
   * @param name   GLSL uniform struct prefix.
   * @param shader Shader to receive the uniforms.
   */
  virtual void load(std::string_view name, Shader& shader) const = 0;

  /** @brief RGB light color; defaults to white @f$ (1, 1, 1) @f$. */
  mEn::Vec3 color{1.F};
};

/**
 * @brief Infinitely distant directional light.
 *
 * Direction is taken from the owning @ref Transform's world forward vector.
 * Uploads @c .color and @c .dir to the shader.
 */
class DirectionalLight : public BaseLight {
 public:
  void imgui() override;

  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

  void load(std::string_view name, Shader& shader) const override;
};

/**
 * @brief Omnidirectional point light with quadratic attenuation.
 *
 * Position is taken from the owning @ref Transform's world position.
 * Uploads @c .color, @c .pos, @c .atten.linear, and @c .atten.quadratic to the shader.
 */
class PointLight : public BaseLight {
 public:
  void imgui() override;

  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

  void load(std::string_view name, Shader& shader) const override;

  /** @brief Attenuation parameters derived from the effective radius. */
  Attenuation atten = Attenuation::from_radius(50.0F);
};

/**
 * @brief Cone-shaped spotlight with quadratic attenuation and soft edges.
 *
 * Position and direction are taken from the owning @ref Transform.
 * The cone is defined by an inner and outer cutoff angle (in degrees); the
 * shader receives their cosines.  Intensity falls off smoothly between the
 * two angles.  The invariant @f$ \theta_{inner} \le \theta_{outer} @f$ is
 * enforced by @ref set_cutoff_angles.
 *
 * Uploads @c .color, @c .pos, @c .dir, @c .atten.linear, @c .atten.quadratic,
 * @c .cutoff, and @c .outerCutoff to the shader.
 */
class SpotLight : public BaseLight {
 public:
  void imgui() override;

  [[nodiscard]] std::unique_ptr<GameComponent> clone() const override;

  void load(std::string_view name, Shader& shader) const override;

  /**
   * @brief Sets the inner and outer cutoff angles of the spotlight cone.
   *
   * @pre @p inner_deg <= @p outer_deg.
   * @param inner_deg Full-intensity cone half-angle in degrees.
   * @param outer_deg Zero-intensity cone half-angle in degrees.
   */
  void set_cutoff_angles(float inner_deg, float outer_deg);

  /** @brief Returns the inner cutoff half-angle in degrees. */
  [[nodiscard]] float inner_cutoff_angle() const { return inner_cutoff_angle_; }

  /** @brief Returns the outer cutoff half-angle in degrees. */
  [[nodiscard]] float outer_cutoff_angle() const { return outer_cutoff_angle_; }

  /** @brief Attenuation parameters derived from the effective radius. */
  Attenuation atten = Attenuation::from_radius(50.0F);

 private:
  float inner_cutoff_angle_ = 5.0F;
  float outer_cutoff_angle_ = 10.0F;
};

}  // namespace kEn
