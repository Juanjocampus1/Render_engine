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

void MeshManager::AddPlane(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) {
    std::vector<Vertex> vertices = {
        Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
    };

    std::vector<GLuint> indices = {
        0, 1, 2, 2, 3, 0
    };

    std::vector<Texture> textures; // No necesitamos texturas para los planos

    Mesh* planeMesh = new Mesh(vertices, indices, textures);
    Shader* planeShader = new Shader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    Node* planeNode = new Node(planeMesh, planeShader);
    planeNode->nodeInfo.data.transform = glm::translate(glm::mat4(1.0f), position) *
        glm::mat4_cast(rotation) *
        glm::scale(glm::mat4(1.0f), scale);
    scene.AddNode(planeNode);

    MeshData meshData;
    meshData.id = planeNode->nodeInfo.data.id; // Usar el ID del nodo
    meshData.position = position;
    meshData.scale = scale;
    meshData.rotation = rotation;
    meshData.mesh = planeMesh;
    meshData.node = planeNode;

    meshes.push_back(meshData);
}

void MeshManager::AddCylinder(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) {
    const int segments = 36;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    // Generate vertices for the top and bottom circles
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * glm::pi<float>() * float(i) / float(segments);
        float x = cosf(theta);
        float z = sinf(theta);

        vertices.push_back(Vertex{ glm::vec3(x, 1.0f, z), glm::vec3(x, 0.0f, z), glm::vec3(x, 0.0f, z), glm::vec2(float(i) / float(segments), 1.0f) });
        vertices.push_back(Vertex{ glm::vec3(x, -1.0f, z), glm::vec3(x, 0.0f, z), glm::vec3(x, 0.0f, z), glm::vec2(float(i) / float(segments), 0.0f) });
    }

    // Generate indices for the top and bottom circles
    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;
        indices.push_back(i * 2);
        indices.push_back(next * 2);
        indices.push_back(i * 2 + 1);

        indices.push_back(next * 2);
        indices.push_back(next * 2 + 1);
        indices.push_back(i * 2 + 1);
    }

    std::vector<Texture> textures; // No necesitamos texturas para los cilindros

    Mesh* cylinderMesh = new Mesh(vertices, indices, textures);
    Shader* cylinderShader = new Shader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    Node* cylinderNode = new Node(cylinderMesh, cylinderShader);
    cylinderNode->nodeInfo.data.transform = glm::translate(glm::mat4(1.0f), position) *
        glm::mat4_cast(rotation) *
        glm::scale(glm::mat4(1.0f), scale);
    scene.AddNode(cylinderNode);

    MeshData meshData;
    meshData.id = cylinderNode->nodeInfo.data.id; // Usar el ID del nodo
    meshData.position = position;
    meshData.scale = scale;
    meshData.rotation = rotation;
    meshData.mesh = cylinderMesh;
    meshData.node = cylinderNode;

    meshes.push_back(meshData);
}

void MeshManager::AddPyramid(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) {
    std::vector<Vertex> vertices = {
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f)},
        Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        Vertex{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
    };

    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        1, 2, 3,
        1, 3, 4
    };

    std::vector<Texture> textures; // No necesitamos texturas para las pirámides

    Mesh* pyramidMesh = new Mesh(vertices, indices, textures);
    Shader* pyramidShader = new Shader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    Node* pyramidNode = new Node(pyramidMesh, pyramidShader);
    pyramidNode->nodeInfo.data.transform = glm::translate(glm::mat4(1.0f), position) *
        glm::mat4_cast(rotation) *
        glm::scale(glm::mat4(1.0f), scale);
    scene.AddNode(pyramidNode);

    MeshData meshData;
    meshData.id = pyramidNode->nodeInfo.data.id; // Usar el ID del nodo
    meshData.position = position;
    meshData.scale = scale;
    meshData.rotation = rotation;
    meshData.mesh = pyramidMesh;
    meshData.node = pyramidNode;

    meshes.push_back(meshData);
}

void MeshManager::AddSphere(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation) {
    const int segments = 36;
    const int rings = 18;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (int i = 0; i <= rings; ++i) {
        float theta = i * glm::pi<float>() / rings;
        float sinTheta = sinf(theta);
        float cosTheta = cosf(theta);

        for (int j = 0; j <= segments; ++j) {
            float phi = j * 2.0f * glm::pi<float>() / segments;
            float sinPhi = sinf(phi);
            float cosPhi = cosf(phi);

            glm::vec3 position = glm::vec3(cosPhi * sinTheta, cosTheta, sinPhi * sinTheta);
            glm::vec3 normal = glm::normalize(position);
            glm::vec2 texCoord = glm::vec2(float(j) / segments, float(i) / rings);

            vertices.push_back(Vertex{ position, normal, normal, texCoord });
        }
    }

    for (int i = 0; i < rings; ++i) {
        for (int j = 0; j < segments; ++j) {
            int first = (i * (segments + 1)) + j;
            int second = first + segments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    std::vector<Texture> textures; // No necesitamos texturas para las esferas

    Mesh* sphereMesh = new Mesh(vertices, indices, textures);
    Shader* sphereShader = new Shader("Shaders/VertexShader.glsl", "Shaders/FragmentShader.glsl");
    Node* sphereNode = new Node(sphereMesh, sphereShader);
    sphereNode->nodeInfo.data.transform = glm::translate(glm::mat4(1.0f), position) *
        glm::mat4_cast(rotation) *
        glm::scale(glm::mat4(1.0f), scale);
    scene.AddNode(sphereNode);

    MeshData meshData;
    meshData.id = sphereNode->nodeInfo.data.id; // Usar el ID del nodo
    meshData.position = position;
    meshData.scale = scale;
    meshData.rotation = rotation;
    meshData.mesh = sphereMesh;
    meshData.node = sphereNode;

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
