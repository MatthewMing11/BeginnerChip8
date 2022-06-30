class chip8
{
    public:
        //opcodes
        unsigned short opcode;
        //total memory
        unsigned char memory[4096];
        //16 registers
        unsigned char V[16];
        //Index register and program counter
        unsigned short I;
        unsigned short pc;
        /*
        0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
        0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
        0x200-0xFFF - Program ROM and work RAM
        */
        //graphics, made of pixels with dimensions 64 by 32
        unsigned char gfx[64*32];
        //Interrupts and hardware registers(Chip-8 has none, but
        //these are instead timer registers)
        unsigned char delay_timer;
        unsigned char sound_timer;
        //stack used for Chip-8
        unsigned short stack[16];
        unsigned short sp;
        //state of key
        unsigned char key[16];
        //state of screen update
        unsigned char drawFlag;
        void initialize();
        void loadGame(char *filename);
        void emulateCycle();
};