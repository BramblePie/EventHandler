#include <iostream>

#include "EventHandler.h"

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
	counter.AddOne();
	counter.AddOne();


	return 0;
}
