#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <cstring>
#include <iostream>
#include <stack>

#include "List.h"

class BinaryTree {

    class DuplicateException {
    public:
        DuplicateException () {
            std::cerr << "Такой элемент уже существует.";
        }
    };

    struct CoffeeVariety {
        std::string name;
        std::string origin;
        std::string roast;
        int caffeine{};
        int price{};
    };

    CoffeeVariety *dataArr = {};
    int arrLength = 0;
    int empty = 0;

public:
    class Node {
    public:
        Node *pLeftByName;
        Node *pRightByName;
        Node *pLeftByPrice;
        Node *pRightByPrice;
        std::string name;
        int price{};
        int index;

        explicit Node (const std::string &name, const int price, const int index, Node *pLeftByName = nullptr, Node *pRightByName = nullptr, Node *pLeftByPrice = nullptr, Node *pRightByPrice = nullptr) {
            this->name = name;
            this->price = price;
            this->pLeftByName = pLeftByName;
            this->pRightByName = pRightByName;
            this->pLeftByPrice = pLeftByPrice;
            this->pRightByPrice = pRightByPrice;
            this->index = index;

        }

        [[nodiscard]] std::string ToString (const CoffeeVariety &data, const char split = ' ') const {
            return this->name + split + data.origin + split + std::to_string(data.caffeine) + split + data.roast + split + std::to_string(this->price);
        }
    };

    Node *pRootByName;
    Node *pRootByPrice;

    BinaryTree();
    ~BinaryTree();

    void push (const std::string &name, const std::string &origin, const std::string &roast, const int caffeine, const int price) {
        if (this->FindByName(name) != nullptr || this->FindByPrice(price) != nullptr) throw DuplicateException();

        int j = -1;
        while (++j < arrLength && dataArr[j].price != -1)
            ;
        const auto pTemp = new Node(name, price, arrLength);

        if (j == arrLength) {
            auto *tempArr = new CoffeeVariety[arrLength+1];

            for (int i = 0; i < arrLength; i++) tempArr[i] = dataArr[i];

            tempArr[arrLength++] = {name, origin, roast, caffeine, price};
            dataArr = tempArr;
        }
        else {
            pTemp->index = j;
            dataArr[j] = {name, origin, roast, caffeine, price};
        }

        if (pRootByName == nullptr) {
            pRootByName = pTemp;
            pRootByPrice = pRootByName;

        } else {
            Node *pCurrentByName = pRootByName;
            Node *pCurrentByPrice = pRootByPrice;
            Node *pPrevByName = nullptr;
            Node *pPrevByPrice = nullptr;

            while (pCurrentByName != nullptr) {
                pPrevByName = pCurrentByName;
                if (strcmp(name.c_str(), pCurrentByName->name.c_str()) < 0) {
                    pCurrentByName = pCurrentByName->pLeftByName;
                } else {
                    pCurrentByName = pCurrentByName->pRightByName;
                }
            }

            while (pCurrentByPrice != nullptr) {
                pPrevByPrice = pCurrentByPrice;
                if (price < pCurrentByPrice->price) {
                    pCurrentByPrice = pCurrentByPrice->pLeftByPrice;
                } else {
                    pCurrentByPrice = pCurrentByPrice->pRightByPrice;
                }
            }

            if (strcmp(name.c_str(), pPrevByName->name.c_str()) < 0)
                pPrevByName->pLeftByName = pTemp;
            else
                pPrevByName->pRightByName = pTemp;

            if (price < pPrevByPrice->price)
                pPrevByPrice->pLeftByPrice = pTemp;
            else
                pPrevByPrice->pRightByPrice = pTemp;
        }
    }

private:
    static Node* ToTree(Node** nodes, int start, int end, bool byName);
    static void ToArray(Node* node, Node** nodes, int& index, bool byName);
public:
    void balanceTree(bool byName = true);
    [[nodiscard]] Node *FindByName(const std::string &name) const;
    [[nodiscard]] std::string FindByName(const std::string &name, char split, bool recursive) const;
    [[nodiscard]] Node *FindByPrice(int price) const;
    [[nodiscard]] std::string FindByPrice (int price, char split, bool recursive) const;
    [[nodiscard]] std::string Show (bool byName = true) const;
    static void Show(const Node *node, std::string &output, CoffeeVariety *dataArr, bool byName = true, int depth = 0);
    [[nodiscard]] std::string ToString (bool byName = true, bool reverse = false) const;
    static void ToString (const Node *node, std::string &output, CoffeeVariety *dataArr, bool byName = true, bool reverse = false);
    bool Remove (const std::string &name);
    bool Remove (int price);
    void Remove (Node *parent, const Node *target, bool byName);

