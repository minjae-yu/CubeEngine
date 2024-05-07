//Author: DOYEONG LEE
//Project: CubeEngine
//File: ObjectManager.hpp
#pragma once
#include "Object.hpp"

#include <algorithm>
#include <map>
#include <memory>
#include <vector>
#include <iostream>

class Engine;
class ObjectManager
{
public:
    ObjectManager()  = default;
    ~ObjectManager() { engine = nullptr; }

	void Update(float dt);
    void End();

    template <typename T, typename... Args>
    void AddObject(Args... args)
    {
        //auto object = std::unique_ptr<T>(std::make_unique<T>(std::forward<Args>(args)...));
        ////T& objectRef = *object.get();
        //static_cast<Object&>(*object.get()).SetId(lastObjectID);

        objectMap[lastObjectID] = std::unique_ptr<T>(std::make_unique<T>(std::forward<Args>(args)...));;
        objectMap[lastObjectID].get()->SetId(lastObjectID);
        objectMap[lastObjectID].get()->SetEngine(engine);
        ++lastObjectID;
        //return static_cast<T&>(*objectMap[lastObjectID - 1].get());
    }

    void  Draw(float dt);
    void  Destroy(int id);
    void  DestroyAllObjects();

    int   GetLastObjectID() { return (lastObjectID - 1); }
    std::map<int, std::unique_ptr<Object>>& GetObjectMap() { return objectMap; }

    Object* FindObjectWithName(std::string name);
    Object* FindObjectWithId(int id) { return objectMap.at(id).get(); }
    Object* GetLastObject() { return objectMap.at(lastObjectID - 1).get(); }

    void SetEngine(Engine* engine_) { engine = engine_; }
private:
    int                                    lastObjectID = 0;
    std::map<int, std::unique_ptr<Object>> objectMap;
    std::vector<int>                       objectsToBeDeleted; // list of object id to be deleted

    Engine* engine = nullptr;
};