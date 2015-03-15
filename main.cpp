/*
* CPPFile1.cpp
*
* Created: 07.02.2015 15:00:36
*  Author: Benjamin
*/
#define F_CPU 22118400 //redefine! Defined in compiler too but always doublecheck

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>

//#include "Cube.h"
#include "Display.h"
#include "USART.h"
#include "Random.h" //progmem random generator
#include "Temperatur.h"
#include "Input.h"

//global objects to access in interupts
//Cube cube;
Input input;

void initInterrupt();
int main()
{
    USART::init();//setup the usart0
    initInterrupt();

    //Test LED
    DDRB |= (1 << DDB3); //PB3

    char buff[20] = {0};

    Display::init();
    Display::set_cursor(0, 0);
    Display::write_string("Cube V2");
    Display::set_cursor(1, 0);
    Display::write_string("I AM HERE!");
    Display::set_cursor(2, 0);
    Display::write_string("Guess it's works");

    _delay_ms(5000);
    Display::clear();

    uint8_t i = 0;
    while(true)
    {
        Display::changeDisplayLight(input.getIncDelta()*2);
        i++;
        if(i == 255)
            i = 0;
        _delay_ms(20);

        //_delay_ms(500);
        PORTB ^= (1 << PB3); //toggle the testled
        if(i == 125)
        {
            Temperatur::get(buff);
            Display::set_cursor(0, 0);
            Display::write_string("Temperatur: ");
            Display::set_cursor(1, 0);
            Display::write_string(buff);
            Display::write_data('C');
        }

        itoa(input.getIncDelta(), buff, 10);
        Display::set_cursor(2, 10);
        Display::write_string(buff);

        if(input.isPressed())
        {
            Display::set_cursor(2, 0);
            Display::write_string("Pressed!");
        }
        else
        {
            Display::set_cursor(2, 0);
            Display::write_string("        ");
        }
    }
}

void initInterrupt()
{
    TCCR2A = 0x00;
    TCCR2A |= (1 << WGM21); //CTC mode
    TCCR2B = 0x00;
    TCCR2B |= (1 << CS22); // 64 prescaler
    TIMSK2 |= (1 << OCIE2A); //enable compare interrupt
    //some compare counter ~ 1ms
    OCR2A = (uint8_t)(F_CPU / 64.0 * 1e-3 - 0.5);
    sei();
}

ISR(TIMER2_COMPA_vect)
{
    input.update();
}