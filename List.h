#ifndef LIST_H
#define LIST_H

#include <cstring>
#include <iostream>


class List {
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

public:
    List();
    ~List();

    void push (const std::string &name, const std::string &origin, const std::string &roast, const int caffeine, const int price) {
        if (this->FindByName(name) != nullptr || this->FindByPrice(price) != nullptr) throw DuplicateException();

        const auto pTemp = new Node(name, price, arrLength);
        auto *tempArr = new CoffeeVariety[arrLength+1];

        for (int i = 0; i < arrLength; i++) {
            tempArr[i] = dataArr[i];
        }

        tempArr[arrLength++] = {name, origin, roast, caffeine, price};
        dataArr = tempArr;

        if (pHeadByName == nullptr) {
            pHeadByName = pTemp;
            pHeadByPrice = pHeadByName;
            pHeadByInput = pHeadByName;
        } else {
            Node *pCurrentByName = pHeadByName;
            Node *pCurrentByPrice = pHeadByPrice;
            Node *pPrevByName = nullptr;
            Node *pPrevByPrice = nullptr;

            while (pCurrentByName != nullptr && strcmp(name.c_str(), pCurrentByName->name.c_str()) > 0) {
                pPrevByName = pCurrentByName;
                pCurrentByName = pCurrentByName->pNextByName;
            }

            while (pCurrentByPrice != nullptr && price > pCurrentByPrice->price) {
                pPrevByPrice = pCurrentByPrice;
                pCurrentByPrice = pCurrentByPrice->pNextByPrice;
            }

            if (pPrevByName == nullptr) {
                pHeadByName = pTemp;
            } else {
                pPrevByName->pNextByName = pTemp;
            }

            pTemp->pNextByName = pCurrentByName;

            if (pPrevByPrice == nullptr) {
                pHeadByPrice = pTemp;
            } else {
                pPrevByPrice->pNextByPrice = pTemp;
            }

            pTemp->pNextByPrice = pCurrentByPrice;

            Node *pPrevByInput = pHeadByInput;

            while (pPrevByInput->pNextByInput != nullptr)
                pPrevByInput = pPrevByInput->pNextByInput;

            pPrevByInput->pNextByInput = pTemp;
        }
    }

    class Node {
    public:
        Node *pNextByName;
        Node *pNextByPrice;
        Node *pNextByInput;
        std::string name;
        int price{};
        int index;


        explicit Node (const std::string &name, const int price, const int index, Node *pNextByName = nullptr, Node *pNextByPrice = nullptr) {
            this->name = name;
            this->price = price;
            this->index = index;
            this->pNextByName = pNextByName;
            this->pNextByPrice = pNextByPrice;
            this->pNextByInput = nullptr;
        }

        [[nodiscard]] std::string ToString (const CoffeeVariety &data, const char split = ' ') const {
            return this->name + split + data.origin + split + std::to_string(data.caffeine) + split + data.roast + split + std::to_string(this->price);
        }
    };

    Node *pHeadByName;
    Node *pHeadByPrice;
    Node *pHeadByInput;

    [[nodiscard]] Node *FindByName(const std::string &name) const;
    [[nodiscard]] std::string FindByName (const std::string &name, char split, bool recursive) const;
    [[nodiscard]] Node *FindByPrice (int price) const;
    [[nodiscard]] std::string FindByPrice (int price, char split, bool recursive) const;
    [[nodiscard]] std::string Show (char split = '\n', int order = 0, bool reverse = false) const;
    [[nodiscard]] bool Remove(const std::string &name);
    [[nodiscard]] bool Remove (int price);
    static Node *FindByNameRecursive(Node *node, const std::string &name);
    static Node *FindByPriceRecursive(Node *node, int price);
};

inline List::List() {
    pHeadByName = nullptr;
    pHeadByPrice = nullptr;
    pHeadByInput = nullptr;
}

inline List::~List() {
    while (pHeadByName != nullptr) {
        const Node* temp = pHeadByName;
        pHeadByName = pHeadByName->pNextByName;
        delete temp;
    }
}

