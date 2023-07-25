#pragma once
#include "application.h"

#ifdef KEN_PLATFORM_WIN

extern kEn::application* kEn::create_application();

int main(int argc, char** argv)
{
	const auto app = kEn::create_application();
	app->run();
	delete app;
}

#endif