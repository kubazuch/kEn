#pragma once

#include "kenpch.h"
#include "kEn/core/core.h"

#define KEN_EVENT_SUBSCRIBER(function) [this](auto& event) { return function(event); }

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
		virtual std::size_t event_id() = 0;

		VIRTUAL_FIVE(base_event);
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
		std::size_t event_id() override { return id(); }

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

		bool dispatch(base_event& event)
		{
			const auto id = event.event_id();
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

	inline std::ostream& operator<<(std::ostream& os, const base_event& e)
	{
		return os << e.to_string();
	}
}
