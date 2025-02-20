#include <iostream>
#include <unordered_map> // unordered_map için
#include <typeindex>     // type_index için
#include <memory>        // shared_ptr için
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"

#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

#include "Input.h"

const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
Vertex vertices[] =
    { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
        Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
    };

// Indices for vertices order
GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

Vertex lightVertices[] =
    { //     COORDINATES     //
        Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
        Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
        Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
        Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}
    };

GLuint lightIndices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
glm::quat rotate(float angleX, float angleY, float angleZ);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Texture textures[]{
        Texture(RESOURCES_PATH "textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture(RESOURCES_PATH "textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)};

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram(RESOURCES_PATH "cube.vs", RESOURCES_PATH "cube.fs");
    // Store mesh data in vectors for the mesh
    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));

    // Create floor mesh
    GameObject floorObject;
    std::shared_ptr<Mesh> floor = std::make_shared<Mesh>(verts, ind, tex);
    floorObject.AddComponent(floor);
    floorObject.AddComponent<Transform>();

    // Shader for light cube
    Shader lightShader(RESOURCES_PATH "light.vs", RESOURCES_PATH  "light.fs");
    // Store mesh data in vectors for the mesh
    std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

    // Create light mesh
    GameObject lightObject;
    std::shared_ptr<Mesh> light = std::make_shared<Mesh>(lightVerts, lightInd, tex);
    lightObject.AddComponent(light);
    lightObject.AddComponent<Transform>();

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);

    lightShader.use();
    lightShader.setMatrix("model", 1, GL_FALSE, glm::value_ptr(lightModel));
    lightShader.setFloat("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.use();
    shaderProgram.setMatrix("model", 1, GL_FALSE, glm::value_ptr(objectModel));
    shaderProgram.setFloat("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.setFloat("lightPos", lightPos.x, lightPos.y, lightPos.z);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    while (!glfwWindowShouldClose(window))
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //floorObject.Update();

        // Handles camera inputss
        camera.Inputs(window);
        // Updates and exports the camera matrix to the Vertex Shaderaa
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        auto floorTrans = floorObject.GetComponent<Transform>();
        
        floorTrans->objectPos = glm::vec3(0.0f, 0.0f, 0.0f);

        // Draws different meshes
        floorObject.Render(shaderProgram, camera);
        lightObject.Render(lightShader, camera);


        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }
    // Delete all the objects we've created
    shaderProgram.Delete();
    lightShader.Delete();
    // Delete window before ending the program
    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

glm::quat rotate(float angleX, float angleY, float angleZ){
    glm::quat rotation = glm::angleAxis(glm::radians(angleX), glm::vec3(1, 0, 0)) * glm::angleAxis(glm::radians(angleY), glm::vec3(0, 1, 0)) * glm::angleAxis(glm::radians(angleZ), glm::vec3(0, 0, 1));
    return rotation;
}