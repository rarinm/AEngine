#include "Shader.h"
// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    // Read vertexFile and fragmentFile and store the strings
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    // Convert the shader source strings into character arrays
    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    // Create Vertex Shader Object and get its reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);
    // Checks if Shader compiled succesfully
    compileErrors(vertexShader, "VERTEX");

    // Create Fragment Shader Object and get its reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Compile the Vertex Shader into machine code
    glCompileShader(fragmentShader);
    // Checks if Shader compiled succesfully
    compileErrors(fragmentShader, "FRAGMENT");

    // Create Shader Program Object and get its reference
    ID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(ID);
    // Checks if Shaders linked succesfully
    compileErrors(ID, "PROGRAM");

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char *type)
{
    // Stores status of compilation
    GLint hasCompiled;
    // Character array to store error message in
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n"
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR for:" << type << "\n"
                      << infoLog << std::endl;
        }
    }
}
void Shader::setInt(const std::string &name, int v0) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), v0);
}
void Shader::setInt(const std::string &name, int v0, int v1) const
{
    glUniform2i(glGetUniformLocation(ID, name.c_str()), v0, v1);
}
void Shader::setInt(const std::string &name, int v0, int v1, int v2) const
{
    glUniform3i(glGetUniformLocation(ID, name.c_str()), v0, v1, v2);
}
void Shader::setInt(const std::string &name, int v0, int v1, int v2, int v3) const
{
    glUniform4i(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

void Shader::setFloat(const std::string &name, float v0) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), v0);
}
void Shader::setFloat(const std::string &name, float v0, float v1) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), v0, v1);
}
void Shader::setFloat(const std::string &name, float v0, float v1, float v2) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2);
}
void Shader::setFloat(const std::string &name, float v0, float v1, float v2, float v3) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

void Shader::setMatrix(const std::string &name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), count, transpose, value);
}