#include "chip8.h"

#include <stdio.h>
#include <string.h>

void cycle()
{
	// fetch opcode

	opcode = (mem[pc] << 8) | mem[pc + 1];

	// decode opcode
	
	switch(opcode & 0xf000)
	{
		case 0:
			switch(opcode & 0x00ff)
			{
				case 0x00e0: // CLS
					for (int i = 0; i < GFX_SIZE; ++i)
						gfx[i] = 0;
					break;
				case 0x00ee: // RET
					pc = stack[sp--];
					break;
			}
			break;
		case 0x1000: // JP addr
			pc = opcode & 0x0fff;
			break;
		case 0x2000: // CALL addr
			sp++;
			stack[sp] = pc;
			pc = opcode & 0x0fff;
			break;
		case 0x3000: // SE Vx, byte
			if (Vx[(opcode & 0x0f00) >> 4] == (opcode & 0x00ff))
				pc += 2;
			break;
		case 0x4000: // SNE Vx, byte
			if (Vx[(opcode & 0x0f00) >> 4] != (opcode & 0x00f))
				pc += 2;
			break;
		case 0x5000: // SE Vx, Vy
			if (Vx[(opcode & 0x0f00) >> 4] == Vx[(opcode & 0x00f0) >> 2])
				pc += 2;
			break;
		case 0x6000: // LD Vx, byte
			Vx[(opcode & 0x0f00) >> 4] = opcode & 0x00ff;
			break;
		case 0x7000: // ADD Vx, byte
			Vx[(opcode & 0x0f00) >> 4] &= (opcode & 0x00ff);
			break;
		case 0x8000:
			switch(opcode & 0x000f)
			{
				case 0x0000: // LD Vx, Vy
					Vx[(opcode & 0x0f00) >> 4] = Vx[(opcode & 0x00f0) >> 2];
					break;
				case 0x0001: // OR Vx, Vy
					Vx[(opcode & 0x0f00) >> 4] |= Vx[(opcode & 0x00f0) >> 2];
					break;
				case 0x0002: // AND Vx, Vy
					Vx[(opcode & 0x0f00) >> 4] &= Vx[(opcode & 0x00f0) >> 2];
					break;
				case 0x0003: // XOR Vx, Vy
					Vx[(opcode & 0x0f00) >> 4] ^= Vx[(opcode & 0x00f0) >> 2];
					break;
				case 0x0004: // ADD Vx, Vy
				 	/* TODO : check for carry */
					Vx[(
					break;
				case 0x0005: // SUB Vx, Vy
					break;
				case 0x0006: // SHR Vx {, Vy}
					break;
				case 0x0007: // SUBN Vx, Vy
					break;
				case 0x000e:// SHL Vx {, Vy}
					break;
			}
			break;
		case 0x9000: // SNE Vx, Vy
			break;
		case 0xa000: // LD I, addr
			break;
		case 0xb000: // JP V0, addr
			break;
		case 0xc000: // RND Vx, byte
			break;
		case 0xd000: // DRW Vx, Vy, nibble
			break;
		case 0xe000:
			switch(opcode & 0x00ff)
			{
				case 0x009e: // SKP Vx
					break;
				case 0x00a1: // SKNP Vx
					break;
			}
			break;
		case 0xf000:
			switch(opcode & 0x00ee)
			{
				case 0x0007: // LD Vx, DT
					break;
				case 0x000a: // LD Vx, K
					break;
				case 0x0015: // LD DT, Vx
					break;
				case 0x0018: // LD ST, Vx
					break;
				case 0x001e: // ADD I, Vx
					break;
				case 0x0029: // LD F, Vx
					break;
				case 0x0033: // LD B, Vx
					break;
				case 0x0055: // LD [I], Vx
					break;
				case 0x0065: // LD Vx, [I]
					break;
			}
			break;
	}

	// update timer

	if (delay_timer > 0)
		delay_timer--;

	if (sound_timer > 0)
	{
		if (sound_timer == 1)
		{
			printf("BIP!\n");
		}
		sound_timer--;
	}

	pc++;
}

void initialize()
{
	opcode = 0;
	I = 0;
	pc = 0x200;
	sp = 0;

	delay_timer = 0;
	sound_timer = 0;

	for (int i = 0; i < 64 * 32; ++i)
		gfx[i] = 0;

	for (int i = 0; i < MEM_SIZE; ++i)
	{
		if (i < 16)
		{
			stack[i] = 0;
			Vx[i] = 0;
		}
		mem[i] = 0;
	}	
}

void load_program(char* path)
{
	FILE* fileno = NULL;

	if ((fileno = fopen(path, "r")) == NULL)
	{
		perror("fopen");
		exit(1);
	}

	fread(mem + 0x200, sizeof(unsigned char), 0xfff - 0x200, fileno);	

	for (int i = 0x200; i < 0xfff; ++i)
	{
		printf("%c", mem[i]);
	}

	if (fclose(fileno) != 0)
	{
		perror("fclose");
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	// initialize
	initialize();

	// load the program
	load_program(argv[argc - 1]);

	// emulator's main loop

	for (; pc <= 0xfff;)
	{
		cycle();

	}

	return 0;
}
