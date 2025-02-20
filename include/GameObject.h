#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <glm/glm.hpp>

#include "Component.h"
#include "Mesh.h"
#include "Transform.h"

class GameObject  {
private:
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
public:

    // 1. Yeni bileşen oluşturup ekleyen metod (orijinal yöntem)
    template <typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args) {
        std::shared_ptr<T> component = std::make_shared<T>(std::forward<Args>(args)...);
        component->SetGameObject(this);
        components[typeid(T)] = component;
        return component;
    }

    // 2. Önceden oluşturulmuş bir bileşeni ekleyen metod
    template <typename T>
    std::shared_ptr<T> AddComponent(const std::shared_ptr<T>& component) {
        if (component) {
            component->SetGameObject(this);
            components[typeid(T)] = component;
        }
        return component;
    }

    template <typename T>
    std::shared_ptr<T> GetComponent() {
        auto it = components.find(typeid(T));
        if (it != components.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    glm::quat rotate(glm::vec3 quarternion);
    void Render(Shader& shader, Camera& camera);

    //void Update();
};