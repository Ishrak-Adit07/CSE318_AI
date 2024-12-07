#include "Car.hpp"

class Node
{
    string attribute;
    map<string, Node *> children;
    string label;
    bool is_leaf = false;

public:
    Node(bool init_is_leaf = false, string init_class = "") : is_leaf(init_is_leaf), label(init_class) {}
    ~Node()
    {
        if (is_leaf)
            return;
        for (auto child : children)
            if (child.second != nullptr)
                delete child.second;
    }

    void setNodeAttribute(string node_attr) { attribute = node_attr; }
    string getNodeAttribute() { return attribute; }

    void setIsLeaf(bool Is_leaf) { is_leaf = Is_leaf; }
    bool getIsLeaf() { return is_leaf; }

    void setLabel(string Label) { label = Label; }
    string getLabel() { return label; }

    void setChildren(map<string, Node *> Children) { children = Children; }
    map<string, Node *> getChildren() { return children; }

    Node* getChild(string attr) { return children[attr]; }
    void setChild(string attr, Node * node) { children[attr] = node; }
};

class DecisionTree
{
    Node *root;

public:
    DecisionTree(Node *Root) : root(Root) {}
    ~DecisionTree()
    {
        if (root)
            delete root;
    }
    string getClassification(Car car)
    {
        Node *node = root;
        while (node && !(node->getIsLeaf()))
        {
            string attr_value = car.getCarAttribute(node->getNodeAttribute());
            node = node->getChild(attr_value);
        }
        if(node) return node->getLabel();
        else return "";
    }
};