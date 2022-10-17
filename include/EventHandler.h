#pragma once

#include <forward_list>


template <class EventArgs, class SenderType>
class EventHandler;

template <class EventArgs, class SenderType>
class EventInvoker
{
public:
	using MyHandler = EventHandler<EventArgs, SenderType>;
	/// <summary>
	/// Function signature of handled event
	/// </summary>
	using Event = void(*)(SenderType*, EventArgs);

	/// <summary>
	/// Notify all listeners of raised event
	/// </summary>
	/// <param name="sender">Sender of event</param>
	/// <param name="eventArgs">Arguments of event</param>
	constexpr void Raise(SenderType* const sender, EventArgs eventArgs)
	{
		for (const auto& listener : this->listeners)
			listener(sender, eventArgs);
	}

	constexpr MyHandler GetHandler() { return MyHandler(*this); }

	mutable std::forward_list<Event> listeners;
};

/// <summary>
/// Event handling class where potential listeners can subscribe
/// </summary>
/// <typeparam name="EventArgs">Event argument type</typeparam>
/// <typeparam name="SenderType">Type of the sender if given, else void</typeparam>
template <class EventArgs, class SenderType = void>
class EventHandler
{
public:
	using MyInvoker = EventInvoker<EventArgs, SenderType>;
	/// <summary>
	/// Function signature of handled event
	/// </summary>
	using Event = typename MyInvoker::Event;

	/// <summary>
	/// <c>EventHandler</c> objects should only be created from their <c>EventInvoker</c>
	/// </summary>
	/// <param name="invoker">Reference to invoker to handle</param>
	constexpr explicit EventHandler(MyInvoker& invoker) : _invoker(invoker) {}

	constexpr EventHandler(const EventHandler&) = default;
	EventHandler(EventHandler&&) noexcept = delete;
	constexpr EventHandler& operator=(const EventHandler&) = default;
	EventHandler& operator=(EventHandler&&) noexcept = delete;

	/// <summary>
	/// Add a new instance of given listener to this event
	/// </summary>
	/// <param name="listener">Listener to add</param>
	constexpr void operator+=(Event listener) const
	{
		_invoker.listeners.emplace_front(listener);
	}

	/// <summary>
	/// Remove all instances of given listener to this event
	/// </summary>
	/// <param name="listener">Listener to remove</param>
	constexpr void operator-=(Event listener) const
	{
		_invoker.listeners.remove(listener);
	}

	constexpr explicit operator bool() const noexcept { return !_invoker.listeners.empty(); }

private:
	MyInvoker& _invoker;
};
