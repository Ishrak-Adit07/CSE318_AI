#include "Car.hpp"

class Node
{
    string node_attribute;
    map<string, Node *> children;

    string classification;
    bool is_leaf;

public:
    Node(bool init_is_leaf = false, string init_class = "") : is_leaf(init_is_leaf), classification(init_class) {}
    ~Node()
    {
        if (is_leaf)
            return;
        for (auto child : children)
            if (child.second != nullptr)
                delete child.second;
    }

    void setNodeAttribute(string node_attr) { node_attribute = node_attr; }
    string getNodeAttribute() { return node_attribute; }

    void setIsLeaf(bool Is_leaf) { is_leaf = Is_leaf; }
    bool getIsLeaf() { return is_leaf; }

    void setClassification(string Classification) { classification = Classification; }
    string getClassification() { return classification; }

    void setChild(map<string, Node *> Children) { children = Children; }
};

class Tree
{
    Node *root;
    Tree(Node *root) : root(root) {}
    ~Tree()
    {
        if (root)
            delete root;
    }

public:
    string getClassification(Car car)
    {
        Node *node = root;
        while (!node->is_leaf)
        {
            string attr_value = car.getCarAttributes()[node->attribute_to_check];
            node = node->children[attr_value];
        }
        return node->classification;
    }

    void print_tree(Node *node, int depth)
    {
        if (node->is_leaf)
        {
            for (int i = 0; i < depth; i++)
                cout << "\t";
            cout << node->classification << endl;
            return;
        }
        for (auto they : node->children)
        {
            cout << endl;
            for (int i = 0; i < depth; i++)
                cout << "\t";
            cout << node->attribute_to_check << " = " << they.first << " : ";
            print_tree(they.second, depth + 1);
        }
    }
};