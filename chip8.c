#include "chip8.h"

void chip8::initialize()
{
     // Initialize registers and memory once
    for (int i = 0; i < 4096; i++) memory[i] =0; 
    for (int i = 0; i < 16; i++) V[i] =0; 
    I=0;
    delay_timer=0;
    sound_timer=0;
}
 
void chip8::emulateCycle()
{
  // Fetch Opcode
  opcode = memory[pc] << 8 | memory[pc + 1];
  // Decode Opcode
  
  // Execute Opcode
 
  // Update timers
}
