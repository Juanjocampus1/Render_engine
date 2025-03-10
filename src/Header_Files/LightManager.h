#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include "Mesh.h"
#include "Node.h"
#include "Scene.h"
#include "ShaderCLass.h"

enum LightType {
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    SPOT_LIGHT
};

struct Light {
    glm::vec3 position;
    glm::vec4 color;
    glm::mat4 model;
    LightType type;
    int id;
    Mesh* mesh;
    Node* node; // Añadir un nodo para cada luz
};

class LightManager {
public:
    LightManager(Scene& scene, Shader& lightShader);

    void AddLight(const glm::vec3& position, const glm::vec4& color, LightType type);
    void RemoveLight(int id);
    void UpdateLight(int id, const glm::vec3& position, const glm::vec4& color, LightType type);
    void UpdateLightPosition(int id, const glm::vec3& position);
    void DrawLights(const Camera& camera);
    Light* GetLight(int id);
    std::vector<Light>& GetLights();

private:
    Scene& scene;
    Shader& lightShader;
    std::vector<Light> lights;
    int nextId;
};

#endif // LIGHTMANAGER_H
