#include "../Header_Files/Node.h"
#include <memory>
#include <algorithm>

static int nextNodeId = 0;

bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale);

Node::Node(Mesh* mesh, Shader* shader, const glm::mat4& transform, int level)
    : nodeInfo(nullptr, nullptr, {}, level, NodeData(mesh, shader, transform, nextNodeId++)) {
}

Node::~Node() {
    for (Node* child : nodeInfo.children) {
        delete child;
    }
    nodeInfo.children.clear();
}

Node* Node::CreateNode(Mesh* mesh, Shader* shader, const glm::mat4& transform, int level) {
    return new Node(mesh, shader, transform, level);
}

void Node::InsertNode(Node* parent, Node* child) {
    if (parent) {
        parent->nodeInfo.children.push_back(child);
        child->nodeInfo.Parent = parent;
        child->nodeInfo.level = parent->nodeInfo.level + 1;
    }
}

void Node::RemoveChild(Node* child) {
    auto it = std::find(nodeInfo.children.begin(), nodeInfo.children.end(), child);
    if (it != nodeInfo.children.end()) {
        (*it)->nodeInfo.Parent = nullptr;
        nodeInfo.children.erase(it);
    }
}

void Node::RemoveNode(Node* nodeDelete) {
    if (nodeDelete->nodeInfo.Parent) {
        nodeDelete->nodeInfo.Parent->RemoveChild(nodeDelete);
    }
    DestroyNode(nodeDelete);
}

void Node::DestroyNode(Node* nodeD) {
    for (Node* child : nodeD->nodeInfo.children) {
        DestroyNode(child);
    }
    nodeD->nodeInfo.children.clear();
    delete nodeD;
}

void Node::UpdateTransform(const glm::mat4& parentTransform) {
    glm::mat4 globalTransform = parentTransform * nodeInfo.data.transform;

    glm::vec3 translation, scale;
    glm::quat rotation;
    DecomposeTransform(globalTransform, translation, rotation, scale);

    if (nodeInfo.data.mesh) {
        nodeInfo.data.mesh->SetTransform(translation, rotation, scale);
    }
    for (Node* child : nodeInfo.children) {
        child->UpdateTransform(globalTransform);
    }
}

void Node::Draw(const Camera& camera) {
    if (nodeInfo.data.mesh && nodeInfo.data.shader) {
        nodeInfo.data.shader->Activate();
        nodeInfo.data.mesh->Draw(*nodeInfo.data.shader, camera);
    }
    for (Node* child : nodeInfo.children) {
        child->Draw(camera);
    }
}

bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation, glm::vec3& scale) {
    translation = glm::vec3(transform[3]);

    glm::mat3 rotScaleMat(transform);
    scale.x = glm::length(rotScaleMat[0]);
    scale.y = glm::length(rotScaleMat[1]);
    scale.z = glm::length(rotScaleMat[2]);

    if (scale.x) rotScaleMat[0] /= scale.x;
    if (scale.y) rotScaleMat[1] /= scale.y;
    if (scale.z) rotScaleMat[2] /= scale.z;

    rotation = glm::quat_cast(rotScaleMat);

    return true;
}
