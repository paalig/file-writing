#include <iostream>
#include <fstream>
#include <ctime>

/*
 * С помощью генератора случайных чисел рисуется картинка из нулей и единиц.
 * На вход от пользователя принимается размер картины: высота и ширина в пикселях.
 * На выходе нужно создать файл pic.txt, содержимое которого будет наполнено случайными нулями и единицами в рамках размера картины.
 * Это ещё не всё. Если вы запустите программу несколько раз на одних и тех же исходных данных (ширина и высота картины),
 * вы очень скоро обнаружите, что всё время выводится одна и та же картинка.
 * Дело в том, что у генератора случайных чисел есть ещё и «зерно», от которого он отталкивается.
 * Если это зерно не меняется, то и серия из rand() всегда будет возвращать одни и те же случайные числа.
 * Чтобы разнообразить вывод, вы можете использовать вызов std::srand(std::time(nullptr)); в самом начале программы.
 * Эта операция задаёт начальное зерно случайных чисел в зависимости от текущего времени.
 * Для её корректной работы не забудьте также включить заголовочный файл <ctime>.
 */
bool CheckNumber(int a) {
    return a > 0;
}

int main() {
    std::srand(std::time(nullptr));
    std::ofstream file("C:\\Users\\Ana\\ClionProjects\\file-writing\\002_Task2\\pic.txt", std::ios::app);
    int height, width;

    std::cout << "Input picture height: " << std::endl;
    std::cin >> height;
    std::cout << "Input picture width: " << std::endl;
    std::cin >> width;

    if (CheckNumber(height) && CheckNumber(width) && file.is_open()) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                file << (std::rand() % 2);
            }
            file << std::endl;
        }
    } else {
        std::cout << "Wrong input" << std::endl;
    }
    file.close();
}