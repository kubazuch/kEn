#include <kEn.h>

class sandbox : public kEn::application
{
public:
	sandbox() {}
	~sandbox() {}
};

kEn::application* kEn::create_application()
{
	return new sandbox();
}
