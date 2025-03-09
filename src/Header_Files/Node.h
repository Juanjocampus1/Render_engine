#ifndef NODE_H
#define NODE_H

#include "Mesh.h"
#include <memory>

class Node {
public:
    Node* parent;
    Node* leftChild;
    Node* rightChild;
    Mesh* mesh;
    glm::mat4 transform;
    int id;

    Node(Mesh* mesh, const glm::mat4& transform = glm::mat4(1.0f));
    ~Node();

    void AddChild(Node* child);
    void RemoveChild(Node* child);
    void RemoveNode(Node* nodeDelete);
	Node* MinNode(Node*);
    void ReplaceNode(Node* node, Node* nodeReplace);
	void DestroyNode(Node* nodeD);
    void UpdateTransform(const glm::mat4& parentTransform = glm::mat4(1.0f));
    void Draw(Shader& shader, Camera& camera);
};

#endif // NODE_H