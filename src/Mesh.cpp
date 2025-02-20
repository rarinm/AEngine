#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    VAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO EBO(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float)));
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::Draw(Shader &shader,
                Camera &camera
            )
{
    // Bind shader to be able to access uniforms
    shader.use();
    VAO.Bind();

    // Keep track of how many of each type of textures we have
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        std::string num;
        std::string type = textures[i].type;
        if (type == "diffuse")
        {
            num = std::to_string(numDiffuse++);
        }
        else if (type == "specular")
        {
            num = std::to_string(numSpecular++);
        }
        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }
    // Take care of the camera Matrix
    shader.setFloat("camPos", camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");

    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);

    auto Trans = gameObject->GetComponent<Transform>();

    trans = glm::translate(trans, Trans->objectPos);
    rot = glm::mat4_cast(rotate(Trans->objectQuaternion));
    sca = glm::scale(sca, Trans->objectScale);

    shader.setMatrix("translation", 1, GL_FALSE, glm::value_ptr(trans));
    shader.setMatrix("rotation", 1, GL_FALSE, glm::value_ptr(rot));
    shader.setMatrix("scale", 1, GL_FALSE, glm::value_ptr(sca));
    // Draw the actual mesh
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

glm::quat Mesh::rotate(glm::vec3 quarternion) {
    glm::quat rotation = glm::angleAxis(glm::radians(quarternion.x), glm::vec3(1, 0, 0)) * glm::angleAxis(glm::radians(quarternion.y), glm::vec3(0, 1, 0)) * glm::angleAxis(glm::radians(quarternion.z), glm::vec3(0, 0, 1));
    return rotation;
}