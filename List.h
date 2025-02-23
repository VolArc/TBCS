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

private:
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

public:
    [[nodiscard]] Node *FindByName(const std::string &name, int order = 0) const;
    [[nodiscard]] std::string FindByName (const std::string &name, char split, int order = 0) const;
    [[nodiscard]] Node *FindByPrice (int price, int order = 0) const;
    [[nodiscard]] std::string FindByPrice (int price, char split, int order = 0) const;
    [[nodiscard]] std::string Show (char split = '\n', int order = 0) const;
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

inline List::Node *List::FindByName(const std::string &name, int order) const {
    Node *pCurrent = pHeadByName;

    while (order > -1) {
        while (strcmp (name.c_str(), pCurrent->name.c_str()) > 0) {
            if (pCurrent->pNextByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pNextByName;
        }

        if (strcmp (name.c_str(), pCurrent->name.c_str()) < 0)
            break;

        if (strcmp (name.c_str(), pCurrent->name.c_str()) == 0) {
            if (order-- == 0)
                return pCurrent;

            if (pCurrent->pNextByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pNextByName;
        }
    }

    return nullptr;
}

inline std::string List::FindByName(const std::string &name, const char split, const int order) const {
    const Node *temp = this->FindByName(name, order);
    return temp != nullptr ? temp->ToString(dataArr[temp->index], split) : "";
}


inline List::Node *List::FindByPrice(const int price, int order) const {
    Node *pCurrent = pHeadByPrice;

    while (order > -1) {
        while (price > pCurrent->price) {
            if (pCurrent->pNextByPrice == nullptr)
                return nullptr;
            pCurrent = pCurrent->pNextByPrice;
        }

        if (price < pCurrent->price)
            break;

        if (price == pCurrent->price) {
            if (order-- == 0)
                return pCurrent;

            if (pCurrent->pNextByPrice != nullptr)
                pCurrent = pCurrent->pNextByPrice;
            else
                return nullptr;
        }
    }

    return nullptr;
}

inline std::string List::FindByPrice(const int price, const char split, const int order) const {
    const Node *temp = this->FindByPrice(price, order);
    return temp != nullptr ? temp->ToString(dataArr[temp->index], split) : "";
}

inline std::string List::Show(const char split, const int order) const {
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

    std::string result;

    while (pCurrent != nullptr) {
        result += pCurrent->ToString(dataArr[pCurrent->index]) + split;
        pCurrent = order == 0 ? pCurrent->pNextByName :
                   order == 1 ? pCurrent->pNextByPrice :
                   pCurrent->pNextByInput;
    }

    return result;
}



#endif