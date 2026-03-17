
#include <format>

#include <mEn/mat3.hpp>
#include <mEn/mat4.hpp>
#include <mEn/quat.hpp>
#include <mEn/vec2.hpp>
#include <mEn/vec3.hpp>
#include <mEn/vec4.hpp>

// NOLINTBEGIN(bugprone-std-namespace-modification)
namespace std {

template <mEn::length_t L, typename T>
struct formatter<mEn::vec<L, T>> {
  MEN_FORCE_INLINE constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  MEN_FORCE_INLINE auto format(const mEn::vec<L, T>& v, format_context& ctx) const {
    auto out = format_to(ctx.out(), "(");
    for (mEn::length_t i = 0; i < L; ++i) {
      if (i > 0) {
        out = format_to(out, ", ");
      }
      out = format_to(out, "{}", v[i]);
    }
    return format_to(out, ")");
  }
};

template <mEn::length_t N, typename T>
struct formatter<mEn::mat<N, T>> {
  MEN_FORCE_INLINE constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  MEN_FORCE_INLINE auto format(const mEn::mat<N, T>& m, format_context& ctx) const {
    auto out = format_to(ctx.out(), "(");
    for (mEn::length_t r = 0; r < N; ++r) {
      if (r > 0) {
        out = format_to(out, ", ");
      }
      out = format_to(out, "(");
      for (mEn::length_t c = 0; c < N; ++c) {
        if (c > 0) {
          out = format_to(out, ", ");
        }
        out = format_to(out, "{}", m[c][r]);
      }
      out = format_to(out, ")");
    }
    return format_to(out, ")");
  }
};

template <typename T>
struct formatter<mEn::qua<T>> {
  MEN_FORCE_INLINE constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

  MEN_FORCE_INLINE auto format(const mEn::qua<T>& q, format_context& ctx) const {
    return format_to(ctx.out(), "({}, {}, {}, {})", q.w, q.x, q.y, q.z);
  }
};

}  // namespace std
// NOLINTEND(bugprone-std-namespace-modification)
