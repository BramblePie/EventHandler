#include <iostream>

#include "EventHandler.h"
#include "Observer.h"

struct Counter
{
	EventHandler<int, Counter> CountReached = _invoker.GetHandler();

	int count = 0;

	void AddOne()
	{
		if (++count < 3)
			return;

		_invoker.Raise(this, count);
	}

private:
	EventInvoker<int, Counter> _invoker;
};

void OnThree(Counter* counter, int i)
{
	std::cout << "whoopee three reached\n";
	counter->count = 0;
}


struct Person
{
	std::string name;
	int age;
};

class PersonMonitor : public Observable<Person>
{
	Person _person;
public:
	const Person& GetPerson() const { return _person; }

	void SetPerson(const Person& person)
	{
		_person = person;
		NotifyNext(person);
	}
};

class PersonReporter : public Observer<Person>
{
	std::string _name;
public:
	explicit PersonReporter(const std::string& name) : _name(name) {}
	virtual ~PersonReporter() = default;

	void OnComplete() override {}
	void OnError(const std::exception&) override {}

	void OnNext(const Person& person) override
	{
		std::cout << _name << " saw : \"" << person.name << "\"\n";
	}
};

int main(int, char*[])
{
	Counter counter;
	counter.CountReached += [](auto sender, auto e)
	{
		std::cout << "send by: " << typeid(*sender).name() << ", with: " << e << std::endl;
	};
	counter.CountReached += OnThree;
	counter.AddOne();
	counter.AddOne();
	counter.AddOne();
	counter.AddOne();
	counter.CountReached -= OnThree;
	counter.AddOne();
	counter.AddOne();
	counter.AddOne();

	PersonMonitor monitor;
	PersonReporter reporter("consumer #1");
	auto token = monitor.Subscribe(reporter);
	monitor.SetPerson({ "barry allen", 31 });
	//monitor.Unsubscribe(reporter);
	//token.Unsubscribe();
	monitor.SetPerson({ "voldemort", 72 });

	return 0;
}
