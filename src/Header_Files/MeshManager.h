#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Mesh.h"
#include "Node.h"
#include "Scene.h"

class MeshManager {
public:
    MeshManager(Scene& scene);
    void AddCube(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
    void RemoveMesh(int id);
    std::vector<Node*> GetMeshes();

private:
    Scene& scene;
    std::vector<Node*> meshes;
};

#endif // MESHMANAGER_H
