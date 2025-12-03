#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {

    // --- CONFIGURACIÓN DE PINES ---
    // LED en PB0 (pin 8 del Arduino) → salida
    DDRB |= (1 << PB0);
    PORTB &= ~(1 << PB0);   // Apagar LED

    // Botón en PD2 (pin 2 del Arduino) → entrada
    DDRD &= ~(1 << PD2);
    // NO activar pull-up → usando pull-down físico
    PORTD &= ~(1 << PD2);

    // --- VARIABLES ---
    uint8_t estadoLED = 0;
    uint8_t ultimoEstado = 0;

    while (1) {

        // Leer botón desde PIND
        uint8_t lectura = (PIND & (1 << PD2)) ? 1 : 0;

        // Detecta flanco ascendente (LOW → HIGH)
        if (lectura == 1 && ultimoEstado == 0) {

            _delay_ms(20);  // antirrebote

            if (PIND & (1 << PD2)) {    // Verificación

                // Toggle LED
                estadoLED ^= 1;

                if (estadoLED)
                    PORTB |=  (1 << PB0);   // Encender LED
                else
                    PORTB &= ~(1 << PB0);   // Apagar LED

                // Esperar a que el botón se suelte
                while (PIND & (1 << PD2)) {
                    _delay_ms(5);
                }
            }
        }

        ultimoEstado = lectura;
    }

    return 0;
}