inline List::Node *List::FindByName(const std::string &name) const {
    Node *pCurrent = pHeadByName;

    if (!pCurrent) return nullptr;
        while (strcmp (name.c_str(), pCurrent->name.c_str()) > 0) {
            if (pCurrent->pNextByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pNextByName;
        }

        if (strcmp (name.c_str(), pCurrent->name.c_str()) == 0)
            return pCurrent;

    return nullptr;
}

inline std::string List::FindByName(const std::string &name, const char split, const bool recursive) const {
    const Node *temp = recursive ? FindByNameRecursive(pHeadByName, name) : this->FindByName(name);
    return temp != nullptr ? temp->ToString(dataArr[temp->index], split) : "";
}


inline List::Node *List::FindByPrice(const int price) const {
    Node *pCurrent = pHeadByPrice;

    if (!pCurrent) return nullptr;
    while (price > pCurrent->price) {
        if (pCurrent->pNextByName == nullptr)
            return nullptr;
        pCurrent = pCurrent->pNextByName;
    }

    if (price == pCurrent->price)
        return pCurrent;

    return nullptr;
}

inline std::string List::FindByPrice(const int price, const char split, const bool recursive) const {
    const Node *temp = recursive ? FindByPriceRecursive(pHeadByPrice, price) : this->FindByPrice(price);
    return temp != nullptr ? temp->ToString(dataArr[temp->index], split) : "";
}

inline List::Node *List::FindByNameRecursive(Node *node, const std::string &name) {
    if (!node || strcmp(name.c_str(), node->name.c_str()) <= 0)
        return (node && strcmp(name.c_str(), node->name.c_str()) == 0) ? node : nullptr;
    return FindByNameRecursive(node->pNextByName, name);
}

inline List::Node *List::FindByPriceRecursive(Node *node, const int price) {
    if (!node || price <= node->price)
        return (node && price == node->price) ? node : nullptr;
    return FindByPriceRecursive(node->pNextByPrice, price);
}

inline std::string List::Show(const char split, const int order, const bool reverse) const {
    const Node *pCurrent;

    switch (order) {
        case 0:
            pCurrent = pHeadByName;
            break;
        case 1:
            pCurrent = pHeadByPrice;
            break;
        default:
            pCurrent = pHeadByInput;
            break;
    }

    std::string output;

    while (pCurrent != nullptr) {
        std::string result = pCurrent->ToString(dataArr[pCurrent->index]);
        pCurrent = order == 0 ? pCurrent->pNextByName :
                   order == 1 ? pCurrent->pNextByPrice :
                   pCurrent->pNextByInput;

        if (!reverse) output += result + split;
        else output = result + split + output;
    }

    return output;
}

inline bool List::Remove(const std::string &name) {
    if (!pHeadByName) return false;

    Node *pPrev = nullptr;
    Node *pCurrent = pHeadByName;

    while (pCurrent && strcmp(name.c_str(), pCurrent->name.c_str()) > 0) {
        pPrev = pCurrent;
        pCurrent = pCurrent->pNextByName;
    }

    if (!pCurrent || strcmp(name.c_str(), pCurrent->name.c_str()) != 0) {
        return false;
    }

    if (pPrev) {
        pPrev->pNextByName = pCurrent->pNextByName;
    } else {
        pHeadByName = pCurrent->pNextByName;
    }

    Node *pPrevPrice = nullptr, *pCurrentPrice = pHeadByPrice;
    while (pCurrentPrice && pCurrentPrice != pCurrent) {
        pPrevPrice = pCurrentPrice;
        pCurrentPrice = pCurrentPrice->pNextByPrice;
    }

    if (pCurrentPrice) {
        if (pPrevPrice) {
            pPrevPrice->pNextByPrice = pCurrentPrice->pNextByPrice;
        } else {
            pHeadByPrice = pCurrentPrice->pNextByPrice;
        }
    }

    Node *pPrevInput = nullptr, *pCurrentInput = pHeadByInput;
    while (pCurrentInput && pCurrentInput != pCurrent) {
        pPrevInput = pCurrentInput;
        pCurrentInput = pCurrentInput->pNextByInput;
    }

    if (pCurrentInput) {
        if (pPrevInput) {
            pPrevInput->pNextByInput = pCurrentInput->pNextByInput;
        } else {
            pHeadByInput = pCurrentInput->pNextByInput;
        }
    }

    auto *tempArr = new CoffeeVariety[arrLength - 1];
    for (int i = 0, j = 0; i < arrLength; ++i) {
        if (i != pCurrent->index) {
            tempArr[j++] = dataArr[i];
        }
    }

    delete[] dataArr;
    dataArr = tempArr;
    arrLength--;

    Node *pUpdate = pHeadByInput;
    int newIndex = 0;
    while (pUpdate) {
        pUpdate->index = newIndex++;
        pUpdate = pUpdate->pNextByInput;
    }

    delete pCurrent;
    return true;
}

inline bool List::Remove(const int price) {
    if (!pHeadByPrice) return false;

    Node *pCurrent = pHeadByPrice;

    while (pCurrent && price > pCurrent->price)
        pCurrent = pCurrent->pNextByPrice;

    if (!pCurrent || price != pCurrent->price)
        return false;

    return Remove(pCurrent->name);
}

#endif