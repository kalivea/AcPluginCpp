#include "stdafx.h"
#include "CallbackManager.h"

CallbackManager& CallbackManager::getInstance()
{
	static CallbackManager instance;
	return instance;
}

void CallbackManager::registerCallback(int id, std::function<void(const AcDbObject*, AcDbObjectId)> callback)
{
	callbacks[id] = callback;
}

void CallbackManager::executeCallback(int id, const AcDbObject* id1, AcDbObjectId id2)
{
	if (callbacks.find(id) != callbacks.end())
	{
		callbacks[id](id1, id2);
	}
}
