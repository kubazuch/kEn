#pragma once

/** @file
 *  @ingroup ken
 */

#include <memory>
#include <string_view>
#include <vector>

#include <kEn/core/application.hpp>

extern kEn::Application* kEn::create_application(std::vector<std::string_view> args);

int main(int argc, char** argv) {  // NOLINT(misc-definitions-in-headers)
  std::unique_ptr<kEn::Application> app(kEn::create_application({argv, argv + argc}));
  app->run();
}