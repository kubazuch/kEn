#pragma once
#include <kEn/core/application.hpp>
#include <kEn/core/log.hpp>
#include <memory>

#ifdef KEN_PLATFORM_WIN

extern kEn::Application* kEn::create_application();

int main(int /*argc*/, char** /*argv*/) {  // NOLINT
  kEn::Log::init();

  std::unique_ptr<kEn::Application> app(kEn::create_application());
  app->run();
}

#endif