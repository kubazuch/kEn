#pragma once
#include <cstddef>
#include <string>
#include <functional>

namespace kEn
{
	class base_event
	{
	public:
		bool handled = false;

		base_event() = default;
		virtual ~base_event() = default;

		virtual const char* name() const = 0;
		virtual std::string to_string() const { return name(); }

		base_event(const base_event&) = delete;
		base_event(base_event&&) = delete;
		base_event& operator=(const base_event& other) noexcept = delete;
		base_event& operator=(base_event&& other) noexcept = delete;

	protected: 
		static std::size_t next_id()
		{
			static std::size_t id = 0;
			return id++;
		}
	};

	template<typename EventType>
	class event : public base_event
	{
	public:

		static std::size_t id()
		{
			static auto id = base_event::next_id();
			return id;
		}
	};

	class event_dispatcher
	{
	public:
		template<typename EventType>
		using callback_t = std::function<bool(EventType&)>;

		template<typename EventType, typename F>
		void subscribe(const F& callback_fn)
		{
			const auto id = EventType::id();
			subscribers_[id].push_back(callback_wrapper<EventType>(callback_fn));
		}

		template<typename EventType>
		bool dispatch(EventType& event)
		{
			const auto id = EventType::id();
			for (auto& callback : subscribers_[id])
			{
				event.handled |= callback(event);
				if (event.handled)
					return true;
			}

			return false;
		}

	private:
		std::unordered_map<std::size_t, std::vector<callback_t<base_event>>> subscribers_;

		template<typename EventType>
		class callback_wrapper
		{
		public:
			template<typename F>
			explicit callback_wrapper(const F& callback) : callback_(callback) {}

			bool operator()(base_event& event)
			{
				return callback_(static_cast<EventType&>(event));
			}

		private:
			callback_t<EventType> callback_;
		};
	};

	template<typename EventType>
	std::ostream& operator<<(std::ostream& os, const event<EventType>& e)
	{
		return os << e.to_string();
	}
}
