#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath);
    // use/activate the shader
    void use();

    // Deletes the Shader Program
    void Delete();

    // utility uniform functions
    void setInt(const std::string &name, int v0) const;
    void setInt(const std::string &name, int v0, int v1) const;
    void setInt(const std::string &name, int v0, int v1, int v2) const;
    void setInt(const std::string &name, int v0, int v1, int v2, int v3) const;

    void setFloat(const std::string &name, float v0) const;
    void setFloat(const std::string &name, float v0, float v1) const;
    void setFloat(const std::string &name, float v0, float v1, float v2) const;
    void setFloat(const std::string &name, float v0, float v1, float v2, float v3) const;

    void setMatrix(const std::string &name, GLsizei count, GLboolean transpose, const GLfloat *value);

private:
    // Checks if the different Shaders have compiled properly
    void compileErrors(unsigned int shader, const char *type);
};