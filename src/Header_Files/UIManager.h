#ifndef UIMANAGER_H
#define UIMANAGER_H

#include "imgui.h"
#include "LightManager.h"

class UIManager {
public:
    UIManager(LightManager& lightManager);
    void Render();
    int selectedObjectId; // Variable para almacenar el ID del objeto seleccionado

private:
    LightManager& lightManager;
    void RenderHierarchy();
    void RenderProperties();
};

#endif // UIMANAGER_H


