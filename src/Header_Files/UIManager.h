#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "imgui.h"
#include "LightManager.h"
#include "MeshManager.h"

class UIManager {
public:
    UIManager(LightManager& lightManager, MeshManager& meshManager, Scene& scene);
    void Render();
    int selectedObjectId; // Variable para almacenar el ID del objeto seleccionado

private:
    LightManager& lightManager;
    MeshManager& meshManager;
    Scene& scene;
    void RenderMenuBar();
    void RenderHierarchy();
    void RenderProperties();
    void RenderAddMenu();
};

#endif // UIMANAGER_H


