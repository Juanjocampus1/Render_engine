#include "../Header_Files/MeshManager.h"

MeshManager::MeshManager(Scene& scene) : scene(scene) {}

void MeshManager::AddCube(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) {
    std::vector<Vertex> vertices = {
        Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f,  1.0f), glm::vec2(0.0f, 1.0f)}
    };

    std::vector<GLuint> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1
    };

    std::vector<Texture> textures; // No necesitamos texturas para los cubos

    Mesh* cubeMesh = new Mesh(vertices, indices, textures);
    Node* cubeNode = new Node(cubeMesh);
    cubeNode->transform = glm::translate(glm::mat4(1.0f), position) *
        glm::mat4_cast(rotation) *
        glm::scale(glm::mat4(1.0f), scale);
    scene.AddNode(cubeNode);
    meshes.push_back(cubeNode);
}

void MeshManager::RemoveMesh(int id) {
    auto it = std::remove_if(meshes.begin(), meshes.end(), [id](Node* mesh) {
        return mesh->id == id;
        });
    if (it != meshes.end()) {
        scene.RemoveNode(*it);
        delete* it; // Eliminar la instancia de Node
        meshes.erase(it, meshes.end());
    }
}

std::vector<Node*> MeshManager::GetMeshes() {
    return meshes;
}