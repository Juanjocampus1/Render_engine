#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::string get_file_contents(const char* filename);

class Shader {
public:
    GLuint ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void Activate();
    void Delete();
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
private:
    void CompileErrors(unsigned int shader, const char* type);
};

#endif // SHADERCLASS_H
