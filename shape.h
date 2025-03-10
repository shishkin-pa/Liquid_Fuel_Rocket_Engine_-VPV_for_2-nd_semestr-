#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

class LavalNozzle {
private:
    double gamma;       // Показатель адиабаты (отношение теплоёмкостей)
    double R;           // Удельная газовая постоянная (Дж/(кг·К))
    double P0;          // Давление в камере сгорания (Па)
    double T0;          // Температура в камере сгорания (К)
    double Pe;          // Атмосферное давление на выходе (Па)
    double m_dot;       // Массовый расход газа (кг/с)
    double mu;          // Динамическая вязкость газа (Па·с)
    double k;           // Теплопроводность газа (Вт/(м·К))
    double D_throat;    // Диаметр горловины сопла (м)
    double k_parabola;  // Коэффициент для параболического профиля сопла
    double L;           // Длина сопла (м)

public:
    // Конструктор: инициализирует все параметры сопла и газа
    LavalNozzle(double gamma, double R, double P0, double T0, double Pe, double m_dot, double mu, double k, double D_throat, double k_parabola, double L);

    // Расчёт профиля сопла: возвращает диаметр сопла в каждой точке
    std::vector<double> calculateNozzleProfile(int num_points);

    // Численное решение уравнений: рассчитывает давление, температуру и скорость газа
    void solveNavierStokes(std::vector<double>& pressure, std::vector<double>& temperature, std::vector<double>& velocity);

private:
    // Функция для задания параболического профиля сопла
    double nozzleProfile(double x);

    // Функция для расчёта площади сечения сопла
    double calculateArea(double D);
};

#endif // SHAPE_H
