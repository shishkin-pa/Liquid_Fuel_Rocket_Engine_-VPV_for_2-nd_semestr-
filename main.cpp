#include <iostream>
#include "shape.h"

int main() {
    // Входные данные
    double gamma = 1.4;         // Показатель адиабаты для воздуха
    double R = 287.0;           // Удельная газовая постоянная для воздуха (Дж/(кг·К))
    double P0 = Никита добавит;         // Давление в камере сгорания (Па)
    double T0 = Никита добавит;         // Температура в камере сгорания (К)
    double Pe = 1.0e5;          // Атмосферное давление (Па)
    double m_dot = Никита добавит;         // Массовый расход (кг/с)
    double L = Никита добавит??;             // Длина расширяющейся части сопла (м)
    int num_points = 500;       // Количество точек для построения профиля

    // Создание объекта сопла Лаваля
    LavalNozzle nozzle(gamma, R, P0, T0, Pe, m_dot);

    // Расчет параметров
    double P_star = nozzle.calculateCriticalPressure();
    double T_star = nozzle.calculateCriticalTemperature();
    double A_star = nozzle.calculateCriticalArea();
    double A_e = nozzle.calculateExitArea(A_star);

    // Построение профиля сопла
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