    static Node* FindByNameRecursive(Node* node, const std::string &name);

    static Node* FindByPriceRecursive(Node* node, int price);
};

inline BinaryTree::BinaryTree() {
    pRootByName = nullptr;
    pRootByPrice = nullptr;
    arrLength = 0;
}

inline BinaryTree::~BinaryTree() {
    if (pRootByName == nullptr) return;

    std::stack<Node*> nodeStack;
    nodeStack.push(pRootByName);

    while (!nodeStack.empty()) {
        const Node* current = nodeStack.top();
        nodeStack.pop();

        if (current->pLeftByName) nodeStack.push(current->pLeftByName);
        if (current->pRightByName) nodeStack.push(current->pRightByName);

        delete current;
    }

    pRootByName = nullptr;
    delete [] dataArr;
}

inline BinaryTree::Node *BinaryTree::FindByName(const std::string &name) const {
    Node *pCurrent = pRootByName;

    if (!pCurrent) return nullptr;

    while (strcmp(name.c_str(), pCurrent->name.c_str()) != 0) {
        if (strcmp(name.c_str(), pCurrent->name.c_str()) < 0) {
            if (pCurrent->pLeftByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pLeftByName;
        }
        else if (strcmp(name.c_str(), pCurrent->name.c_str()) > 0) {
            if (pCurrent->pRightByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pRightByName;
        }
    }

    return pCurrent;
}

inline std::string BinaryTree::FindByName(const std::string &name, const char split, const bool recursive) const {
    const Node *temp = recursive ? FindByNameRecursive(pRootByName, name) : FindByName(name);
    return temp != nullptr ? temp->ToString(dataArr[temp->index], split) : " ";
}


inline BinaryTree::Node *BinaryTree::FindByPrice(const int price) const {
    Node *pCurrent = pRootByPrice;

    if (!pCurrent) return nullptr;

    while (price != pCurrent->price) {
        if (price < pCurrent->price) {
            if (pCurrent->pLeftByPrice == nullptr)
                return nullptr;
            pCurrent = pCurrent->pLeftByPrice;
        }
        else if (price > pCurrent->price) {
            if (pCurrent->pRightByPrice == nullptr)
                return nullptr;
            pCurrent = pCurrent->pRightByPrice;
        }
    }

    return pCurrent;
}

inline std::string BinaryTree::FindByPrice(const int price, const char split, const bool recursive) const {
    const Node *temp = recursive ? FindByPriceRecursive(pRootByPrice, price) : FindByPrice(price);
    return temp != nullptr ? temp->ToString(dataArr[temp->index], split) : " ";
}

inline BinaryTree::Node* BinaryTree::FindByNameRecursive(Node* node, const std::string &name) {
    if (!node || node->name == name) return node;
    if (name < node->name)
        return FindByNameRecursive(node->pLeftByName, name);
    return FindByNameRecursive(node->pRightByName, name);
}

inline BinaryTree::Node* BinaryTree::FindByPriceRecursive(Node* node, const int price) {
    if (!node || node->price == price) return node;
    if (price < node->price)
        return FindByPriceRecursive(node->pLeftByPrice, price);
    return FindByPriceRecursive(node->pRightByPrice, price);
}


inline void BinaryTree::Show(const Node *node, std::string &output, CoffeeVariety *dataArr, bool byName, int depth ) {
    if (!node) return;
    Show(byName ? node-> pRightByName : node->pRightByPrice, output, dataArr, byName, depth + 1);
    output += std::string(depth * 20, ' ') + '|' + node->ToString(dataArr[node->index]) + "\n";
    Show(byName ? node->pLeftByName : node->pLeftByPrice, output, dataArr, byName, depth + 1);
}

inline std::string BinaryTree::Show(const bool byName) const {
    std::string output;
    Show(byName ? pRootByName : pRootByPrice, output, dataArr, byName);
    return output;
}

inline void BinaryTree::balanceTree(bool byName) {
    int count = 0;
    ToArray(byName ? pRootByName : pRootByPrice, nullptr, count, byName);
    auto* nodes = new Node*[count];
    int index = 0;
    ToArray(byName ? pRootByName : pRootByPrice, nodes, index, byName);
    Node* newRoot = ToTree(nodes, 0, count - 1, byName);
    if (byName) {
        pRootByName = newRoot;
    } else {
        pRootByPrice = newRoot;
    }
    delete[] nodes;
}


inline BinaryTree::Node* BinaryTree::ToTree(Node** nodes, int start, int end, bool byName) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2;
    Node* node = nodes[mid];
    if (byName) {
        node->pLeftByName = ToTree(nodes, start, mid - 1, byName);
        node->pRightByName = ToTree(nodes, mid + 1, end, byName);
    } else {
        node->pLeftByPrice = ToTree(nodes, start, mid - 1, byName);
        node->pRightByPrice = ToTree(nodes, mid + 1, end, byName);
    }
    return node;
}

inline void BinaryTree::ToArray(Node* node, Node** nodes, int& index, bool byName) {
    if (!node) return;
    ToArray(byName ? node->pLeftByName : node->pLeftByPrice, nodes, index, byName);
    if (nodes) nodes[index] = node;
    index++;
    ToArray(byName ? node->pRightByName : node->pRightByPrice, nodes, index, byName);
}

inline std::string BinaryTree::ToString(const bool byName, const bool reverse) const {
    std::string output;
    ToString(byName ? pRootByName : pRootByPrice, output, dataArr, byName, reverse);
    return output;
}

inline void BinaryTree::ToString(const Node *node, std::string &output, CoffeeVariety *dataArr, const bool byName, const bool reverse) {
    if (!node) return;
    ToString(byName ? node->pLeftByName : node->pLeftByPrice, output, dataArr, byName, reverse);
    if (!reverse) output += node->ToString(dataArr[node->index]) + '\n';
    else output = node->ToString(dataArr[node->index]) + '\n' + output;
    ToString(byName ? node->pRightByName : node->pRightByPrice, output, dataArr, byName, reverse);
}

inline bool BinaryTree::Remove(const std::string &name) {
    Node *parentByName = nullptr;
    Node *parentByPrice = nullptr;
    Node *current = pRootByName;

    while (current != nullptr && current->name != name) {
        parentByName = current;
        if (name < current->name) current = current->pLeftByName;
        else  current = current->pRightByName;
    }

    if (current == nullptr) return false;

    parentByPrice = nullptr;
    Node *currentByPrice = pRootByPrice;
    while (currentByPrice != nullptr && currentByPrice != current) {
        parentByPrice = currentByPrice;
        if (current->price < currentByPrice->price)
            currentByPrice = currentByPrice->pLeftByPrice;
        else
            currentByPrice = currentByPrice->pRightByPrice;
    }

    Remove(parentByName, current, true);
    Remove(parentByPrice, current, false);

    delete current;

    return true;
}

inline bool BinaryTree::Remove(const int price) {
    Node *parentByPrice = nullptr;
    Node *parentByName = nullptr;
    Node *current = pRootByPrice;

    while (current != nullptr && current->price != price) {
        parentByPrice = current;
        if (price < current->price)
            current = current->pLeftByPrice;
        else
            current = current->pRightByPrice;
    }

    if (current == nullptr) return false;

    parentByName = nullptr;
    Node *currentByName = pRootByName;
    while (currentByName != nullptr && currentByName != current) {
        parentByName = currentByName;
        if (current->name < currentByName->name)
            currentByName = currentByName->pLeftByName;
        else
            currentByName = currentByName->pRightByName;
    }

    Remove(parentByName, current, true);
    Remove(parentByPrice, current, false);

    delete current;

    return true;
}

inline void BinaryTree::Remove(Node *parent, const Node *target, const bool byName) {
    Node *child = (byName ? target->pLeftByName : target->pLeftByPrice) ?
                  (byName ? target->pLeftByName : target->pLeftByPrice) :
                  (byName ? target->pRightByName : target->pRightByPrice);

    const int index = target->index;
    dataArr[index].name = "";
    dataArr[index].caffeine = -1;
    dataArr[index].origin = "";
    dataArr[index].price = -1;
    dataArr[index].roast = "";
    empty++;

    if (parent == nullptr) {
        if (byName)
            pRootByName = child;
        else
            pRootByPrice = child;
    } else {
        if (byName) {
            if (parent->pLeftByName == target)
                parent->pLeftByName = child;
            else
                parent->pRightByName = child;
        } else {
            if (parent->pLeftByPrice == target)
                parent->pLeftByPrice = child;
            else
                parent->pRightByPrice = child;
        }
    }
}
#endif
