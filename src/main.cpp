#include <iostream>

#include "EventHandler.h"

struct Counter
{
	EventHandler<int, Counter> CountReached = _invoker.GetHandler();

	int count = 0;

	void TestCount()
	{
		if (count < 3)
			return;

		_invoker.Raise(this, count);
	}

private:
	EventInvoker<int, Counter> _invoker;
};

int main(int, char*[])
{
	Counter counter;

	counter.CountReached += [](auto sender, auto e)
	{
		std::cout << "send by: " << typeid(*sender).name() << ", with: " << e << std::endl;
	};

	counter.TestCount();
	counter.count += 7;
	counter.TestCount();

	system("pause");
	return 0;
}
