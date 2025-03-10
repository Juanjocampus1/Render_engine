#ifndef NODE_H
#define NODE_H

#include "Mesh.h"
#include "ShaderClass.h"
#include "Camera.h"

class Node {
public:
    Node* parent;
    Node* leftChild;
    Node* rightChild;
    Mesh* mesh;
    Shader* shader; // Añadir una referencia al shader
    glm::mat4 transform;
    int id;

    Node(Mesh* mesh, Shader* shader, const glm::mat4& transform = glm::mat4(1.0f));
    ~Node();

    void AddChild(Node* child);
    void RemoveChild(Node* child);
    void RemoveNode(Node* nodeDelete);
	Node* MinNode(Node*);
    void ReplaceNode(Node* node, Node* nodeReplace);
    void DestroyNode(Node* nodeD);
    void UpdateTransform(const glm::mat4& parentTransform = glm::mat4(1.0f));
    void Draw(const Camera& camera);
};

#endif // NODE_H



