#include "../Header_Files/UIManager.h"

UIManager::UIManager(LightManager& lightManager, MeshManager& meshManager, Scene& scene)
    : lightManager(lightManager), meshManager(meshManager), scene(scene), selectedObjectId(-1) {
}

void UIManager::Render(Camera& camera) {
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
    RenderDeleteMenu();

    // Draw lights
    lightManager.DrawLights(camera);

    
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
        if (ImGui::Selectable(("Mesh " + std::to_string(mesh.id)).c_str(), selectedObjectId == mesh.id)) {
            selectedObjectId = mesh.id; // Actualizar el ID del objeto seleccionado
        }
        ImGui::PopID();
    }
}

void UIManager::RenderProperties() {
    ImGui::Text("Transform");
    static glm::vec3 position(0.0f);
    static glm::vec3 scale(1.0f);
    static glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);

    if (selectedObjectId != -1) {
        Light* selectedLight = lightManager.GetLight(selectedObjectId);
        if (selectedLight) {
            position = selectedLight->position;
            scale = glm::vec3(1.0f); // Las luces no tienen escala
            rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Las luces no tienen rotación
        }
        else {
            MeshData* selectedMesh = meshManager.GetMesh(selectedObjectId);
            if (selectedMesh) {
                position = selectedMesh->position;
                scale = selectedMesh->scale;
                rotation = selectedMesh->rotation;
            }
        }
    }

    if (ImGui::SliderFloat3("Translation", glm::value_ptr(position), -10.0f, 10.0f)) {
        if (selectedObjectId != -1) {
            Light* selectedLight = lightManager.GetLight(selectedObjectId);
            if (selectedLight) {
                lightManager.UpdateLightPosition(selectedObjectId, position);
            }
            else {
                MeshData* selectedMesh = meshManager.GetMesh(selectedObjectId);
                if (selectedMesh) {
                    meshManager.UpdateMesh(selectedObjectId, position, scale, rotation);
                }
            }
        }
    }

    glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(rotation));
    if (ImGui::SliderFloat3("Rotation", glm::value_ptr(rotationEuler), -180.0f, 180.0f)) {
        rotation = glm::quat(glm::radians(rotationEuler));
        if (selectedObjectId != -1) {
            MeshData* selectedMesh = meshManager.GetMesh(selectedObjectId);
            if (selectedMesh) {
                meshManager.UpdateMesh(selectedObjectId, position, scale, rotation);
            }
        }
    }

    if (ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.1f, 10.0f)) {
        if (selectedObjectId != -1) {
            MeshData* selectedMesh = meshManager.GetMesh(selectedObjectId);
            if (selectedMesh) {
                meshManager.UpdateMesh(selectedObjectId, position, scale, rotation);
            }
        }
    }

    ImGui::Separator();
    ImGui::Text("Light Properties");
    static glm::vec4 lightColor(1.0f);
    static int lightType = 0;

    if (selectedObjectId != -1) {
        Light* selectedLight = lightManager.GetLight(selectedObjectId);
        if (selectedLight) {
            lightColor = selectedLight->color;
            lightType = static_cast<int>(selectedLight->type);
        }
    }

    if (ImGui::ColorEdit4("Light Color", glm::value_ptr(lightColor))) {
        if (selectedObjectId != -1) {
            Light* selectedLight = lightManager.GetLight(selectedObjectId);
            if (selectedLight) {
                lightManager.UpdateLight(selectedObjectId, selectedLight->position, lightColor, selectedLight->type);
            }
        }
    }

    const char* lightTypes[] = { "Point Light", "Directional Light", "Spot Light" };
    if (ImGui::Combo("Light Type", &lightType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
        if (selectedObjectId != -1) {
            Light* selectedLight = lightManager.GetLight(selectedObjectId);
            if (selectedLight) {
                selectedLight->type = static_cast<LightType>(lightType);
                lightManager.UpdateLight(selectedObjectId, selectedLight->position, selectedLight->color, selectedLight->type);
            }
        }
    }
}

void UIManager::RenderDeleteMenu() {
    // Handle delete popup
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_X)) && selectedObjectId != -1) {
        ImGui::OpenPopup("Delete Object");
    }

    if (ImGui::BeginPopupModal("Delete Object", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to delete the selected object?\nThis operation cannot be undone!\n\n");
        ImGui::Separator();

        if (ImGui::Button("Delete", ImVec2(120, 0))) {
            Light* selectedLight = lightManager.GetLight(selectedObjectId);
            if (selectedLight) {
                lightManager.RemoveLight(selectedObjectId);
            }
            else {
                MeshData* selectedMesh = meshManager.GetMesh(selectedObjectId);
                if (selectedMesh) {
                    meshManager.RemoveMesh(selectedObjectId);
                }
            }
            selectedObjectId = -1; // Deseleccionar el objeto
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
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
            if (ImGui::MenuItem("Add Plane")) {
                meshManager.AddPlane(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
            }
            if (ImGui::MenuItem("Add Cylinder")) {
                meshManager.AddCylinder(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
            }
            if (ImGui::MenuItem("Add Pyramid")) {
                meshManager.AddPyramid(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
            }
            if (ImGui::MenuItem("Add Sphere")) {
                meshManager.AddSphere(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}