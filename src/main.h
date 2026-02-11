
struct chip8
{
/* System memory map
 * 0x000 - 0x1FF - Chip 8 interpreter (contains fonts set in emu)
 * 0x050 - 0x0A0 - Used for the built in 4x5 pixels font set (0 - F)
 * 0x200 - 0xFFF - ROM
 */
unsigned char memory[4096];

// opcode = operation code
unsigned short opcode;

 //V0,V1 ... VD,VE
unsigned char V[16];                    

// I = Index register   
// pc = program counter 0x000...0xFFF
unsigned short I;
unsigned short pc;

// Rozmiar ekranu 2048 pixeli
unsigned char display[64 * 32];

// sound timery
unsigned char delay_timer;
unsigned char sound_timer;

// store the currently executing address
unsigned short stack[16];
// point to the topmost level of the stack
unsigned short sp;

// miejsce na klawiature
unsigned char key[16];
};

