#include <iostream>
#include <string>
#include "interface.h"
#include "BinaryTree.h"
#include "List.h"
#include "Array.h"

struct CoffeeVariety {
    std::string name;
    std::string origin;
    std::string roast;
    int caffeine{};
    int price{};
};


void push(BinaryTree *tree, List *listC, Array *arr, const std::string &name, const std::string &origin, const std::string &roast, const int caffeine, const int price) {
    tree->push(name, origin, roast, caffeine, price);
    listC->push(name, origin, roast, caffeine, price);
    arr->push(name, origin, roast, caffeine, price);
}

void inputPush (BinaryTree *tree, List *listC, Array *arr, int count = 20) {
    CoffeeVariety temp;

    while (count-- > 0) {
        temp.name = InputString("Введите название (Чтобы закончить, нажмите Enter, ничего не вводя.):", true);

        if (Trim(temp.name).empty())
            break;

        temp.origin = InputString("Введите страну происхождения:");
        temp.roast = InputString("Введите степень обжарки:");
        temp.caffeine = InputInt("Введите содержание кофеина:");
        temp.price = InputInt("Введите цену:");
        push(tree, listC, arr, temp.name, temp.origin, temp.roast, temp.caffeine, temp.price);
    }
}


int main() {
    #ifdef _WIN32
    SetConsoleCP( 65001 );
    SetConsoleOutputCP( 65001 );
    #endif
    std::cout << "\x1B[41mВНИМАНИЕ!!!\nСправка.\033[0m\n"
                 "Рекомендуется открыть программу в отдельном терминале, а не в терминале IDE.\n"
                 "Для перемещения между кнопками меню использовать стрелки вверх и вниз, либо кнопки W и S.\n"
                 "Для выбора пункта меню использовать Enter или кнопку E.\n"
                 "Если кнопки не работают, попробуйте переключить язык на английский и отключить Caps Lock.\n"
                 "Если и это не сработает, поставьте Linux. Именно под него писалась прога. Там всё работает.\n\n"
                 "Нажимая на любую кнопку, вы передаёте мне все сохранённые на устройстве пароли и персональные данные и соглашаетесь на их обработку. (или нет) . . .";
    Pause();
    BinaryTree coffeeTree;
    List coffeeList;
    Array coffeeArray;
    int count = 20;
    char yesNoOptions[2][1024] = {"Да", "Нет"};

    if (Menu (yesNoOptions, 2, const_cast<char *>("Загрузить данные из шаблона?")) == 0) {
        push(&coffeeTree, &coffeeList, &coffeeArray, "Иргачиф", "Эфиопия", "Светлая", 50, 46);
        push(&coffeeTree, &coffeeList, &coffeeArray, "Супремо", "Колумбия", "Средняя", 60, 30);
        push(&coffeeTree, &coffeeList, &coffeeArray, "Сантос", "Бразилия", "Средняя", 60, 28);
        push(&coffeeTree, &coffeeList, &coffeeArray, "Малабарский муссон", "Индия", "Тёмная", 70, 29);
        push(&coffeeTree, &coffeeList, &coffeeArray, "Антигуа", "Гватемала", "Светлая", 50, 35);
        push(&coffeeTree, &coffeeList, &coffeeArray, "Тарразу", "Коста Рика", "Средняя", 60, 38);
        std::cout << "Загружено:\n" << coffeeList.Show() << '\n';
        count -= 6;
    }

    inputPush(&coffeeTree, &coffeeList, &coffeeArray, count);

    bool isRunning = true;
    std::string content;

    while (isRunning) {
        char options[][1024] = {"Добавить элементы", "Показать список", "Найти в списке", "Удалить из списка", "Показать дерево", "Балансировать дерево", "Найти в дереве", "Удалить из дерева", "Показать массив", "Найти в массиве", "Редактировать элемент", "Удалить элемент", "Выйти"};
        char sortOptions[][1024] = {"По названию", "По цене", "По порядку добавления"};
        char treeOptions[][1024] = {"В виде дерева", "В виде списка"};
        char orderOptions[][1024] = {"В прямом", "В обратном"};
        switch (Menu(options, 13, const_cast<char *>("Выберите действие:"), content)) {
            case 0: {
                inputPush(&coffeeTree, &coffeeList, &coffeeArray, count);
                break;
            }
            case 1: {
                content = coffeeList.Show('\n', Menu (sortOptions, 3, const_cast<char *>("Выберите порядок:"), content), Menu (orderOptions, 2, const_cast<char *>("Выберите порядок:"), content) == 1);
                break;
            }
            case 2: {
                std::string coffeeString;
                if (Menu(sortOptions, 2, const_cast<char *>("Выберите атрибут")) == 0)
                    coffeeString = coffeeList.FindByName(InputString("Введите название:"), ' ', Menu (yesNoOptions, 2, const_cast<char *>("Рекурсией?")) == 0);
                else
                    coffeeString = coffeeList.FindByPrice(InputInt("Введите цену:"), ' ', Menu (yesNoOptions, 2, const_cast<char *>("Рекурсией?")) == 0);
                if (coffeeString.empty()) content = "Такого элемента нет.";
                else content = coffeeString;
                break;
            }
            case 3: {
                if (Menu(sortOptions, 2, const_cast<char *>("Выберите атрибут")) == 0)
                    if (coffeeList.Remove(InputString("Введите название:")))
                        content = "Элемент удалён.";
                    else content = "Такого элемента нет";
                else if (coffeeList.Remove(InputInt("Введите цену:")))
                        content = "Элемент удалён.";
                else content = "Такого элемента нет";
                break;
            }
            case 4: {
                content = Menu(treeOptions, 2, const_cast<char *>("В каком виде?")) == 0
                ? coffeeTree.Show(Menu (sortOptions, 2, const_cast<char *>("Выберите порядок:"), content) == 0)
                : coffeeTree.ToString(Menu (sortOptions, 2, const_cast<char *>("Выберите порядок:"), content) == 0,
                    Menu (orderOptions, 2, const_cast<char *>("Выберите порядок:"), content) == 1);
                break;
            }
            case 5: {
                coffeeTree.balanceTree();
                coffeeTree.balanceTree(false);
                content = "Дерево сбалансировано.";
                break;
            }
            case 6: {
                std::string coffeeString;
                if (Menu(sortOptions, 2, const_cast<char *>("Выберите атрибут")) == 0)
                    coffeeString = coffeeTree.FindByName(InputString("Введите название:"), ' ', Menu (yesNoOptions, 2, const_cast<char *>("Рекурсией?")) == 0);
                else
                    coffeeString = coffeeTree.FindByPrice(InputInt("Введите цену:"), ' ', Menu (yesNoOptions, 2, const_cast<char *>("Рекурсией?")) == 0);
                if (coffeeString.empty()) content = "Такого элемента нет.";
                else content = coffeeString;
                break;
            }
            case 7: {
                if (Menu(sortOptions, 2, const_cast<char *>("Выберите атрибут")) == 0)
                    if (coffeeTree.Remove(InputString("Введите название:")))
                        content = "Элемент удалён.";
                    else content = "Такого элемента нет";
                else if (coffeeTree.Remove(InputInt("Введите цену:")))
                    content = "Элемент удалён.";
                else content = "Такого элемента нет";
                break;
            }
            case 8: {
                content = coffeeArray.Show(Menu (sortOptions, 2, const_cast<char *>("Выберите порядок:"), content) == 0, Menu (orderOptions, 2, const_cast<char *>("Выберите порядок:"), content) == 1);
                break;
            }
            case 9: {
                std::string coffeeString;
                if (Menu(sortOptions, 2, const_cast<char *>("Выберите атрибут")) == 0)
                    coffeeString = coffeeArray.FindByName(InputString("Введите название:"));
                else
                    coffeeString = coffeeArray.FindByPrice(InputInt("Введите цену:"));
                if (coffeeString.empty()) content = "Такого элемента нет.";
                else content = coffeeString;
                break;
            }
            case 10: {
                int index;
                if (Menu(sortOptions, 2, const_cast<char *>("Выберите атрибут")) == 0)
                    index = coffeeArray.FindByName(InputString("Введите название:"), 0);
                else
                    index = coffeeArray.FindByPrice(InputInt("Введите цену:"));
                if (index == -1) {
                    content = "Такого элемента нет.";
                    break;
                }
                if (coffeeArray.Edit(index, InputString("Введите новое название:"), InputString("Введите новое происхождение:"), InputString("Введите обжарку:"), InputInt("Введите содержание кофеина:"), InputInt("Введите новую цену"))) content = "Элемент редактирован.";
                break;
            }
            case 11: {
                if (Menu(sortOptions, 2, const_cast<char *>("Выберите атрибут")) == 0)
                    if (coffeeArray.Remove(InputString("Введите название:")))
                        content = "Элемент удалён.";
                    else content = "Такого элемента нет";
                else if (coffeeArray.Remove(InputInt("Введите цену:")))
                        content = "Элемент удалён.";
                else content = "Такого элемента нет";
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
