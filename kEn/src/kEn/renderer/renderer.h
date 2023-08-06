#pragma once

namespace kEn
{
	enum class renderer_api
	{
		none = 0,
		opengl = 1
	};

	class renderer
	{
	public:
		static renderer_api get_api() { return renderer_api_; }
	private:
		static renderer_api renderer_api_;
	};
}