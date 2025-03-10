#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "LightManager.h"
#include "MeshManager.h"
#include "Scene.h"
#include "Camera.h"
#include "imgui.h"

class UIManager {
public:
    UIManager(LightManager& lightManager, MeshManager& meshManager, Scene& scene);
    void Render(Camera& camera);
    int selectedObjectId;
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



