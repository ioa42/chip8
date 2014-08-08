#ifndef CHIP8_H_
# define CHIP8_H_

# include <stdlib.h>
# include <stdint.h>

# define MEM_SIZE 4096
# define VX_SIZE 16
# define STACK_SIZE 16
# define GFX_SIZE 2048
# define KEY_SIZE 16

/*
 *  The 4Kio of RAM
 *  0x0 - 0x1FF is reserved for the original emulator
 *  Usually, programs begins at 0x200 (others at 0x600)
 */

unsigned char mem[MEM_SIZE];

/*
 * The 16 registers called Vx (V0, V1, V2 ... Ve, Vf)
 * The Vf register is reserved (used as a flag by some instr)
 */

unsigned short opcode;

unsigned char Vx[VX_SIZE];
unsigned short I;
unsigned short cp;

unsigned char gfx[GFX_SIZE];

unsigned char delay_timer;
unsigned char sound_timer;

unsigned short stack[STACK_SIZE];
unsigned short sp;

unsigned char key[KEY_SIZE];

#endif // !CHIP_8_
