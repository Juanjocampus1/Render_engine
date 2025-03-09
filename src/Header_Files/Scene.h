#ifndef SCENE_H
#define SCENE_H

#include "Node.h"

class Scene {
public:
    Node* root;

    Scene();
    ~Scene();

    void AddNode(Node* node);
    void RemoveNode(Node* node);
    void Update();
    void Draw(Shader& shader, Camera& camera);
};

#endif // SCENE_H



