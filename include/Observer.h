#pragma once
#include <forward_list>


/// <summary>
/// Abstract class, observer of a subject
/// </summary>
/// <typeparam name="Subject">Type of observing subject</typeparam>
template <class Subject>
class Observer
{
protected:
	~Observer() = default;
public:
	/// <summary>
	/// Occurs when <c>Observable</c> has finished sending notifications
	/// </summary>
	virtual void OnComplete() = 0;
	/// <summary>
	/// Occurs when <c>Observable</c> has encountered an error handling the subject
	/// </summary>
	/// <param name="e">Observable raised exception</param>
	virtual void OnError(const std::exception& e) = 0;
	/// <summary>
	/// Occurs when <c>Observable</c> has observed a change in subject
	/// </summary>
	/// <param name="subject">Subject under observation</param>
	virtual void OnNext(const Subject& subject) = 0;
};

/// <summary>
/// Abstract class able to make a derived class member observable
/// </summary>
/// <typeparam name="Subject">Type of observing subject</typeparam>
template <class Subject>
class Observable
{
	std::forward_list<Observer<Subject>*> _observers;

protected:
	Observable() = default;

public:
	/// <summary>
	/// Unsubscriber token. 
	/// When this token is deleted it will unsubscribe the corresponding observer from its observable
	/// </summary>
	struct Unsubscriber
	{
		void Unsubscribe() const {_observable.Unsubscribe(_observer);}

		Unsubscriber(Observer<Subject>& observer, Observable<Subject>& observable)
			: _observer(observer), _observable(observable) {}

		/// <summary>
		/// Unsubscribe on destruction
		/// </summary>
		~Unsubscriber() { Unsubscribe(); }

		Unsubscriber(const Unsubscriber&) = delete;
		Unsubscriber(Unsubscriber&&) noexcept = default;
		Unsubscriber& operator=(const Unsubscriber&) = delete;
		Unsubscriber& operator=(Unsubscriber&&) noexcept = default;

	private:
		Observer<Subject>& _observer;
		Observable<Subject>& _observable;
	};

	/// <summary>
	/// Subscribes an observer to this observable
	/// </summary>
	/// <param name="observer">Observer to add</param>
	/// <returns>Unsubscriber token</returns>
	[[nodiscard]] Unsubscriber Subscribe(Observer<Subject>& observer)
	{
		_observers.push_front(&observer);
		return Unsubscriber(observer, *this);
	}

	/// <summary>
	/// Unsubscribe an observer from this observable
	/// </summary>
	/// <param name="observer">Observer to remove</param>
	void Unsubscribe(Observer<Subject>& observer)
	{
		_observers.remove(&observer);
	}

	/// <summary>
	/// Notify all observers this observable has finished sending notifications
	/// </summary>
	void NotifyComplete()
	{
		for (auto observer : _observers)
			observer->OnComplete();
		_observers.clear();
	}

	/// <summary>
	/// Notify all observers this observable has encountered an error handling the subject
	/// </summary>
	/// <param name="e">Observable raised exception</param>
	void NotifyError(const std::exception& e)
	{
		for (auto observer : _observers)
			observer->OnError(e);
	}

	/// <summary>
	/// Notify all observers this observable has observed a change in subject
	/// </summary>
	/// <param name="subject">Subject under observation</param>
	void NotifyNext(const Subject& subject)
	{
		for (auto observer : _observers)
			observer->OnNext(subject);
	}
};
