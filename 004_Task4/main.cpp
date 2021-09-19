#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

/*
 * Требуется реализовать упрощённую симуляцию работы банкомата.
 * В банкомате могут храниться только бумажные купюры номиналом от 100 до 5000 рублей.
 * Максимально в банкомате может храниться только 1000 купюр. Все они записываются в отдельный файл.
 * У банкомата, как у устройства, две основных операции — снятие денег пользователем и наполнение деньгами инкассаторами банка.
 * Наполнение банкомата происходит, если при старте программы в консоль вводится символ плюс “+”.
 * Количество купюр рассчитывается так, чтобы банкомат был заполнен полностью. Все купюры при этом выбираются случайным образом.
 * Если на старте программы в консоль вводится минус (“-”), то симулируется снятие пользователем денег.
 * Пользователь указывает сумму с точностью до 100 рублей.
 * Мы будем считать, что каждый клиент обладает неограниченным балансом в системе и теоретически может снять любую сумму.
 * На практике, если данная сумма не может быть снята из-за отсутствия подходящих денег в машине, показывается сообщение,
 * что эта операция невозможна.
 * После выполнения любой из операций программа завершает выполнение.
 * Состояние банкомата должно храниться в отдельном бинарном файле, автономно.
 *
 * Советы и рекомендации
 * Вы можете хранить массив купюр целиком, помечая отсутствующие позиции нулями.
 */
std::vector<int> money; // вектор с купюрами
int banknote[6] = {100, 200, 500, 1000, 2000, 5000}; // перечень банкнот
bool status = true; // статус корректности пользовательского ввода, открытия файлов и т.д.

void InitMoney() { // создание исходного вектора с купюрами
    for (int i = 0; i < 1000; i++) {
        money.push_back(banknote[rand() % 6]);
    }
}

void WriteBankFile() { // запись вектора с купюрами в файл
    std::string answer = "Y";
    std::ofstream out_file("..\\bank.bin", std::ios::binary);
    if (out_file.is_open()) {
        for (int i = 0; i < money.size(); i++) {
            out_file << money[i] << std::endl;
        }
    } else {
        std::cout << "Opening file error" << std::endl;
        status = false;
    }
    out_file.close();
}

bool CheckSpace(int a, int b) { // проверка свободного/занятого купюрами места в банкомате
    int count = 0;
    for (int i = 5; i >= 0; i--) {
        int part = b / banknote[i];
        count += part;
        b -= (part * banknote[i]);
    }
    return a >= count;
}

std::string AskUser() { // продолжаем ли работу с банкоматом
    std::string str;
    std::cout << "Do you want to make another operation? Press Y/N" << std::endl;
    std::cin >> str;
    return str;
}

bool CheckCash(int a) { // проверка введенной суммы
    return ((a % 100 == 0) && (a >= 100));
}

std::vector<int> CreateCashVector(int a, int i = 5) { // создание вектора с перечнем банкнот
    std::vector<int> vec;
    while (a != 0 && i >= 0) {
        int count = a / banknote[i];
        a -= count * banknote[i];
        while (count > 0) {
            vec.push_back(banknote[i]);
            count--;
        }
        i--;
    }
    return vec;
}

int VectorSum(std::vector<int> vec) { // расчет суммы вектора
    int sum = 0;
    for (int i = 0; i < vec.size(); i++) {
        sum += vec[i];
    }
    return sum;
}

int VectorMin(std::vector<int> vec) { // поиск минимума вектора
    int min = 5000;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] != 0 && vec[i] < min) {
            min = vec[i];
        }
        if (min == 100) {
            break;
        }
    }
    return min;
}

void Deposit(std::vector<int> vec) { // взнос наличных

    for (int i = 0; i < vec.size(); i++) {
        bool replace = true;
        for (int j = 0; j < money.size() && replace; j++) {
            if (vec[i] == money[j]) {
                money[i] = vec[i];
                replace = false;
            }
        }
    }
    WriteBankFile();
    std::cout << "Your money is on deposit" << std::endl;
}

void Withdrawal(std::vector<int> vec) { // снятие наличных
    int pos = 0;
    int range = vec.size() - 1;
    while (!vec.empty() && pos <= range) {
        bool replace = true;
        for (int i = 0; i < money.size() && replace; i++) {
            if (vec[pos] == money[i]) {
                money[i] = 0;
                vec[pos] = 0;
                pos++;
                replace = false;
            }
            if (i == money.size() - 1) {
                pos++;
            }
        }
    }

    int sum = 0;
    int max = 0;
    for (int i = 0; i < vec.size(); i++) {
        sum += vec[i];
        if (vec[i] > max) {
            max = vec[i];
        }
    }

    if (!vec.empty() && sum != 0 && max > 100) {
        int m;
        for (int i = 5; i >= 0; i--) {
            if (banknote[i] == max) {
                m = i - 1;
            }
        }
        Withdrawal(CreateCashVector(sum,m));
    } else {
        std::cout << "Take your money" << std::endl;
    }
    WriteBankFile();
}

int main() {
    std::srand(std::time(nullptr));

    InitMoney();
    WriteBankFile();

    int fullSlot = 1000;
    int emptySlot = 0;
    std::string answer = "Y";
    std::string operation;

    while (answer == "Y" && status) {
        std::cout << "Input your operation: +/-/c (deposit/withdrawal/cancel)" << std::endl;
        std:: cin >> operation;
        int cash;
        if (operation == "+") {
            std::cout << "Input amount of money for deposit:" << std::endl;
            std::cin >> cash;
            if (CheckCash(cash)) {
                if (CheckSpace(emptySlot, cash)) {
                    std::vector<int> vec = CreateCashVector(cash);
                    Deposit(vec);
                    emptySlot -= vec.size();
                    fullSlot += vec.size();
                } else {
                    std::cout << "ATM is full. Impossible to deposit money" << std::endl;
                }
            } else {
                std::cout << "Incorrect amount of money" << std::endl;
            }
            answer = AskUser();
        } else if (operation == "-") {
            std::cout << "Input amount of money for withdrawal:" << std::endl;
            std::cin >> cash;
            if (CheckCash(cash)) {
                std::vector<int> vec = CreateCashVector(cash);
                if (CheckSpace(fullSlot, cash) && VectorSum(vec) <= VectorSum(money) && cash >= VectorMin(money)) {
                    Withdrawal(vec);
                    emptySlot += vec.size();
                    fullSlot -= vec.size();
                } else {
                    std::cout << "Not enough money in ATM" << std::endl;
                }
            } else {
                std::cout << "Incorrect amount of money" << std::endl;
            }
            answer = AskUser();
        } else {
            std::cout << "Operation cancelled" << std::endl;
            status = false;
        }
    }
    std::cout << "Thank you for the visit!" << std::endl;
}


