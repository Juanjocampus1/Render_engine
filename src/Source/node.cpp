#include "../Header_Files/Node.h"
#include <memory>

static int nextNodeId = 0;

Node::Node(Mesh* mesh, const glm::mat4& transform)
    : parent(nullptr), leftChild(nullptr), rightChild(nullptr), mesh(mesh), transform(transform), id(nextNodeId++) {
}

Node::~Node() = default;

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
    if (child == nullptr) return;
    if (leftChild == child) {
        leftChild->parent = nullptr;
        leftChild = nullptr;
    }
    else if (rightChild == child) {
        rightChild->parent = nullptr;
        rightChild = nullptr;
    }
    else {
        if (leftChild) leftChild->RemoveNode(child);
        if (rightChild) rightChild->RemoveNode(child);
    }
}

// Determinar el nodo más izquierdo posible
Node* Node::MinNode(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->leftChild) {
        return MinNode(node->leftChild);
    }
    return node;
}

void Node::ReplaceNode(Node* node, Node* nodeReplace) {
	if (node->parent) {
		if (node->mesh == node->parent->leftChild->mesh) {
			node->parent->leftChild = nodeReplace;
        }
		else if (node->mesh == node->parent->rightChild->mesh) {
			node->parent->rightChild = nodeReplace;
		}
	}
	if (nodeReplace) {
		nodeReplace->parent = node->parent;
	}
}

void Node::DestroyNode(Node* nodeD) {

	nodeD->leftChild = nullptr;
	nodeD->rightChild = nullptr;

	delete nodeD;
}

void Node::RemoveNode(Node* nodeDelete) {
    if (nodeDelete->leftChild && nodeDelete->rightChild) {
		Node* min = MinNode(nodeDelete->rightChild);
		nodeDelete->mesh = min->mesh;
		nodeDelete->transform = min->transform;
		nodeDelete->id = min->id;
		RemoveNode(min);
    }
    else if (nodeDelete->leftChild) {
		ReplaceNode(nodeDelete, nodeDelete->leftChild);
		DestroyNode(nodeDelete);
    }
    else if (nodeDelete->rightChild) {
		ReplaceNode(nodeDelete, nodeDelete->rightChild);
		DestroyNode(nodeDelete);
    }
    else {
		ReplaceNode(nodeDelete, nullptr);
		DestroyNode(nodeDelete);
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