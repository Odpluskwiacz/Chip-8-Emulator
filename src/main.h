#ifndef MAIN_H
// Przygotowanie do
// Architektury projektu: Unity Build
#define MAIN_H


#include <stdint.h>


struct chip8
{
/* System memory map
 * 0x000 - 0x1FF - Chip 8 interpreter (contains fonts set in emu)
 * 0x050 - 0x0A0 - Used for the built in 4x5 pixels font set (0 - F)
 * 0x200 - 0xFFF - ROM
 */
uint8_t memory[4096];

// opcode = operation code
uint16_t opcode;

 //V0,V1 ... VD,VE,VF
uint8_t V[16];                    

// I = Index register   
// pc = program counter 0x000...0xFFF
uint16_t I;
uint16_t pc;

// Rozmiar ekranu 2048 pixeli
uint8_t display[64 * 32];

// sound timery
uint8_t delay_timer;
uint8_t sound_timer;

// store the currently executing address
uint16_t stack[16];
// point to the topmost level of the stack
uint16_t sp;

// miejsce na klawiature
uint8_t key[16];
};

#endif
