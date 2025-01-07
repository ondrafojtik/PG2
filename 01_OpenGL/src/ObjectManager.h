#pragma once

#include "Object.h"
#include "Ally.h"
#include "Enemy.h"
#include <vector>
#include <unordered_map>
#include <iostream>

class ObjectManager
{
public:
	void Add(Object& object)
	{
		if (typeid(object) == typeid(Enemy))
		{
			object.SetID(id_counter);
			objects.push_back(&object);
			id_counter += 1;
		}
		else
		{
			object.SetID(0);
			objects.push_back(&object);
		}
	}

	void RemoveWithID(int id)
	{
		for (int i = 0; i < objects.size(); i++)
			if (objects[i]->GetID() == id)
				objects.erase(objects.begin() + i);
	}

	std::vector<Object*> GetObjects() { return objects; }

private:
	int id_counter = 1;

	std::vector<Object*> objects;

};