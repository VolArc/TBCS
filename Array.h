#ifndef ARRAY_H
#define ARRAY_H
#include <cstring>
#include <iostream>

class Array {
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

        CoffeeVariety& operator =(const CoffeeVariety &other) = default;
    };



    struct CoffeeByName {
        std::string name;
        int index{};
    };

    struct CoffeeByPrice {
        int price{};
        int index{};
    };

    CoffeeByName *arrayByName = {};
    CoffeeByPrice *arrayByPrice = {};
    CoffeeVariety *dataArr = {};
    int arrLength = 0;

public:
    Array() = default;
    ~Array() {
        delete [] arrayByName;
        delete [] arrayByPrice;
        delete [] dataArr;
    }

    void push (const std::string &name, const std::string &origin, const std::string &roast, const int caffeine, const int price) {
        if (arrLength > 0 && (FindByName(name, 0, arrLength - 1) != -1 || !FindByPrice(price, false).empty())) throw DuplicateException();

        const CoffeeVariety sTemp = {name, origin, roast, caffeine, price};

        auto *tempArr = new CoffeeVariety[arrLength + 1]{};
        for (int i = 0; i < arrLength; i++) tempArr[i] = dataArr[i];

        tempArr[arrLength++] = sTemp;
        delete[] dataArr;
        dataArr = tempArr;

        if (arrLength == 1) {
            arrayByName = new CoffeeByName[1];
            arrayByName[0] = {name, 0};
            arrayByPrice = new CoffeeByPrice[1];
            arrayByPrice[0] = {price, 0};
            dataArr = new CoffeeVariety[1];
            dataArr[0] = {name, origin, roast, caffeine, price};
        }
        else {
            auto *tempNameArr = new CoffeeByName[arrLength];
            int i = -1;
            while (++i < arrLength - 1 && strcmp(name.c_str(), arrayByName[i].name.c_str()) > 0) {
                tempNameArr[i] = arrayByName[i];
            }
            tempNameArr[i] = {name, arrLength - 1};
            while (++i < arrLength) {
                tempNameArr[i] = arrayByName[i - 1];
            }

            delete [] arrayByName;
            arrayByName = tempNameArr;

            auto *tempPriceArr = new CoffeeByPrice[arrLength];
            i = -1;
            while (++i < arrLength - 1 && price > arrayByPrice[i].price) {
                tempPriceArr[i] = arrayByPrice[i];
            }
            tempPriceArr[i] = {price, arrLength - 1};
            while (++i < arrLength) {
                tempPriceArr[i] = arrayByPrice[i - 1];
            }

            delete [] arrayByPrice;
            arrayByPrice = tempPriceArr;
        }
    }

    [[nodiscard]] std::string ToString (const int index) const {
        if (index < 0 || index >= arrLength) return "";
        return std::to_string(index) + ". " + dataArr[index].name + ' ' + dataArr[index].origin + ' ' + dataArr[index].roast + ' ' + std::to_string(dataArr[index].caffeine) + ' ' + std::to_string(dataArr[index].price);
    }
    [[nodiscard]] std::string Show (const bool byName = true, const bool reverse = false, const char split = '\n') const {
        std::string output;
        if (byName) {
            for (int i = 0; i < arrLength; i++)
                if (!reverse) output += ToString(arrayByName[i].index) + split;
                else output = ToString(arrayByName[i].index) + split + output;
        }
        else {
            for (int i = 0; i < arrLength; i++)
                if (!reverse) output += ToString(arrayByPrice[i].index) + split;
                else output = ToString(arrayByPrice[i].index) + split + output;
        }

        return output;
    }

    bool Edit(const int index, const std::string &name, const std::string &origin, const std::string &roast, const int caffeine, const int price) {
        if (index < 0 || index >= arrLength) return false;
        Remove(dataArr[index].name);
        push(name, origin, roast, caffeine, price);
        return true;
    }

    bool Remove(const std::string &name) {
        const int index = FindByName(name, 0, arrLength - 1);
        if (index == -1) return false;

        auto *tempArr = new CoffeeVariety[arrLength - 1];
        auto *tempNameArr = new CoffeeByName[arrLength - 1];
        auto *tempPriceArr = new CoffeeByPrice[arrLength - 1];

        int j = 0;
        for (int i = 0; i < arrLength; i++) {
            if (i != index) {
                tempArr[j] = dataArr[i];
                tempNameArr[j] = arrayByName[j];
                tempNameArr[j].index = j;
                tempPriceArr[j] = arrayByPrice[j];
                tempPriceArr[j].index = j;
                j++;
            }
        }

        delete[] dataArr;
        delete[] arrayByName;
        delete[] arrayByPrice;

        dataArr = tempArr;
        arrayByName = tempNameArr;
        arrayByPrice = tempPriceArr;
        arrLength--;
        return true;
    }

    bool Remove(const int price) {
        const int index = FindByPrice(price);
        if (index == -1) return false;

        auto *tempArr = new CoffeeVariety[arrLength - 1];
        auto *tempNameArr = new CoffeeByName[arrLength - 1];
        auto *tempPriceArr = new CoffeeByPrice[arrLength - 1];

        int j = 0;
        for (int i = 0; i < arrLength; i++) {
            if (i != index) {
                tempArr[j] = dataArr[i];
                tempNameArr[j] = arrayByName[j];
                tempNameArr[j].index = j;
                tempPriceArr[j] = arrayByPrice[j];
                tempPriceArr[j].index = j;
                j++;
            }
        }

        delete[] dataArr;
        delete[] arrayByName;
        delete[] arrayByPrice;

        dataArr = tempArr;
        arrayByName = tempNameArr;
        arrayByPrice = tempPriceArr;
        arrLength--;

        return true;
    }

    [[nodiscard]] int FindByName(const std::string &name, const int left, const int right) const {
        if (left > right) return -1;
        const int mid = left + (right - left) / 2;
        if (arrayByName[mid].name == name) return arrayByName[mid].index;
        if (arrayByName[mid].name < name)
            return FindByName(name, mid + 1, right);
        return FindByName(name, left, mid - 1);
    }

    [[nodiscard]] int FindByName(const std::string &name, const int left) const {
        return FindByName(name, left, arrLength - 1);
    }

    [[nodiscard]] std::string FindByName (const std::string &name) const {
        return ToString (FindByName(name, 0, arrLength - 1));
    }

    [[nodiscard]] int FindByPrice(const int price) const {
        int left = 0, right = arrLength - 1;
        while (left <= right) {
            const int mid = left + (right - left) / 2;
            if (arrayByPrice[mid].price == price) return arrayByPrice[mid].index;
            if (arrayByPrice[mid].price < price)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return -1;
    }

    [[nodiscard]] std::string FindByPrice(const int price, bool) const {
        return ToString(FindByPrice(price));
    }


};

#endif