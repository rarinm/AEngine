#pragma once
#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

#include "Component.h"
#include "GameObject.h"

class Mesh : public Component
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    // Store VAO in public so it can be used in the Draw function
    VAO VAO;

    // Initializes the mesh
    Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);

    // Draws the mesh
    void Draw(
        Shader &shader,
        Camera &camera
    );
    glm::quat rotate(glm::vec3 quarternion);
};