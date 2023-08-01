#pragma once

#include "event.h"
#include "kEn/core/key_codes.h"

namespace kEn
{
	class key_event
	{
	public:
		key_code key() const { return key_; }

	protected:
		explicit key_event(const key_code code) : key_(code) {}

		key_code key_;
	};

	class key_pressed_event : public event<key_pressed_event>, public key_event
	{
	public:
		key_pressed_event(const key_code code, bool repeat = false) : key_event(code), is_repeat_(repeat) {}

		bool is_repeat() const { return is_repeat_; }

		const char* name() const override { return "KeyPressedEvent"; }
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << name() << ": " << key_ << " (" << key::name_of(key_) << "), repeating = " << is_repeat_;
			return ss.str();
		}

	private:
		bool is_repeat_;
	};

	class key_released_event : public event<key_released_event>, public key_event
	{
	public:
		key_released_event(const key_code code) : key_event(code) {}
		
		const char* name() const override { return "KeyReleasedEvent"; }
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << name() << ": " << key_ << " (" << key::name_of(key_) << ")";
			return ss.str();
		}
	};

	class key_typed_event : public event<key_typed_event>, public key_event
	{
	public:
		key_typed_event(const key_code code) : key_event(code) {}
		

		const char* name() const override { return "KeyTypedEvent"; }
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << name() << ": " << key_ << " (" << static_cast<char>(key_) << ")";
			return ss.str();
		}
	};
}