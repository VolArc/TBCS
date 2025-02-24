#ifndef INTERFACE_H
#define INTERFACE_H
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstdint>
#define uint unsigned int

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
    #define CLEAR "cls"
    #define PAUSE system("pause");
    #define GETCHAR _getch()
#else
    #include <termios.h>
    #define CLEAR "clear"
    #define PAUSE getchar()
    #define GETCHAR getchar()
#endif

inline void Pause() {
#ifndef _WIN32
    termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
#endif
    PAUSE;
#ifndef _WIN32
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}

inline int Menu(char options[][1024], const int numOptions, char message[], const std::string& content = "") {
    #ifndef _WIN32
    termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    #endif
    int chosenOption = 0;

    while (true) {
        system(CLEAR);

        printf ("%s\n", message);

        for (int i = 0; i < numOptions; i++) {
            if (i == chosenOption) {
                printf("\x1b[36m%d. %s\n\x1b[0m", i, options[i]);
            } else {
                printf("%d. %s\n", i, options[i]);
            }
        }

        if (!content.empty())
            std::cout << content;

#ifdef _WIN32
        int command = GETCHAR;

        if (command == 224) {
            command = _getch();
            switch (command) {
                case 72: command = 'w'; break;
                case 80: command = 's'; break;
            }
        }

        else if (command == 13) command = 'e';
#else

        int command = GETCHAR;
        if (command == '\033') {
            if (GETCHAR == '[') {
                switch (GETCHAR) {
                    case 'A': command = 'w'; break;
                    case 'B': command = 's'; break;
                    default: break;
                }
            }
        }
#endif

        if (command == 'W' || command == static_cast<unsigned char>('ц') || command == static_cast<unsigned char>('Ц'))
            command = 'w';
        else if (command == 'S' || command == static_cast<unsigned char>('ы') || command == static_cast<unsigned char>('Ы'))
            command = 's';
        else if (command == 'E' || command == static_cast<unsigned char>('у') || command == static_cast<unsigned char>('У') || command == '\n')
            command = 'e';

        switch (command) {
            case 'w': {
                chosenOption = (chosenOption - 1 + numOptions) % numOptions;
                break;
            }
            case 's': {
                chosenOption = (chosenOption + 1) % numOptions;
                break;
            }
            case 'e':
                #ifndef _WIN32
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                #endif
                return chosenOption;
            default: break;
        }
    }
}


#ifdef _WIN32
inline int InputInt(const std::string &message = "", const int max = INT32_MAX) {
    if (!message.empty()) {
        std::cout << message << '\n';
    }

    long int input = 0;
    char c;
    while (true) {
        c = GETCHAR;
        if (c >= '0' && c <= '9') {
            input *= 10;
            input += c - '0';

            if (input <= max) {
                std::cout << c;
            } else {
                input /= 10;
            }
        } else if (c == 8) {
            input /= 10;
            std::cout << "\b \b";
        } else if (c == '\r') {
            break;
        }
    }

    std::cout << '\n';
    return static_cast<int>(input);
}
#else
inline int InputInt(const std::string &message = "", const int max = INT32_MAX) {
    termios oldt{}, newt{};
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    if (!message.empty()) {
        std::cout << message << '\n';
    }

    long int input = 0;
    char c;

    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c >= '0' && c <= '9') {
            input *= 10;
            input += c - '0';

            if (input <= max) {
                write(STDOUT_FILENO, &c, 1);
            } else {
                input /= 10;
            }
        } else if (c == 127) {
            input /= 10;
            write(STDOUT_FILENO, "\b \b", 3);
        } else if (c == '\n' || c == '\r') {
            break;
        }
    }

    std::cout << '\n';
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return static_cast<int>(input);
}
#endif
inline std::string Trim (std::string str) {
    while (str[0] == ' ' && !str.empty()) {
        str = str.substr(1);
    }

    while (str[str.size() - 1] == ' ' && !str.empty()) {
        str = str.substr(0, str.size() - 1);
    }

    while (str.find("  ") != std::string::npos && !str.empty()) {
        str = str.replace(str.find("  "), 2, " ");
    }

    return str;
}

inline std::string InputString (const std::string &message = "", const bool emptyAllowed = false) {
    std::string input;

    do {
        if (!message.empty()) {
            std::cout << '\n' << message << ' ';
        }
        std::getline(std::cin, input);
    } while (Trim(input).empty() && !emptyAllowed);

    return input;
}
#endif
