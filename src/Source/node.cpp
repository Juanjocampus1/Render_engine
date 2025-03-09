#include "../Header_Files/Node.h"

static int nextNodeId = 0;

Node::Node(Mesh* mesh, const glm::mat4& transform) : parent(nullptr), leftChild(nullptr), rightChild(nullptr), mesh(mesh), transform(transform), id(nextNodeId++) {}

Node::~Node() {
    delete leftChild;
    delete rightChild;
}

void Node::AddChild(Node* child) {
    if (!leftChild) {
        leftChild = child;
    }
    else if (!rightChild) {
        rightChild = child;
    }
    else {
        // Si ambos hijos están ocupados, añadir el hijo al subárbol izquierdo
        leftChild->AddChild(child);
    }
    child->parent = this;
}

void Node::RemoveChild(Node* child) {
    if (leftChild == child) {
        delete leftChild;
        leftChild = nullptr;
    }
    else if (rightChild == child) {
        delete rightChild;
        rightChild = nullptr;
    }
    else {
        if (leftChild) leftChild->RemoveChild(child);
        if (rightChild) rightChild->RemoveChild(child);
    }
}

bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale) {
    // Extract translation
    translation = glm::vec3(transform[3]);

    // Extract scale
    glm::mat3 rotScaleMat(transform);
    scale.x = glm::length(rotScaleMat[0]);
    scale.y = glm::length(rotScaleMat[1]);
    scale.z = glm::length(rotScaleMat[2]);

    // Remove scale from the matrix
    if (scale.x) rotScaleMat[0] /= scale.x;
    if (scale.y) rotScaleMat[1] /= scale.y;
    if (scale.z) rotScaleMat[2] /= scale.z;

    // Extract rotation
    rotation = glm::quat_cast(rotScaleMat);

    return true;
}

void Node::UpdateTransform(const glm::mat4& parentTransform) {
    glm::mat4 globalTransform = parentTransform * transform;

    // Descomponer globalTransform en translation, rotation y scale
    glm::vec3 translation, scale;
    glm::quat rotation;
    DecomposeTransform(globalTransform, translation, rotation, scale);

    if (mesh) {
        mesh->SetTransform(translation, rotation, scale);
    }
    if (leftChild) leftChild->UpdateTransform(globalTransform);
    if (rightChild) rightChild->UpdateTransform(globalTransform);
}

void Node::Draw(Shader& shader, Camera& camera) {
    if (mesh) {
        mesh->Draw(shader, camera);
    }
    if (leftChild) leftChild->Draw(shader, camera);
    if (rightChild) rightChild->Draw(shader, camera);
}



