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
  char ch = opcode >>12;
  switch(ch){
    case 0:
      clear();//clears the display, whatever that means
      break;
    case 1:
      pc = opcode & 0x0FFF;
      break;
    case 2:
      sp++;//check if incrementing stack pointer is correct
      pc = opcode & 0x0FFF;
      break;
    case 3:
      if(V[(opcode & 0x0F00) >> 8]==(opcode & 0x00FF)){
        pc+=2;
      }
      break;
    case 4:
      if(V[(opcode & 0x0F00) >> 8]!=(opcode & 0x00FF)){
        pc+=2;
      }
      break;
    case 5:
      if((opcode & 0x000F)==0){
        if(V[(opcode & 0x0F00) >> 8]==V[(opcode & 0x00F0)>>4]){
          pc+=2;
        } 
      }
      break;
    case 6:
      V[(opcode & 0x0F00) >> 8]=(opcode & 0x00FF);
      break;
    case 7:
      V[(opcode & 0x0F00) >> 8]+=(opcode & 0x00FF);
      break;
    case 8:
      char ones =opcode & 0x000F;
      switch(ones){
        case 0:
          V[(opcode & 0x0F00) >> 8]=V[(opcode & 0x00F0)>>4];
          break;
        case 1:
          V[(opcode & 0x0F00) >> 8]=(V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0)>>4]);
          break;
        case 2:
          V[(opcode & 0x0F00) >> 8]=(V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0)>>4]);
          break;
        case 3:
          V[(opcode & 0x0F00) >> 8]=(V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0)>>4]);
          break;
        case 4://can probably shorthand the carryover using ternary operator
          if((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0)>>4])>255){
            V[15]=1;
          }
          else{
            V[15]=0;
          }
          V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0)>>4]
          break;
        case 5:
          if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0)>>4]){
            V[15]=1;
          }
          else{
            V[15]=0;
          }
          V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0)>>4]
          break;
        case 6:
          if(V[(opcode & 0x0F00) >> 8]%2){
            V[15]=1;
          }
          else{
            V[15]=0;
          }
          V[(opcode & 0x0F00) >> 8]/=2;
          break;
        case 7:
          if(V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0)>>4]){
            V[15]=1;
          }
          else{
            V[15]=0;
          }
          V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0)>>4]-V[(opcode & 0x0F00) >> 8];
          break;
        case 14:
          if(V[(opcode & 0x0F00) >> 8]>>7){
            V[15]=1;
          }
          else{
            V[15]=0;
          }
          V[(opcode & 0x0F00) >> 8]*=2;
          break;
      }
      break;//unnecessary break but standard for all cases in outer switch
    case 9:
      if((opcode & 0x000F)==0){
        if(V[(opcode & 0x0F00) >> 8]!=V[(opcode & 0x00F0)>>4]){
          pc+=2;
        } 
      }
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
