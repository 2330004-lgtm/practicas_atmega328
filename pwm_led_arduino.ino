
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void setup() {
  // --- Configurar PD6 (OC0A) como salida ---
  DDRD |= (1 << PD6);  // D6 en Arduino


  TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
  // Prescaler CS02:0 = 010 -> clk/8
  TCCR0B = (1 << CS01);

  // Inicializa OCR0A a 0 (LED apagado)
  OCR0A = 0;

  

  ADMUX = (1 << REFS0);


  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

  // Lectura dummy para estabilizar ADC
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
}

void loop() {
  // Variables estáticas para no reasignar cada ciclo
  static uint8_t current_duty = 0;
  uint16_t adc_value;
  uint8_t target_duty;

  // --- Iniciar conversión ADC ---
  ADCSRA |= (1 << ADSC);
  // Esperar a que termine
  while (ADCSRA & (1 << ADSC));
  // Leer ADC (registro ADC es ADCW)
  adc_value = ADC; // valor 0..1023

  // Mapear 0..1023 -> 0..255 (simple: shift right 2)
  target_duty = (uint8_t)(adc_value >> 2);

  // --- Suavizado simple: mover current_duty 1 paso hacia target_duty ---
  if (current_duty < target_duty) {
    current_duty++;
  } else if (current_duty > target_duty) {
    current_duty--;
  }
  // Escribir al OCR0A (duty cycle)
  OCR0A = current_duty;

  
  _delay_ms(2); 
}
