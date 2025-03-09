#include <iostream>
#include "shape.h"

int main() {
    // Входные данные для керосин-кислородного двигателя
    double gamma = 1.2;         // Показатель адиабаты для продуктов сгорания
    double R = 377.0;           // Удельная газовая постоянная для продуктов сгорания (Дж/(кг·К))
    double P0 = Никита добавит;         // Давление в камере сгорания (Па)
    double Pe = 1.0e5;          // Атмосферное давление (Па)
    double m_dot = Никита добавит;         // Массовый расход (кг/с)
    double Q = Никита добавит;           // Теплота сгорания топлива (Дж/кг)
    double OF_ratio = Никита добавит;     // Соотношение окислителя к топливу (O/F)
    double M = Никита добавит;           // Молярная масса продуктов сгорания (кг/моль)

    // Создание объекта сопла Лаваля
    LavalNozzle nozzle(gamma, R, P0, Pe, m_dot, Q, OF_ratio, M);

    // Расчет параметров
    double P_star = nozzle.calculateCriticalPressure();
    double T_star = nozzle.calculateCriticalTemperature();
    double A_star = nozzle.calculateCriticalArea();
    double A_e = nozzle.calculateExitArea(A_star);

    // Построение профиля сопла
    double L = Решим позже;             // Длина расширяющейся части сопла (м)
    int num_points = 100;       // Количество точек для построения профиля
    std::vector<double> nozzle_profile = nozzle.generateNozzleProfile(A_star, A_e, L, num_points);

    // Вывод результатов
    std::cout << "Критическое давление (P*): " << P_star << " Па" << std::endl;
    std::cout << "Критическая температура (T*): " << T_star << " К" << std::endl;
    std::cout << "Площадь критического сечения (A*): " << A_star << " м²" << std::endl;
    std::cout << "Площадь выходного сечения (A_e): " << A_e << " м²" << std::endl;
    std::cout << "Профиль сопла (диаметры в метрах):" << std::endl;

    for (int i = 0; i < num_points; ++i) {
        std::cout << "x = " << static_cast<double>(i) / (num_points - 1) * L
                  << " м, D = " << nozzle_profile[i] << " м" << std::endl;
    }

    return 0;
}
