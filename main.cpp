#include <cstring>
#include <iostream>
#include <stack>

#include "interface.h"

struct CoffeeVariety {
    std::string name;
    std::string origin;
    std::string roast;
    int caffeine{};
    int price{};
};

class BinaryTree {
public:
    BinaryTree();
    ~BinaryTree();

    void push (const CoffeeVariety &data) {
        if (pRootByName == nullptr) {
            pRootByName = new Node(data);
            pRootByPrice = pRootByName;
        } else {
            Node *pCurrentByName = pRootByName;
            Node *pCurrentByPrice = pRootByPrice;
            Node *pPrevByName = nullptr;
            Node *pPrevByPrice = nullptr;

            while (pCurrentByName != nullptr) {
                pPrevByName = pCurrentByName;
                if (strcmp(data.name.c_str(), pCurrentByName->data.name.c_str()) < 0) {
                    pCurrentByName = pCurrentByName->pLeftByName;
                } else {
                    pCurrentByName = pCurrentByName->pRightByName;
                }
            }

            while (pCurrentByPrice != nullptr) {
                pPrevByPrice = pCurrentByPrice;
                if (data.price < pCurrentByPrice->data.price) {
                    pCurrentByPrice = pCurrentByPrice->pLeftByPrice;
                } else {
                    pCurrentByPrice = pCurrentByPrice->pRightByPrice;
                }
            }

            const auto pTemp = new Node(data);

            if (strcmp(data.name.c_str(), pPrevByName->data.name.c_str()) < 0)
                pPrevByName->pLeftByName = pTemp;
            else
                pPrevByName->pRightByName = pTemp;

            if (data.price < pPrevByPrice->data.price)
                pPrevByPrice->pLeftByPrice = pTemp;
            else
                pPrevByPrice->pRightByPrice = pTemp;
        }
    }

private:
    class Node {
    public:
        Node *pLeftByName;
        Node *pRightByName;
        Node *pLeftByPrice;
        Node *pRightByPrice;
        CoffeeVariety data;

        explicit Node (const CoffeeVariety &data, Node *pLeftByName = nullptr, Node *pRightByName = nullptr, Node *pLeftByPrice = nullptr, Node *pRightByPrice = nullptr) {
            this->data = data;
            this->pLeftByName = pLeftByName;
            this->pRightByName = pRightByName;
            this->pLeftByPrice = pLeftByPrice;
            this->pRightByPrice = pRightByPrice;
        }

        [[nodiscard]] std::string ToString (const char split = ' ') const {
            return data.name + split + data.origin + split + std::to_string(data.caffeine) + split + data.roast + split + std::to_string(data.price);
        }
    };

    Node *pRootByName;
    Node *pRootByPrice;

public:
    [[nodiscard]] Node *FindByName(const std::string &name, int order = 0) const;
    [[nodiscard]] std::string FindByName(const std::string &name, char split, int order = 0) const;
    [[nodiscard]] Node *FindByPrice(int price, int order = 0) const;
    [[nodiscard]] std::string FindByPrice (int price, char split, int order = 0) const;
    [[nodiscard]] std::string Show () const;
    static void Show(const Node *node, std::string &output, int depth = 0);
};

BinaryTree::BinaryTree() {
    pRootByName = nullptr;
    pRootByPrice = nullptr;
}

