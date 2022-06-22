#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "chip8.c" // Your cpu core implementation
// need to use linkers to get gcc to work
chip8 myChip8;
 //game loop

int main(int argc, char **argv) 
{
  // Set up render system and register input callbacks
  setupGraphics();
  setupInput();
 
  // Initialize the Chip8 system and load the game into the memory  
  myChip8.initialize();
  myChip8.loadGame("pong");
 
  // Emulation loop
  for(;;)
  {
    // Emulate one cycle
    myChip8.emulateCycle();
 
    // If the draw flag is set, update the screen
    if(myChip8.drawFlag)
      drawGraphics();
 
    // Store key press state (Press and Release)
    //myChip8.setKeys();	
    //states are already maintained in sdl2
  }
 
  return 0;
}