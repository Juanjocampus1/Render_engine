#include "../Header_Files/UIManager.h"

UIManager::UIManager(LightManager& lightManager, MeshManager& meshManager, Scene& scene) : lightManager(lightManager), meshManager(meshManager), scene(scene), selectedObjectId(-1) {}

void UIManager::Render() {
    // Render the menu bar
    RenderMenuBar();

    // Render the hierarchy panel
    ImGui::Begin("Hierarchy");
    RenderHierarchy();
    ImGui::End();

    // Render the properties panel
    ImGui::Begin("Properties");
    RenderProperties();
    ImGui::End();

    // Render the add menu
    RenderAddMenu();
}

void UIManager::RenderMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene")) {
                // Lógica para crear una nueva escena
            }
            if (ImGui::MenuItem("Open Scene")) {
                // Lógica para abrir una escena existente
            }
            if (ImGui::MenuItem("Save Scene")) {
                // Lógica para guardar la escena actual
            }
            ImGui::EndMenu();
        }
        ImGui::Text("Current Scene: Default Scene");
        ImGui::EndMainMenuBar();
    }
}

void UIManager::RenderHierarchy() {
    ImGui::Text("Objects in Scene");
    for (const auto& light : lightManager.GetLights()) {
        ImGui::PushID(light.id);
        if (ImGui::Selectable(("Light " + std::to_string(light.id)).c_str(), selectedObjectId == light.id)) {
            selectedObjectId = light.id; // Actualizar el ID del objeto seleccionado
        }
        ImGui::PopID();
    }
    for (const auto& mesh : meshManager.GetMeshes()) {
        ImGui::PushID(mesh.id);
        if (ImGui::Selectable(("Cube " + std::to_string(mesh.id)).c_str(), selectedObjectId == mesh.id)) {
            selectedObjectId = mesh.id; // Actualizar el ID del objeto seleccionado
        }
        ImGui::PopID();
    }
}

void UIManager::RenderProperties() {
    ImGui::Text("Properties");
    if (selectedObjectId != -1) {
        Light* selectedLight = lightManager.GetLight(selectedObjectId);
        if (selectedLight) {
            if (ImGui::ColorEdit4("Light Color", const_cast<float*>(glm::value_ptr(selectedLight->color)))) {
                lightManager.UpdateLight(selectedObjectId, selectedLight->position, selectedLight->color, selectedLight->type);
            }
            if (ImGui::SliderFloat3("Light Position", const_cast<float*>(glm::value_ptr(selectedLight->position)), -1.0f, 1.0f)) {
                lightManager.UpdateLightPosition(selectedObjectId, selectedLight->position);
            }
            const char* lightTypes[] = { "Point Light", "Directional Light", "Spot Light" };
            int currentType = static_cast<int>(selectedLight->type);
            if (ImGui::Combo("Light Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
                selectedLight->type = static_cast<LightType>(currentType);
                lightManager.UpdateLight(selectedObjectId, selectedLight->position, selectedLight->color, selectedLight->type);
            }
            if (ImGui::Button("Remove Light")) {
                lightManager.RemoveLight(selectedObjectId);
                selectedObjectId = -1; // Deseleccionar el objeto
            }
        }
        else {
            MeshData* selectedMesh = meshManager.GetMesh(selectedObjectId);
            if (selectedMesh) {
                if (ImGui::SliderFloat3("Translation", glm::value_ptr(selectedMesh->position), -10.0f, 10.0f)) {
                    meshManager.UpdateMesh(selectedObjectId, selectedMesh->position, selectedMesh->scale, selectedMesh->rotation);
                }
                glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(selectedMesh->rotation));
                if (ImGui::SliderFloat3("Rotation", glm::value_ptr(rotationEuler), -180.0f, 180.0f)) {
                    glm::quat rotQuat = glm::quat(glm::radians(rotationEuler));
                    meshManager.UpdateMesh(selectedObjectId, selectedMesh->position, selectedMesh->scale, rotQuat);
                }
                if (ImGui::SliderFloat3("Scale", glm::value_ptr(selectedMesh->scale), 0.1f, 10.0f)) {
                    meshManager.UpdateMesh(selectedObjectId, selectedMesh->position, selectedMesh->scale, selectedMesh->rotation);
                }
                if (ImGui::Button("Remove Mesh")) {
                    meshManager.RemoveMesh(selectedObjectId);
                    selectedObjectId = -1; // Deseleccionar el objeto
                }
            }
        }
    }
    else {
        ImGui::Text("Select an object to see its properties.");
    }
}

void UIManager::RenderAddMenu() {
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_F))) {
        ImGui::OpenPopup("New Mesh");
    }

    if (ImGui::BeginPopup("New Mesh")) {
        ImGui::Text("Add New Object");
        ImGui::Separator();
        if (ImGui::BeginMenu("Lights")) {
            if (ImGui::MenuItem("Add Light")) {
                lightManager.AddLight(glm::vec3(0.0f, 0.5f, 0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), POINT_LIGHT);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Meshes")) {
            if (ImGui::MenuItem("Add Cube")) {
                meshManager.AddCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

