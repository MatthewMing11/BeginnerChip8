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
  char ch = opcode >>12;//alternative is opcode & 0xf000,which is much cleaner
  switch(ch){
    case 0:
      clear();//clears the display, whatever that means
      break;
    case 1:
      pc = opcode& 0x0FFF;
      break;
    case 2:
      sp++;
      pc = opcode & 0x0FFF;
      break;
    case 3://check how to find digits within opcode
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      I= opcode & 0x0FFF;
      break;
    case 11:
      pc=(opcode & 0x0FFF)+V[0];
      break;
    case 12:
      break;
    case 13:
      break;
    case 14:
      break;
    case 15:
      break;
  }
  // Execute Opcode
 
  // Update timers
}
