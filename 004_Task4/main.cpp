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

void InitMoney() { // создание вектора с купюрами
    for (int i = 0; i < 10; i++) {
        money.push_back(banknote[rand() % 6]);
    }
}

void WriteBankFile() { // запись исходного вектора с купюрами в файл
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

std::vector<int> CreateCashVector(int a, int i = 5) { //создание вектора с перечнем банкнот
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

void Deposit(std::vector<int> vec) { // взнос наличных
    std::ifstream read_file;
    read_file.open("..\\bank.bin", std::ios::binary);
    std::ofstream write_file("..\\bank.bin", std::ios::binary);

    if (!read_file.is_open() || !write_file.is_open()) {
        std::cout << "Opening bank file error" << std::endl;
        status = false;
    } else {
        int m = vec.size() - 1;
        while (!read_file.eof() && m >= 0) {
            int currentBanknote;
            read_file >> currentBanknote;
            if (currentBanknote == 0) {
                write_file << vec[m];
                vec[m] = 0;
                m--;
            }
        }
        std::cout << "Your money is on deposit" << std::endl;
    }
    read_file.close();
    write_file.close();
}

void Withdrawal(std::vector<int> vec) { // снятие наличных
    std::ifstream read_file;
    read_file.open("..\\bank.bin", std::ios::binary);
    char buffer[10];
    //std::ofstream write_file("..\\bank.bin", std::ios::binary);

    if (!read_file.is_open()) {
        std::cout << "Opening bank file error" << std::endl;
        status = false;
    } else {
        while (!read_file.eof() && !vec.empty()) {
            read_file.read(buffer, sizeof(int));
            if (std::stoi(buffer) == vec[0]) {
                std::ofstream write_file("..\\bank.bin", std::ios::binary);
                write_file << "0";
                std::remove(reinterpret_cast<const char *>(vec[0]));
                write_file.close();
            }
        }
        if (!vec.empty()) {
            int sum = 0;
            int max = 0;
            for (int i = 0; i < vec.size(); i++) {
                sum += vec[i];
                if (vec[i] > max) {
                    max = vec[i];
                }
            }
            int m;
            for (int i = 5; i >= 0; i--) {
                if (banknote[i] == max) {
                    m = i;
                }
            }
            Withdrawal(CreateCashVector(sum,m));
        }
        std::cout << "Take your money" << std::endl;
    }
    read_file.close();
}

int main() {
    std::srand(std::time(nullptr));

    InitMoney();
    WriteBankFile();

    int fullSlot = 10;//1000;
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
                    std::cout << "ATM is full" << std::endl;
                }
            } else {
                std::cout << "Incorrect amount of money" << std::endl;
            }
            answer = AskUser();
        } else if (operation == "-") {
            std::cout << "Input amount of money for withdrawal:" << std::endl;
            std::cin >> cash;
            if (CheckCash(cash)) {
                if (CheckSpace(fullSlot, cash)) {
                    std::vector<int> vec = CreateCashVector(cash);
                    Withdrawal(vec);
                    //
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


