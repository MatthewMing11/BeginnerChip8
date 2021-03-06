#include "chip8.h"
#include <stdio.h>
#include <math.h>
#include <sys/stat.h>
//contemplating if i shold switch to opcodes to make everything uniform
//probably?
unsigned char chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};



void chip8::initialize()
{
  pc     = 0x200;  // Program counter starts at 0x200
  opcode = 0;      // Reset current opcode
  sp     = 0;      // Reset stack pointer
 
     // Initialize registers and memory once
    for (int i = 0; i < 64*32; i++) gfx[i] =0; // Clear display
    for (int i = 0; i < 4096; i++) memory[i] =0; // Clear memory
    for (int i = 0; i < 16; i++) V[i] =0; // Clear registers V0-VF
    for (int i = 0; i < 16; i++) stack[i] =0; // Clear stack
    I=0;// Reset index register
    // Load fontset
    for(int i = 0; i < 80; ++i){
      memory[i+0x050] = chip8_fontset[i];
    }	
    // Reset timers
    delay_timer=0;
    sound_timer=0;

}

void chip8::loadGame(char *filename){
  FILE *buf=fopen(filename,"rb");
  // int c;
  // printf("wtf is here\n");
  // while(1) {
  //     c = fgetc(buf);
  //     if( feof(buf) ) {
  //        break;
  //     }
  //     printf("%x\n", c & 0xff);
  //  }
  //  fclose(buf);
  struct stat st;
  stat(filename, &st);
  int size=st.st_size;
  char buffer[size];
  fread(buffer,sizeof(char),size,buf);
  for(int i=0;i<size;i++){
    memory[i+0x200]=buffer[i];
  }
  // for(int i=0;i<size;i++){
  //   printf("%c\n",memory[i+0x200]);
  // }
}

void chip8::emulateCycle()
{
  // printf("%d\n",pc);
  // Fetch Opcode
  opcode = memory[pc] << 8 | memory[pc + 1];
  pc+=2;
  // printf("%x\n",opcode);
  // for(int i=0;i<16;i++){ 
  //   printf("V[%d]=%d\n",i,V[i]);
  // }
  // printf("\n");
  // Decode Opcode
  char ch = opcode >>12;
  switch(ch){//might later change from using first letter switch to opcode switch 
    case 0:{
      unsigned char loctet=opcode & 0xFF;
      switch(loctet){
        case 0xE0:
          for (int i = 0; i < 64*32; i++) gfx[i] =0; // Clear display
          drawFlag=1;
          break;
        case 0xEE:
          pc=stack[sp];
          sp--;
          break;
      }
      break;
    }
    case 1:
      pc = opcode & 0x0FFF;
      break;
    case 2:
      stack[++sp]=pc;
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
    case 8:{
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
          V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0)>>4];
          break;
        case 5:
          if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0)>>4]){
            V[15]=1;
          }
          else{
            V[15]=0;
          }
          V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0)>>4];
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
          if((V[(opcode & 0x0F00) >> 8]>>7)%2){//don't know if there are values over 255 but just to make sure
            V[15]=1;
          }
          else{
            V[15]=0;
          }
          V[(opcode & 0x0F00) >> 8]*=2;
          break;
      }
      break;//unnecessary break but standard for all cases in outer switch
    }
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
      V[(opcode & 0x0F00) >> 8]=((rand() % 256) & (opcode & 0x00FF));
      break;
    case 13://CHECK THIS
    //not too sure about display stuff
    {
      char n= opcode & 0x000F;
      int X=V[(opcode & 0x0F00) >> 8]%64;
      int Y=V[(opcode & 0x00F0) >> 4]%32;
      char place =7;//location of bit
      int start =Y*64+X;
      int i=0;
      int col = 0;//collision variable
      int old=0;
      while(i<n){
        if(place>-1 && X+(7-place)<64 && Y+i < 32){
          old=gfx[start+(7-place)+64*i];
          gfx[start+(7-place)+64*i]=(memory[I+i]/(1<<place))%2;
          // printf("%d\n",start+(7-place)+64*i);
          // printf("%d\n",gfx[start+(7-place)+64*i]);
          if(gfx[start+(7-place)+64*i]<old){
            col=1;
          }
          place--;
        }
        else{
          i++;
          place=7;
        }
      }
      V[15]=col;
      drawFlag=1;
      break;
    }
    case 14://not sure if check means wait, will see later
    {
    unsigned char loctet=opcode & 0xFF;
    int keypress=0;
    //got help from https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html
    SDL_Event event;
    if(loctet==0x9E){
      //checking key in down position
      while(!keypress){ 
          if(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
              if(V[(opcode & 0x0F00) >> 8]==event.key.keysym.scancode){
                pc+=2;
                break;
              }
            }
          }
      }
      break;
    }
    if(loctet==0xA1){
      //checking key in up position
      while(!keypress){ 
          if(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYUP){
              if(V[(opcode & 0x0F00) >> 8]==event.key.keysym.scancode){
                pc+=2;
                break;
              }
            }
          }
      }
      break;
    }
    break;//unnecessary break but standard for all cases in outer switch
    }
    case 15:
    {
    unsigned char loctet=opcode & 0xFF;
    switch(loctet){
      case 0x07:
        V[(opcode & 0x0F00) >> 8]=delay_timer;
        break;
      case 0x0A:// got help from https://stackoverflow.com/questions/4468028/sdl-delay-until-keyboard-input
      {
        int keypress=0;
        int key;
        SDL_Event event;
        while(!keypress){
          if(SDL_WaitEvent(&event)){
            if(event.type == SDL_KEYDOWN){
              if (key >3 && key <10 && key >29 && key <40){//check if key is valid within current system
                keypress = 1;
                key=event.key.keysym.scancode;
                break;
              }
            }
          }
        }
        V[(opcode & 0x0F00) >> 8]=key;
        break;
      }
      case 0x15:
        delay_timer=V[(opcode & 0x0F00) >> 8];
        break;
      case 0x18:
        sound_timer=V[(opcode & 0x0F00) >> 8];
        break;
      case 0x1E:
        I+=V[(opcode & 0x0F00) >> 8];
        break;
      case 0x29:
      //don't really understand display and sprites yet
        I=V[(opcode & 0x0F00) >> 8];
        break; 
      case 0x33:
      {
        int bcd =V[(opcode & 0x0F00) >> 8];
        memory[I]=bcd/100;
        memory[I+1]=(bcd%100)/10;
        memory[I+2]=bcd%10;
        break;
      }
      case 0x55:
      {
        int max=((opcode & 0x0F00) >> 8)+1;
        for(int i=0;i<max;i++){
          memory[I+i]=V[i];
        }
        break; 
      }
      case 0x65:
      {
        int max=((opcode & 0x0F00) >> 8)+1;
        for(int i=0;i<max;i++){
          V[i]=memory[I+i];
        }
        break;
      }
    }
      break;
    }
  }
  // Execute Opcode
 
  // Update timers
  if(delay_timer>0){
    delay_timer--;
  }
  if(sound_timer>0){
    sound_timer--;
  }
}
