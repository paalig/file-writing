#include <iostream>
#include <fstream>
#include <vector>

/*
 * Реализуйте простую симуляцию игры в рыбалку.
 * В исходном текстовом файле реки (river.txt) задаётся список из видов рыб, которые можно в ней поймать.
 * Рыбок может быть сколько угодно, разных видов. Виды при этом могут повторяться.
 * В начале программы пользователь указывает, какую именно рыбу он сейчас будет ловить — вид этой рыбы.
 * После этого из первого файла друг за другом осуществляется чтение его содержимого, вид за видом.
 * Если на отдельных шагах вид совпал с указанным пользователем, в выходной файл basket.txt (корзинка) записывается этот вид.
 * В конце программы показывается, сколько было поймано рыб за текущую ловлю.
 * Программу можно запускать несколько раз, при этом уже пойманные рыбы должны сохраняться в файле-корзинке.
 */

bool CheckAnswer(const std::string& str) {
    return str == "Y";
}

int main() {
    std::ifstream in_file;
    std::ofstream out_file("C:\\Users\\Ana\\ClionProjects\\file-writing\\003_Task3\\basket.txt", std::ios::app);
    std::string answer = "Y", fish, currentFish;
    int totalRiverCount = 0, totalBasketCount = 0;
    bool checkCount = true; //количество рыбы в реке не посчитано
    bool checkFish = true; //рыба не вылавливалась
    std::vector<std::string> vec;

    while (CheckAnswer(answer) && out_file.is_open()) {
        std::cout << "What kind of fish do you want to catch?" << std::endl;
        std::cin >> fish;
        if (vec.empty()) {
            vec.push_back(fish);
        } else {
            for (int i = 0; i < vec.size(); i++) {
                if (vec[i] == fish) {
                    checkFish = false;
                }
            }
            if (checkFish) {
                vec.push_back(fish);
            }
        }

        in_file.open("C:\\Users\\Ana\\ClionProjects\\file-writing\\003_Task3\\river.txt");
        if (!in_file.is_open()) {
            break;
        }

        if (checkFish) {
            int count = 0;
            while (!in_file.eof()) {
                in_file >> currentFish;
                if (checkCount) {
                    totalRiverCount++;
                }
                if (currentFish == fish) {
                    out_file << currentFish << std::endl;
                    count++;
                }
            }
            checkCount = false;
            totalBasketCount += count;
        }
        in_file.close();
        std::cout << "Do you want to catch more fish? (Press Y/N)" << std::endl;
        std::cin >> answer;
        checkFish = true;
        if (totalBasketCount >= totalRiverCount) {
            break;
        }
    }
    std::cout << totalBasketCount << " fish in your basket." << std::endl;
    if (totalBasketCount == totalRiverCount) {
        std::cout << "No more fish in the river!" << std::endl;
    }
    out_file.close();
}
