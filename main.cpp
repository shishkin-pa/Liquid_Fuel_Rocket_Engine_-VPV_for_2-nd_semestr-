#include <iostream>
#include "substance.h"
#include "engine_calculations.h"

int main() {
    try {
        const double P_atm = 100000; // Атмосферное давление
        double P_chamber, r_chamber, F, concentration;

        // Функция для ввода с проверкой (лямбда-функция)
        auto inputPositive = [](const std::string& prompt) {
            double value;
            while (true) {
                std::cout << prompt;
                std::cin >> value;
                if (value > 0) return value;
                std::cout << "Ошибка: значение должно быть положительным!\n";
            }
        };

        std::cout << "Расчет параметров ракетного двигателя\n";
        std::cout << "=====================================\n\n";
        
        // Ввод параметров
        P_chamber = inputPositive("Давление в камере [Па]: ");
        r_chamber = inputPositive("Радиус камеры [м]: ");
        F = inputPositive("Тяга [Н]: ");
        
        std::cout << "Концентрация перекиси [0-1]: ";
        std::cin >> concentration;
        if (concentration < 0 || concentration > 1) {
            throw std::invalid_argument("Концентрация должна быть от 0 до 1");
        }

        // Создание объектов веществ (демонстрирует композицию)
        Substance H2O_liquid("H2O_parametri.txt");
        Substance H2O_gas("Steam_parametri.txt");
        Substance O2("O2_parametri.txt");
        Substance H2O2("H2O2_parametri.txt");

        // Вычисления
        double nu_conc = EngineCalculations::calculateNuConcentration(concentration, H2O2, H2O_liquid);
        double max_T = EngineCalculations::calculateMaxTemperature(nu_conc, H2O2, H2O_liquid, H2O_gas, O2);
        double end_T = EngineCalculations::calculateFinalTemperature(P_atm, P_chamber, H2O_gas, O2, max_T, nu_conc);
        
        // Комплексный расчет сопла
        auto nozzle = EngineCalculations::calculateNozzleParameters(
            H2O_gas, O2, nu_conc, max_T, end_T, 
            r_chamber, P_chamber, P_atm, F);

        // Вывод результатов
        std::cout << "\nРезультаты расчетов:\n";
        std::cout << "--------------------\n";
        std::cout << "Молярная концентрация: " << nu_conc << "\n";
        std::cout << "Макс. температура в камере: " << max_T << " K\n";
        std::cout << "Температура на выходе: " << end_T << " K\n";
        std::cout << "Скорость истечения: " << nozzle.exitVelocity << " м/с\n";
        std::cout << "Радиус выходного сечения: " << nozzle.exitRadius << " м\n";
        std::cout << "Критический радиус: " << nozzle.criticalRadius << " м\n";
        std::cout << "Массовый расход: " << nozzle.massFlowRate << " кг/с\n";
    }
    catch (const std::exception& e) {
        std::cerr << "\nОшибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}