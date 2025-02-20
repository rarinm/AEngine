#include "GameObject.h"
/*void GameObject::Update() {
    for (auto& component : components) {
        component->Update();
    }
}*/

glm::quat GameObject::rotate(glm::vec3 quarternion) {
    glm::quat rotation = glm::angleAxis(glm::radians(quarternion.x), glm::vec3(1, 0, 0)) * glm::angleAxis(glm::radians(quarternion.y), glm::vec3(0, 1, 0)) * glm::angleAxis(glm::radians(quarternion.z), glm::vec3(0, 0, 1));
    return rotation;
}
void GameObject::Render(Shader& shader, Camera& camera) {
    auto mesh = GetComponent<Mesh>();
    mesh->Draw(shader, camera);
}