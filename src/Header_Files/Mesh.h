#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh {
public:
    // Mesh Data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VAO VAO;
    glm::mat4 modelMatrix;

    Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);
    void Draw(Shader& shader, const Camera& camera);
    void SetTransform(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale);
};

#endif
