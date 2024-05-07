//Author: DOYEONG LEE
//Project: CubeEngine
//File: Component.hpp
#pragma once
#include "ComponentTypes.hpp"

class Object;
class Engine;
class Component
{
public:
	friend class Object;

	Component(ComponentTypes type) : componentType(type) {};
	virtual ~Component() { engine = nullptr; };

	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void End() = 0;

	Object* GetOwner() const { return owner; }
	void SetOwner(Object* owner_) { this->owner = owner_; }
	ComponentTypes GetType() { return componentType; }

	void SetEngine(Engine* engine_) { engine = engine_; }
private:
	Object* owner = nullptr;
	ComponentTypes componentType;
protected:
	Engine* engine = nullptr;
};