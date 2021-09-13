#include <iostream>
#include <fstream>

/*
 * В одном из прошлых заданий мы уже создавали программу чтения и анализа ведомости выплат.
 * Теперь требуется создать простую программу записи в эту ведомость.
 * Формат ведомости прежний.
 * Сначала идёт имя и фамилия получателя денежных средств, далее располагается дата выдачи в формате ДД.ММ.ГГГГ.
 * Завершает запись сумма выплаты в рублях. Данные разделяются между собой пробелами.
 * В конце каждой записи должен быть расположен перевод строки.
 *
 * При старте программы пользователь последовательно вводит данные для новой записи,
 * которые записываются затем в файл в текстовом режиме.
 * Программа должна добавлять новые записи в конец файла, не удаляя его текущее содержимое.
 *
 * Советы и рекомендации
 * Введённые данные рекомендуется хотя бы минимально валидировать перед их записью.
 * Для валидации даты в указанном формате можно использовать функции std::stoi и std::subtr
 * с корректно указанным диапазоном символов в дате.
 * К примеру, вызов std::stoi(date.substr(3, 2)) вычленит из строковой даты целочисленный месяц для последующего анализа.
 */

bool CheckMoney(std::string str) {
    return std::stoi(str) > 0;
}

bool CheckDate(std::string str) {
    if(str.length() != 10 || str[2] != '.' || str[5] != '.') {
        return false;
    }
    int day = std::stoi(str.substr(0, 2));
    int month = std::stoi(str.substr(3, 2));
    int year = std::stoi(str.substr(6, 4));
    if (day < 1 || month < 1 || month > 12 || year < 1) {
        return false;
    }
    if ((month == 1 ||month == 3 ||month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31) {
        return false;
    }
    if ((month == 4 ||month == 6 ||month == 9 || month == 11) && day > 30) {
        return false;
    }
    if ((month == 2) && (day > 29) && ((year %4 == 0 && year % 100 != 0) || (year %4 == 0 && year % 100 == 0 && year % 400 == 0))) {
        return false;
    }
    if ((month == 2) && (day > 28) && ((year % 4 != 0) || (year % 4 == 0 && year % 100 == 0 && year % 400 != 0))) {
        return false;
    }
    return true;
}

bool CheckName(std::string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] < 'A' || str[i] > 'z' || ((str[i] > 'Z') && (str[i]) < 'a')) {
            return false;
        }
    }
    return true;
}

bool Continue(const std::string& str) {
    return (str == "Y");
}

int main() {
    std::ofstream file("C:\\Users\\Ana\\ClionProjects\\file-writing\\001_Task1\\bill.txt", std::ios::app);
    std::string answer = "Y", name, surname, date, money;
    //int money;

    while (Continue(answer)) {
        std::cout << "Input first and last name, date (DD.MM.YYYY format) and amount of money: " << std::endl;
        std::cin >> name >> surname >> date >> money;
        if (CheckName(name) && CheckName(surname) && CheckDate(date) && CheckMoney(money)) {
            file << name << " " << surname << " " <<  date << " " <<  money << std::endl;
        } else {
            std::cout << "Wrong input" << std::endl;
        }
        std::cout << "Do you want to make another record? Press Y or N" << std::endl;
        std::cin >> answer;
    }
    file.close();
}
