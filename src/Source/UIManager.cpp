#include "../Header_Files/UIManager.h"
UIManager::UIManager(LightManager& lightManager) : lightManager(lightManager), selectedObjectId(-1) {}

void UIManager::Render() {
    // Render the hierarchy panel
    ImGui::Begin("Hierarchy");
    RenderHierarchy();
    ImGui::End();

    // Render the properties panel
    ImGui::Begin("Properties");
    RenderProperties();
    ImGui::End();
}

void UIManager::RenderHierarchy() {
    ImGui::Text("Objects in Scene");
    if (ImGui::Button("Add Light")) {
        lightManager.AddLight(glm::vec3(0.0f, 0.5f, 0.5f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), POINT_LIGHT);
    }
    for (const auto& light : lightManager.GetLights()) {
        ImGui::PushID(light.id);
        if (ImGui::Selectable(("Light " + std::to_string(light.id)).c_str(), selectedObjectId == light.id)) {
            selectedObjectId = light.id; // Actualizar el ID del objeto seleccionado
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
    }
    else {
        ImGui::Text("Select an object to see its properties.");
    }
}