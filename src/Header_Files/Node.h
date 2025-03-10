#ifndef NODE_H
#define NODE_H

#include "Mesh.h"

class Node;

struct NodeData{
    Mesh* mesh;
    Shader* shader;
    glm::mat4 transform;
    int id;

    NodeData(Mesh* mesh, Shader* shader, const glm::mat4& transform, int id) : 
        mesh(mesh), shader(shader), transform(transform), id(id) {}
};

struct NodeInfo {
    NodeData data;
    Node* Parent;
	Node* Child;
    std::vector<Node*> children;
    int level;

	NodeInfo(Node* parent, Node* child, const std::vector<Node*>& children, int level, NodeData data) : 
        Parent(parent), Child(child), children(children), level(level), data(data) {}
};

class Node {
    public:
        NodeInfo nodeInfo;

        Node(Mesh* mesh, Shader* shader, const glm::mat4& transform = glm::mat4(1.0f), int level = 0);
        ~Node();

        static Node* CreateNode(Mesh* mesh, Shader* shader, const glm::mat4& transform = glm::mat4(1.0f), int level = 0);
        void InsertNode(Node* parent, Node* child);
        void RemoveChild(Node* child);
        void RemoveNode(Node* nodeDelete);
        void DestroyNode(Node* nodeD);
        void UpdateTransform(const glm::mat4& parentTransform = glm::mat4(1.0f));
        void Draw(const Camera& camera);
};

#endif // NODE_H




