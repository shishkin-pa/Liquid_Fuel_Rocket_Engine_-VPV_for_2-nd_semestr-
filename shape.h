#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

class LavalNozzle {
private:
    double gamma;       // Показатель адиабаты
    double R;           // Удельная газовая постоянная (Дж/(кг·К))
    double P0;          // Давление в камере сгорания (Па)
    double T0;          // Температура в камере сгорания (К)
    double Pe;          // Атмосферное давление (Па)
    double m_dot;       // Массовый расход (кг/с)
    double mu;          // Динамическая вязкость (Па·с)
    double k;           // Теплопроводность (Вт/(м·К))
    double D_throat;    // Диаметр горловины сопла (м)
    double k_parabola;  // Коэффициент для параболического профиля
    double L;           // Длина сопла (м)

public:
    // Конструктор
    LavalNozzle(double gamma, double R, double P0, double T0, double Pe, double m_dot, double mu, double k, double D_throat, double k_parabola, double L);

    // Расчет профиля сопла
    std::vector<double> calculateNozzleProfile(int num_points);

    // Численное решение уравнений
    void solveNavierStokes(std::vector<double>& pressure, std::vector<double>& temperature, std::vector<double>& velocity);

private:
    // Функция для задания параболического профиля сопла
    double nozzleProfile(double x);

    // Функция для расчёта площади сечения
    double calculateArea(double D);
};

#endif // SHAPE_H
