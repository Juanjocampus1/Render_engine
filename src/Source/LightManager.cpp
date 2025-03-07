#include "../Header_Files/LightManager.h"

LightManager::LightManager() : nextId(0) {}

void LightManager::AddLight(const glm::vec3& position, const glm::vec4& color, LightType type) {
    Light light;
    light.position = position;
    light.color = color;
    light.model = glm::translate(glm::mat4(1.0f), position);
    light.type = type;
    light.id = nextId++;

    // Crear una instancia de Mesh para la luz
    std::vector<Vertex> lightVertices = {
        Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
    };

    std::vector<GLuint> lightIndices = {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };

    std::vector<Texture> lightTextures; // No necesitamos texturas para las luces

    light.mesh = new Mesh(lightVertices, lightIndices, lightTextures);

    lights.push_back(light);
}

void LightManager::RemoveLight(int id) {
    auto it = std::remove_if(lights.begin(), lights.end(), [id](const Light& light) {
        return light.id == id;
        });
    if (it != lights.end()) {
        delete it->mesh; // Eliminar la instancia de Mesh
        lights.erase(it, lights.end());
    }
}

void LightManager::UpdateLight(int id, const glm::vec3& position, const glm::vec4& color, LightType type) {
    for (auto& light : lights) {
        if (light.id == id) {
            light.position = position;
            light.color = color;
            light.model = glm::translate(glm::mat4(1.0f), position);
            light.type = type;
            break;
        }
    }
}

void LightManager::UpdateLightPosition(int id, const glm::vec3& position) {
    for (auto& light : lights) {
        if (light.id == id) {
            light.position = position;
            light.model = glm::translate(glm::mat4(1.0f), position);
            break;
        }
    }
}

Light* LightManager::GetLight(int id) {
    for (auto& light : lights) {
        if (light.id == id) {
            return &light;
        }
    }
    return nullptr;
}

std::vector<Light>& LightManager::GetLights() {
    return lights;
}