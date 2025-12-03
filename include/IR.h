//#include <avr/io.h>
//#include <avr/interrupt.h>
//
//// led switchen
//// TCCR0A ^= (1 << COM0A0);
//
//void initializeIRIO()
//{
//    // PD6 als output
//    DDRD |= (1 << DDD6);
//
//    // Timer0 instellingen --> CTC, ORC0A als TOP
//    OCR0A = 210;
//    TCCR0A = (1 << WGM01);
//    TCCR0B = (1 << CS00);
//
//    // Timer1 instellingen --> CTC, OCR1A als TOP prescalen /64
//    OCR1A = 500;
//    TCCR1B |= (1 << WGM12) | (1 << CS10) | (1 << CS11);
//    TIMSK1 |= (1 << OCIE1A);
//
//}
//
//ISR(TIMER1_COMPA_vect)
//{
//    TCCR0A &= ~(1 << COM0A0);
//}
//
//// voor demo proof of concept --> een bitje sturen (zonder enige check of protocol)
//void sendBit()
//{
//    TCNT1 = 0;
//    TCCR0A |= (1 << COM0A0);
//}
//
//typedef enum
//{
//    Field = 0b11010111,
//    Game  = 0b01001011
//} Adress;
//
//// mogelijk later gebruiken --> als we dit protocol gebruiken
//uint32_t encode8BitsForTransmission(uint8_t data, Adress adress)
//{
//    uint32_t encodedMessage = 0;
//
//    uint32_t addr8 = ((uint32_t)adress) & 0xFF;
//    uint32_t data8 = ((uint32_t)data) & 0xFF;
//
//    encodedMessage |= (addr8 << 24);
//    encodedMessage |= ((~addr8 & 0xFF) << 16);
//    encodedMessage |= (data8 << 8);
//    encodedMessage |= (~data8 & 0xFF);
//
//    return encodedMessage;
//}
//
