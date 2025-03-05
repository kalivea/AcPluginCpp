#pragma once
#include "StdAfx.h"
#include <map>
#include <functional>
#define POLA_DLL_REACTOR
#include "ReactorDef.h"
class POLADLL_REACTOR_API CallbackManager
{
public:
    static CallbackManager& getInstance();
    void registerCallback(int id, std::function<void(const AcDbObject*, AcDbObjectId)> callback);
    void executeCallback(int id, const AcDbObject* id1, AcDbObjectId id2);

private:
    CallbackManager() {}
    std::map<int, std::function<void(const AcDbObject*, AcDbObjectId)>> callbacks;

    CallbackManager(const CallbackManager&) = delete;
    CallbackManager& operator=(const CallbackManager&) = delete;
};
