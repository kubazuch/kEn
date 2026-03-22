#pragma once

/** @file
 *  @ingroup ken
 *
 *  @brief kEn application entry point.
 *
 *  Include this header **exactly once** in the client executable (typically
 *  in the same translation unit that defines @ref kEn::create_application()).
 *  It provides the @c main() function that bootstraps the engine:
 *
 *  1. Forwards command-line arguments to @ref kEn::create_application() as a
 *     @c std::vector<std::string_view>.
 *  2. Transfers ownership of the returned Application to a @c unique_ptr.
 *  3. Calls @ref kEn::Application::run() to start the main loop.
 *
 *  The NOLINT suppression for @c misc-definitions-in-headers is intentional --
 *  this file is a single-include bootstrap header, not a regular library header.
 */

#include <memory>
#include <string_view>
#include <vector>

#include <kEn/core/application.hpp>

extern kEn::Application* kEn::create_application(std::vector<std::string_view> args);

/** @brief kEn application entry point.
 *
 *  Constructed by the engine; do not define @c main() yourself when using kEn.
 *  Instead, implement @ref kEn::create_application() and include this header.
 *
 *  @param argc  Argument count from the OS.
 *  @param argv  Null-terminated argument vector from the OS.
 *  @return      0 on normal exit.
 */
int main(int argc, char** argv) {  // NOLINT(misc-definitions-in-headers)
  std::unique_ptr<kEn::Application> app(kEn::create_application({argv, argv + argc}));
  app->run();
}