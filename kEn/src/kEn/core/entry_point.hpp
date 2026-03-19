#pragma once

/** @file
 *  @ingroup ken
 */

#include <memory>

#include <kEn/core/application.hpp>

#ifdef KEN_PLATFORM_WIN

extern kEn::Application* kEn::create_application();

int main(int /*argc*/, char** /*argv*/) {  // NOLINT(misc-definitions-in-headers)
  std::unique_ptr<kEn::Application> app(kEn::create_application());
  app->run();
}

#endif