#include <iostream>
#include "interface.h"
#include "BinaryTree.h"
#include "List.h"

struct CoffeeVariety {
    std::string name;
    std::string origin;
    std::string roast;
    int caffeine{};
    int price{};
};


void push(BinaryTree *tree, List *listC, const std::string &name, const std::string &origin, const std::string &roast, const int caffeine, const int price) {
    tree->push(name, origin, roast, caffeine, price);
    listC->push(name, origin, roast, caffeine, price);
}

void inputPush (BinaryTree *tree, List *listC, int count = 20) {
    CoffeeVariety temp;

    while (count-- > 0) {
        temp.name = InputString("Введите название (Чтобы закончить, нажмите Enter, ничего не вводя.):", true);

        if (Trim(temp.name).empty())
            break;

        temp.origin = InputString("Введите страну происхождения:");
        temp.roast = InputString("Введите степень обжарки:");
        temp.caffeine = InputInt("Введите содержание кофеина:");
        temp.price = InputInt("Введите цену:");
        push(tree, listC, temp.name, temp.origin, temp.roast, temp.caffeine, temp.price);
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
    int count = 20;
    char yesNoOptions[2][1024] = {"Да", "Нет"};

    if (Menu (yesNoOptions, 2, const_cast<char *>("Загрузить данные из шаблона?")) == 0) {
        push(&coffeeTree, &coffeeList, "Иргачиф", "Эфиопия", "Светлая", 50, 46);
        push(&coffeeTree, &coffeeList, "Супремо", "Колумбия", "Средняя", 60, 30);
        push(&coffeeTree, &coffeeList, "Сантос", "Бразилия", "Средняя", 60, 28);
        push(&coffeeTree, &coffeeList, "Малабарский муссон", "Индия", "Тёмная", 70, 29);
        push(&coffeeTree, &coffeeList, "Антигуа", "Гватемала", "Светлая", 50, 35);
        push(&coffeeTree, &coffeeList, "Тарразу", "Коста Рика", "Средняя", 60, 38);
        std::cout << "Загружено:\n" << coffeeList.Show() << '\n';
        count -= 6;
    }

    inputPush(&coffeeTree, &coffeeList, count);

    bool isRunning = true;
    std::string content;

    while (isRunning) {
        char options[][1024] = {"Показать список", "Найти по названию", "Найти по цене", "Показать дерево", "Выйти"};
        char sortOptions[][1024] = {"По названию", "По цене", "По порядку добавления"};
        switch (Menu(options, 5, const_cast<char *>("Выберите действие:"), content)) {
            case 0: {
                content = coffeeList.Show('\n', Menu (sortOptions, 3, const_cast<char *>("Выберите порядок:"), content));
                break;
            }
            case 1: {
                std::string searchName = InputString("Введите название:");
                int order = 0;
                content = "";
                std::string coffeeString = coffeeList.FindByName(searchName, ' ', order);

                if (coffeeString.empty())
                    content = "Такого элемента нет.";
                else {
                    coffeeString = "1. " + coffeeString;
                    content += coffeeString + '\n';

                    while (!coffeeString.empty()) {
                        coffeeString = static_cast<char>(order + 1) + ". " + coffeeList.FindByName(searchName, ' ', ++order);
                        content += coffeeString + '\n';
                    }
                }
                break;
            }
            case 2: {
                const int searchPrice = InputInt("Введите цену:");
                int order = 0;
                content = "";
                std::string coffeeString = coffeeList.FindByPrice(searchPrice, ' ', order);

                if (coffeeString.empty())
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
                content = coffeeTree.Show(Menu (sortOptions, 2, const_cast<char *>("Выберите порядок:"), content) == 0);
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
