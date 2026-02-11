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

// Tu są wszystkie zaimplementowane optcody 
//PO CO: żeby procesor wiedział co ma robić
void chip8_cycle(struct chip8* chip)
{

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
        // więcej akcji na sekunde jak będzie trzeba
        //for (int i = 0; i < 10; i++)
        chip8_cycle(&chip);

        BeginDrawing();
        ClearBackground(BLACK);

        chip8_draw(&chip);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

