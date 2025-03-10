#include "../Header_Files/MeshManager.h"

MeshManager::MeshManager(Scene& scene) : scene(scene), nextId(0) {}

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
    Shader* cubeShader = new Shader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    Node* cubeNode = new Node(cubeMesh, cubeShader);
    cubeNode->nodeInfo.data.transform = glm::translate(glm::mat4(1.0f), position) *
        glm::mat4_cast(rotation) *
        glm::scale(glm::mat4(1.0f), scale);
    scene.AddNode(cubeNode);

    MeshData meshData;
    meshData.id = cubeNode->nodeInfo.data.id; // Usar el ID del nodo
    meshData.position = position;
    meshData.scale = scale;
    meshData.rotation = rotation;
    meshData.mesh = cubeMesh;
    meshData.node = cubeNode;

    meshes.push_back(meshData);
}

void MeshManager::RemoveMesh(int id) {
    auto it = std::find_if(meshes.begin(), meshes.end(), [id](const MeshData& mesh) {
        return mesh.id == id;
        });
    if (it != meshes.end()) {
        if (it->node->nodeInfo.parent) {
            it->node->nodeInfo.parent->RemoveChild(it->node);
        }
        else {
            scene.RemoveNode(it->node);
        }
        meshes.erase(it);
    }
}

void MeshManager::UpdateMesh(int id, const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) {
    for (auto& mesh : meshes) {
        if (mesh.id == id) {
            mesh.position = position;
            mesh.scale = scale;
            mesh.rotation = rotation;
            mesh.node->nodeInfo.data.transform = glm::translate(glm::mat4(1.0f), position) *
                glm::mat4_cast(rotation) *
                glm::scale(glm::mat4(1.0f), scale);
            break;
        }
    }
}

std::vector<MeshData>& MeshManager::GetMeshes() {
    return meshes;
}

MeshData* MeshManager::GetMesh(int id) {
    for (auto& mesh : meshes) {
        if (mesh.id == id) {
            return &mesh;
        }
    }
    return nullptr;
}
