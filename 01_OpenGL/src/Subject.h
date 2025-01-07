#pragma once
#include <vector>
#include "Observer.h"

class Subject
{
public:
	virtual void NotifyObservers() = 0;
	virtual void RegisterObservers(Observer& obs)
	{
		observers.push_back(&obs);
	}

	std::vector<Observer*> observers;
};