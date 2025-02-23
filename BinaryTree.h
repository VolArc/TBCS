#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <cstring>
#include <iostream>
#include <stack>


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

public:
    BinaryTree();
    ~BinaryTree();

    void push (const std::string &name, const std::string &origin, const std::string &roast, const int caffeine, const int price) {
        //if (this->FindByName(name) != nullptr)
        //    throw DuplicateException();
        //if (this->FindByPrice(price) != nullptr)
        //    throw DuplicateException();

        const auto pTemp = new Node(name, price, arrLength);
        auto *tempArr = new CoffeeVariety[arrLength+1];

        for (int i = 0; i < arrLength; i++) {
            tempArr[i] = dataArr[i];
        }

        tempArr[arrLength++] = {name, origin, roast, caffeine, price};
        dataArr = tempArr;

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

    [[nodiscard]] Node *FindByName(const std::string &name, int order = 0) const;
    [[nodiscard]] std::string FindByName(const std::string &name, char split, int order = 0) const;
    [[nodiscard]] Node *FindByPrice(int price, int order = 0) const;
    [[nodiscard]] std::string FindByPrice (int price, char split, int order = 0) const;
    [[nodiscard]] std::string Show (bool byName = true) const;
    static void Show(const Node *node, std::string &output, CoffeeVariety *dataArr, bool byName = true, int depth = 0);
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
}

inline BinaryTree::Node *BinaryTree::FindByName(const std::string &name, int order) const {
    Node *pCurrent = pRootByName;

    while (strcmp(name.c_str(), pCurrent->name.c_str()) != 0 || order > -1) {
        if (strcmp(name.c_str(), pCurrent->name.c_str()) < 0) {
            if (pCurrent->pLeftByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pLeftByName;
        }
        else if (strcmp(name.c_str(), pCurrent->name.c_str()) > 0 || order > 0) {
            if (pCurrent->pRightByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pRightByName;
        }
        else order--;
    }

    return pCurrent;
}

inline std::string BinaryTree::FindByName(const std::string &name, const char split, const int order) const {
    const Node *temp = FindByName(name, order);
    return temp != nullptr ? temp->ToString(dataArr[temp->index], split) : " ";
}


inline BinaryTree::Node *BinaryTree::FindByPrice(const int price, int order) const {
    Node *pCurrent = pRootByPrice;

    while (price != pCurrent->price || order > -1) {
        if (price < pCurrent->price) {
            if (pCurrent->pLeftByPrice == nullptr)
                return nullptr;
            pCurrent = pCurrent->pLeftByPrice;
        }
        else if (price > pCurrent->price || order > 0) {
            if (pCurrent->pRightByPrice == nullptr)
                return nullptr;
            pCurrent = pCurrent->pRightByPrice;
        }
        else order--;
    }

    return pCurrent;
}

inline std::string BinaryTree::FindByPrice(const int price, const char split, int order) const {
    const Node *temp = FindByPrice(price, order);
    return temp != nullptr ? temp->ToString(dataArr[temp->index], split) : " ";
}

inline void BinaryTree::Show(const Node *node, std::string &output, CoffeeVariety *dataArr, bool byName, int depth ) {
    if (!node) return;
    Show(byName ? node-> pRightByName : node->pRightByPrice, output, dataArr, byName, depth + 1);
    output += std::string(depth * 20, ' ') + node->ToString(dataArr[node->index]) + "\n";
    Show(byName ? node->pLeftByName : node->pLeftByPrice, output, dataArr, byName, depth + 1);
}

inline std::string BinaryTree::Show(const bool byName) const {
    std::string output;
    Show(byName ? pRootByName : pRootByPrice, output, dataArr, byName);
    return output;
}

#endif