BinaryTree::~BinaryTree() {
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

BinaryTree::Node *BinaryTree::FindByName(const std::string &name, int order) const {
    Node *pCurrent = pRootByName;

    while (strcmp(name.c_str(), pCurrent->data.name.c_str()) != 0 || order > -1) {
        if (strcmp(name.c_str(), pCurrent->data.name.c_str()) < 0) {
            if (pCurrent->pLeftByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pLeftByName;
        }
        else if (strcmp(name.c_str(), pCurrent->data.name.c_str()) > 0 || order > 0) {
            if (pCurrent->pRightByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pRightByName;
        }
        else order--;
    }

    return pCurrent;
}

std::string BinaryTree::FindByName(const std::string &name, const char split, const int order) const {
    const Node *temp = FindByName(name, order);
    return temp != nullptr ? temp->ToString(split) : " ";
}


BinaryTree::Node *BinaryTree::FindByPrice(const int price, int order) const {
    Node *pCurrent = pRootByPrice;

    while (price != pCurrent->data.price || order > -1) {
        if (price < pCurrent->data.price) {
            if (pCurrent->pLeftByPrice == nullptr)
                return nullptr;
            pCurrent = pCurrent->pLeftByPrice;
        }
        else if (price > pCurrent->data.price || order > 0) {
            if (pCurrent->pRightByPrice == nullptr)
                return nullptr;
            pCurrent = pCurrent->pRightByPrice;
        }
        else order--;
    }

    return pCurrent;
}

std::string BinaryTree::FindByPrice(const int price, const char split, int order) const {
    const Node *temp = FindByPrice(price, order);
    return temp != nullptr ? temp->ToString(split) : " ";
}

void BinaryTree::Show(const Node *node, std::string &output, const int depth) {
    if (!node) return;
    Show(node->pRightByName, output, depth + 1);
    output += std::string(depth * 20, ' ') + node->ToString() + "\n";
    Show(node->pLeftByName, output, depth + 1);
}

std::string BinaryTree::Show() const {
    std::string output;
    Show(pRootByName, output);
    return output;
}



class List {
public:
    List();
    ~List();

    void push (const CoffeeVariety &data) {
        if (pHeadByName == nullptr) {
            pHeadByName = new Node(data);
            pHeadByPrice = pHeadByName;
            pHeadByInput = pHeadByName;
        } else {
            Node *pCurrentByName = pHeadByName;
            Node *pCurrentByPrice = pHeadByPrice;
            Node *pPrevByName = nullptr;
            Node *pPrevByPrice = nullptr;

            while (pCurrentByName != nullptr && strcmp(data.name.c_str(), pCurrentByName->data.name.c_str()) > 0) {
                pPrevByName = pCurrentByName;
                pCurrentByName = pCurrentByName->pNextByName;
            }

            while (pCurrentByPrice != nullptr && data.price > pCurrentByPrice->data.price) {
                pPrevByPrice = pCurrentByPrice;
                pCurrentByPrice = pCurrentByPrice->pNextByPrice;
            }

            const auto pTemp = new Node(data);

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
        CoffeeVariety data;


        explicit Node (const CoffeeVariety &data, Node *pNextByName = nullptr, Node *pNextByPrice = nullptr) {
            this->data = data;
            this->pNextByName = pNextByName;
            this->pNextByPrice = pNextByPrice;
            this->pNextByInput = nullptr;
        }

        [[nodiscard]] std::string ToString (const char split = ' ') const {
            return data.name + split + data.origin + split + std::to_string(data.caffeine) + split + data.roast + split + std::to_string(data.price);
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

List::List() {
    pHeadByName = nullptr;
    pHeadByPrice = nullptr;
    pHeadByInput = nullptr;
}

List::~List() {
    while (pHeadByName != nullptr) {
        const Node* temp = pHeadByName;
        pHeadByName = pHeadByName->pNextByName;
        delete temp;
    }
}

List::Node *List::FindByName(const std::string &name, int order) const {
    Node *pCurrent = pHeadByName;

    while (order > -1) {
        while (strcmp (name.c_str(), pCurrent->data.name.c_str()) > 0) {
            if (pCurrent->pNextByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pNextByName;
        }

        if (strcmp (name.c_str(), pCurrent->data.name.c_str()) < 0)
            break;

        if (strcmp (name.c_str(), pCurrent->data.name.c_str()) == 0) {
            if (order-- == 0)
                return pCurrent;

            if (pCurrent->pNextByName == nullptr)
                return nullptr;
            pCurrent = pCurrent->pNextByName;
        }
    }

    return nullptr;
}

std::string List::FindByName(const std::string &name, const char split, const int order) const {
    const Node *temp = this->FindByName(name, order);
    return temp != nullptr ? temp->ToString(split) : "";
}


List::Node *List::FindByPrice(const int price, int order) const {
    Node *pCurrent = pHeadByPrice;

    while (order > -1) {
        while (price > pCurrent->data.price) {
            if (pCurrent->pNextByPrice == nullptr)
                return nullptr;
            pCurrent = pCurrent->pNextByPrice;
        }

        if (price < pCurrent->data.price)
            break;

        if (price == pCurrent->data.price) {
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

std::string List::FindByPrice(const int price, const char split, const int order) const {
    const Node *temp = this->FindByPrice(price, order);
    return temp != nullptr ? temp->ToString(split) : "";
}

std::string List::Show(const char split, const int order) const {
    const Node *pCurrent;

    switch (order) {
        case 1:
            pCurrent = pHeadByName;
            break;
        case 2:
            pCurrent = pHeadByPrice;
            break;
        default:
            pCurrent = pHeadByInput;
            break;
    }

    std::string result;

    while (pCurrent != nullptr) {
        result += pCurrent->ToString() + split;
        pCurrent = order == 1 ? pCurrent->pNextByName :
                   order == 2 ? pCurrent->pNextByPrice :
                   pCurrent->pNextByInput;
    }

    return result;
}

class DuplicateException {
public:
    DuplicateException () {
        std::cerr << "Такой элемент уже существует.";
    };
};


void push(BinaryTree *tree, List *listC, const CoffeeVariety &coffeeVariety) {
    tree->push(coffeeVariety);
    listC->push(coffeeVariety);
}

void inputPush (BinaryTree *tree, List *listC, int count = 20) {
    CoffeeVariety temp;

    while (count-- > 0) {
        temp.name = InputString("Введите название (Чтобы закончить, нажмите Enter, ничего не вводя.):", true);

        if (temp.name.empty())
            break;

        temp.origin = InputString("Введите страну происхождения:");
        temp.roast = InputString("Введите степень обжарки:");
        temp.caffeine = InputInt("Введите содержание кофеина:");
        temp.price = InputInt("Введите цену:");
        push(tree, listC, temp);
    }
}


int main() {
    BinaryTree coffeeTree;
    List coffeeList;
    int count = 20;
    char yesNoOptions[2][1024] = {"Да", "Нет"};

    if (Menu (yesNoOptions, 2, const_cast<char *>("Загрузить данные из шаблона?")) == 0) {
        push(&coffeeTree, &coffeeList, CoffeeVariety("Иргачиф", "Эфиопия", "Светлая", 50, 46));
        push(&coffeeTree, &coffeeList, CoffeeVariety("Супремо", "Колумбия", "Средняя", 60, 30));
        push(&coffeeTree, &coffeeList, CoffeeVariety("Сантос", "Бразилия", "Средняя", 60, 28));
        push(&coffeeTree, &coffeeList, CoffeeVariety("Малабарский муссон", "Индия", "Тёмная", 70, 29));
        push(&coffeeTree, &coffeeList, CoffeeVariety("Антигуа", "Гватемала", "Светлая", 50, 35));
        push(&coffeeTree, &coffeeList, CoffeeVariety("Тарразу", "Коста Рика", "Средняя", 60, 38));
        std::cout << "Загружено:\n" << coffeeList.Show() << '\n';
        count -= 6;
    }

    inputPush(&coffeeTree, &coffeeList, count);

    bool isRunning = true;
    std::string content;

    while (isRunning) {
        char options[][1024] = {"Показать список", "Найти по названию", "Найти по цене", "Показать дерево", "Выйти"};
        char sortOptions[][1024] = {"По порядку добавления", "По названию", "По цене"};
        switch (Menu(options, 5, const_cast<char *>("Выберите действие:"), content)) {
            case 0: {
                content = coffeeList.Show('\n', Menu (sortOptions, 3, const_cast<char *>("Выберите порядок:"), content));
                break;
            }
            case 1: {
                std::string searchName = InputString("Введите название:");
                int order = 0;
                content = "";

                if (std::string coffeeString = coffeeList.FindByName(searchName, ' ', order); coffeeString.empty())
                    content = "Такого элемента нет.";
                else {
                    content += coffeeString + '\n';

                    while (!coffeeString.empty()) {
                        coffeeString = coffeeList.FindByName(searchName, ' ', ++order);
                        content += coffeeString + '\n';
                    }
                }
                break;
            }
            case 2: {
                int searchPrice = InputInt("Введите цену:");
                int order = 0;
                content = "";

                if (std::string coffeeString = coffeeList.FindByPrice(searchPrice, ' ', order); coffeeString.empty())
                    content = "Такого элемента нет.";
                else {
                    content += coffeeString + '\n';

                    while (!coffeeString.empty()) {
                        coffeeString = coffeeList.FindByPrice(searchPrice, ' ', ++order);
                        content += coffeeString + '\n';
                    }
                }
                break;
            }
            case 3: {
                content = coffeeTree.Show();
                break;
            }
            default: {
                isRunning = false;
                break;
            }
        }
    }

    coffeeTree.~BinaryTree();
    coffeeList.~List();

    return 0;
}