#pragma once
#include <glm/glm.hpp>

class GameObject;

class Component {
protected:
    GameObject* gameObject;
public:
    virtual ~Component() = default;
    void SetGameObject(GameObject* obj) { gameObject = obj; }

    virtual void Update() {};
};