#ifndef MESHMANAGER_H
#define MESHMANAGER_H

#include "Mesh.h"
#include "Node.h"
#include "Scene.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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
        void AddPlane(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
        void AddCylinder(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
        void AddPyramid(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
        void AddSphere(const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
        void RemoveMesh(int id);
        void UpdateMesh(int id, const glm::vec3& position, const glm::vec3& scale, const glm::quat& rotation);
        std::vector<MeshData>& GetMeshes();
        MeshData* GetMesh(int id);

    private:
        Scene& scene;
        std::vector<MeshData> meshes;
};

#endif // MESHMANAGER_H

