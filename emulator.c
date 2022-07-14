#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "chip8.cpp" // Your cpu core implementation
// need to use linkers to get gcc to work
chip8 myChip8;
 //game loop

//used https://stackoverflow.com/questions/20579658/how-to-draw-pixels-in-sdl-2-0 to draw pixels
void setupGraphics(SDL_Window **window, SDL_Renderer **renderer){
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(640,320,0,window,renderer);
  SDL_RenderSetScale(*renderer,8,8);
  SDL_SetRenderDrawColor(*renderer,0,0,0,255);
  SDL_RenderClear(*renderer);
}
void drawGraphics(SDL_Window **window,SDL_Renderer **renderer){
  for(int row=0;row<32;row++){
    for(int col=0;col<64;col++){
      if(myChip8.gfx[row*64+col]){
        SDL_SetRenderDrawColor(*renderer,255,255,255,255);
        SDL_RenderDrawPoint(*renderer,col,row);  
      }else{
        SDL_SetRenderDrawColor(*renderer,0,0,0,255);
        SDL_RenderDrawPoint(*renderer,col,row);
      }
    }
  }
  SDL_RenderPresent(*renderer);
  myChip8.drawFlag=0;
}
int main(int argc, char **argv) 
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event windowEvent;//for closing the window

  // Set up render system and register input callbacks
  setupGraphics(&window,&renderer);
  //setupInput();
  // For some reason this code worked when in main but didn't in the helper and I have to use double pointers to fix it
  // Dammit i realize that i was passing by value instead of reference
  // SDL_Init(SDL_INIT_VIDEO);
  // SDL_CreateWindowAndRenderer(640,320,0,&window,&renderer);
  // SDL_RenderSetScale(renderer,4,4);
  // SDL_SetRenderDrawColor(renderer,0,0,0,255);
  // SDL_RenderClear(renderer);
  // Initialize the Chip8 system and load the game into the memory  
  myChip8.initialize();
  char * file="test_opcode.ch8";
  myChip8.loadGame(file);
 char* print;
  // Emulation loop
  while(1)
  {
    // for(int i=0;i<32;i++){
    //   for(int j=0;j<64;j++){
    //     printf("%d",myChip8.gfx[j+i*64]);
    //   }
    //   printf("\n");
    // }
    // printf("\n");
    // Emulate one cycle
    myChip8.emulateCycle();
 
    // If the draw flag is set, update the screen
    if(myChip8.drawFlag){
      // printf("%s\n","Inside the draw");
      drawGraphics(&window,&renderer);
      // printf("%s\n","Outside the draw");
    }
    // Store key press state (Press and Release)
    //myChip8.setKeys();	
    //states are already maintained in sdl2
    if(SDL_PollEvent(&windowEvent)){
      if(SDL_QUIT == windowEvent.type){
        break;
      }
    }
  }
  // SDL_DestroyRenderer(renderer);
  // SDL_DestroyWindow(window);
  // SDL_Quit();
  return 0;
}