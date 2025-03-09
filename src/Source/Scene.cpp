#include "../Header_Files/Scene.h"

Scene::Scene() : root(nullptr) {}

Scene::~Scene() {
    delete root;
}

void Scene::AddNode(Node* node) {
    if (!root) {
        root = node;
    }
    else {
        root->AddChild(node);
    }
}

void Scene::RemoveNode(Node* node) {
    if (root == node) {
        delete root;
        root = nullptr;
    }
    else {
        root->RemoveChild(node);
    }
}

void Scene::Update() {
    if (root) {
        root->UpdateTransform();
    }
}

void Scene::Draw(Shader& shader, Camera& camera) {
    if (root) {
        root->Draw(shader, camera);
    }
}