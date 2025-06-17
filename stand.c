#include "HX711.h"

const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN  = 3;

HX711 scale;

void setup() {
  Serial.begin(9600);
  while (!Serial) {;}
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  
  // Считаем offset (нулевое значение)
  Serial.println("Calibrating offset...");
  long sum = 0;
  const int N = 20;
  for (int i = 0; i < N; i++) {
    scale.wait_ready_timeout(1000);
    sum += scale.read();
    delay(100);
  }
  long offset = sum / N;
  scale.set_offset(offset);
  Serial.print("Offset: "); Serial.println(offset);

  // Теперь поместить известный вес, например X грамм
  Serial.println("Place known weight and press any key in Serial Monitor...");
  while (!Serial.available()) {;}
  Serial.read(); // сброс ввода

  // Считаем среднее значение с нагрузкой
  sum = 0;
  for (int i = 0; i < N; i++) {
    scale.wait_ready_timeout(1000);
    sum += scale.read();
    delay(100);
  }
  long reading_with_weight = sum / N;
  Serial.print("Reading with known weight: "); Serial.println(reading_with_weight);

  float known_weight_grams = 1000.0; // Укажите фактический вес
  float scale_factor = (reading_with_weight - offset) / known_weight_grams;
  scale.set_scale(scale_factor);
  Serial.print("Scale factor: "); Serial.println(scale_factor);

  Serial.println("Calibration done.");
}

void loop() {
  if (scale.wait_ready_timeout(1000)) {
    // Получаем значение в граммах/кг (в зависимости от масштаба)
    float weight = scale.get_units(5); // усреднение 5 измерений
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" g");
  } else {
    Serial.println("HX711 not ready");
  }
  delay(500);
}
