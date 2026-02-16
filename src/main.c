#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <include/raylib.h>
#include "main.h"

#define WIDTH 64
#define HEIGHT 32
#define SKALA 30

// przypisuje początkowe (zerowe) wartości 
void chip8_init(struct chip8 *chip)
{
  chip->pc = 0x200; // zaczynamy od x200
  chip->opcode = 0;  
  chip->I = 0;
  chip->sp = 0;

  memset(chip->memory , 0, sizeof(chip->memory));
  memset(chip->V , 0, sizeof(chip->V));
  memset(chip->display , 0, sizeof(chip->display));
  memset(chip->stack , 0, sizeof(chip->stack));
  memset(chip->key , 0, sizeof(chip->key));

  chip->delay_timer = 0;
  chip->sound_timer = 0;
}

//PO CO: Aby ładować ROM'y z terminalna
void chip8_load_rom(struct chip8 *chip, const char* filename)
{
  FILE* f = fopen(filename, "rb");
  if (!f)
  {
    printf("Otiweranie Rom nie działa :(\n");
    exit(1);
  }
  fread(&chip->memory[0x200], 1, 4096 - 0x200, f);
  fclose(f);
}


void chip8_draw(struct chip8* chip)
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (chip->display[x + y * WIDTH])
            {
                DrawRectangle(x * SKALA, y * SKALA, SKALA, SKALA, GREEN);
            }
        }
    }
}



// Tu są wszystkie zaimplementowane optcody 
//PO CO: żeby procesor wiedział co ma robić
//giga ważne Fetch->Decode->execute
// Jak zaimplementuje wszystkie to przeniose do oddzielnego pliku
void chip8_cycle(struct chip8* chip)
{
  chip->opcode = chip->memory[chip->pc] << 8 | chip->memory[chip->pc + 1];
  chip->pc += 2;

  switch(chip->opcode & 0xF000)
  {
    case 0x0000:
      if (chip->opcode & 0x00E0) //CLS clear display
      {
        memset(chip->display, 0, sizeof(chip->display));
      }
      break;
    case 0x1000:
      // 1nnn
      // Skok do adresu nnn
      chip->pc = chip->opcode & 0x0FFF;
      break;
   /* case 0x2000:
      // 2NNN
      // 1.The interpreter increments the stack pointer, 
      // 2.then puts the current PC on the top of the stack. 
      // 3.The PC is then set to nnn
      // Specyfikacja ang. ma złą kolejnośc 2->1->3
      {
        unsigned short nnn = chip->opcode & 0x0FFF;
        // 1. ProgramCounter na górze stack
        chip->stack[chip->sp] = chip->pc; 
        // 2. zwiekszam stack pointer
        chip->sp++;
        // 3. ProgramCounter = nnn
        chip->pc = nnn;
      }
      break; */
   case 0x6000:
      // 6xkk LD Vx
      // interpterer wkłada kk do rejestru Vx
      {
        unsigned char x = (chip->opcode & 0x0F00) >> 8;
        unsigned char kk = chip->opcode & 0x00FF;
        chip->V[x] = kk;
      }
      break;
    case 0x7000:
      // 7xkk ADD Vx
      // dodaje kk do rejestru Vx
      {
        unsigned char x = (chip->opcode & 0x0F00) >> 8;
        unsigned char kk = chip->opcode & 0x00FF;
        chip->V[x] += kk;
      }
      break;
   case 0xA000:
      // Annn LD I
      // ustawić I na nnn
      // unsigned short 4095 bo potrzeba 12 bitów na 0xfff
      {
        unsigned short nnn = chip->opcode & 0x0FFF;
        chip->I = nnn;
      }
      break;
   case 0xD000:
      // D xyn DRW Vx Vy
      // x , y = wartości na planszy n = wysokosc rysowania, szerokośc to 8
      // DDDD xxxx yyyy nnnn
      // 0000 xxxx 0000 0000 (>> 8) 0000 0000 0000 xxxx
      // 0000 0000 yyyy 0000 (>> 4) 0000 0000 0000 yyyy
      {
      unsigned char x = chip->V[(chip->opcode & 0x0F00) >> 8];
      unsigned char y = chip->V[(chip->opcode & 0x00F0) >> 4];
      unsigned char height = chip->opcode & 0x000F;
      chip->V[0xF] = 0; //VF kolizja
      
      for (int row = 0; row < height; row++)
      {
        unsigned char sprite = chip->memory[chip->I + row];
        
        for (int col = 0; col < 8; col++)
        {
          if(sprite & (0x80 >> col)) // 1000 0000, 0100 0000 ... 0000 0001
          {
            int px = (x + col) % WIDTH;
            int py = (y + row) % HEIGHT;
            int index = px + (py * WIDTH);

            if (chip->display[index] == 1)
              chip->V[0xF] = 1; // xorowanie

            chip->display[index] ^= 1;
          }
        }
      }
      }
      break;
     printf("Nieznany opcode: %04x \n", chip->opcode);
      break;
  }
  
}




// Główna pętla prawie jak podstawowy projekt w raylib
int main(int argc, char **argv)
{
    struct chip8 chip;

    chip8_init(&chip);
    chip8_load_rom(&chip, argv[1]);

    InitWindow(WIDTH * SKALA, HEIGHT * SKALA, "CHIP-8");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    { 
      // więcej akcji
      for (int i = 0; i < 10; i++)
        chip8_cycle(&chip);

      BeginDrawing();
      ClearBackground(BLACK);

      chip8_draw(&chip);

      EndDrawing();
    }

    CloseWindow();
    return 0;
}

