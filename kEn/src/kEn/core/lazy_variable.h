#pragma once

namespace kEn
{
	template<typename T>
	class lazy_variable
	{
	public:
		lazy_variable(const T& value) : value_(value), dirty_(true) {}
		bool is_dirty() const { return dirty_; }
		void set_dirty() const { dirty_ = true; }
		void clear_dirty() const { dirty_ = false; }

		T& get() { return value_; }
		const T& get() const { return value_; }

		lazy_variable& operator=(const T& value) { value_ = value; dirty_ = true; return *this; }
		operator T&() { return value_; }
		operator const T&() const { return value_; }

	private:
		T value_;
		mutable bool dirty_;
	};
}