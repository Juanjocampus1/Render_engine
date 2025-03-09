#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Mesh.h"
#include "Node.h"
#include "Scene.h"

struct MeshData {
    int id;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    Mesh* mesh;
    Node* node;
};

class MeshManager {
public:
    MeshManager(Scene& scene);
    void AddCube(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
    void RemoveMesh(int id);
    void UpdateMesh(int id, const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
    std::vector<MeshData>& GetMeshes();
    MeshData* GetMesh(int id);

private:
    Scene& scene;
    std::vector<MeshData> meshes;
    int nextId;
};

#endif // MESHMANAGER_H