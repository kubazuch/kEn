#pragma once
#include <kEn/core/application.hpp>
#include <kEn/core/log.hpp>
#include <memory>

#ifdef KEN_PLATFORM_WIN

extern kEn::application* kEn::create_application();

int main(int /*argc*/, char** /*argv*/) {
  kEn::log::init();

  std::unique_ptr<kEn::application> app(kEn::create_application());
  app->run();
}

#endif