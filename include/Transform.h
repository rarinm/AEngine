#pragma once
#include "Component.h"
#include "GameObject.h"

class Transform : public Component {
public:
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 objectQuaternion = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 objectScale = glm::vec3(1.0f, 1.0f, 1.0f);
};