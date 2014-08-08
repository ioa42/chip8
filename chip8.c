#include "chip8.h"

#include <stdio.h>

void cycle()
{
	// fetch opcode

	opcode = (mem[cp] << 8) | mem[cp + 1];

	// decode opcode
	
	switch(opcode & 0xE000)
	{
		case 0:
			switch(opcode & 0x00EE)
			{
				case 0x00E0: // CLS
					break;
				case 0x00EE: // RET
					break;
			}
			break;
		case 0x1000: // JP addr 
			break;
		case 0x2000: // CALL addr
			break;
		case 0x3000: // SE Vx, byte
			break;
		case 0x4000: // SNE Vx, byte
			break;
		case 0x5000: // SE Vx, Vy
			break;
		case 0x6000: // LD Vx, byte
			break;
		case 0x7000: // ADD Vx, byte
			break;
		case 0x8000:
			switch(opcode & 0x000E)
			{
				case 0x0000: // LD Vx, Vy
					break;
				case 0x0001: // OR Vx, Vy
					break;
				case 0x0002: // AND Vx, Vy
					break;
				case 0x0003: // XOR Vx, Vy
					break;
				case 0x0004: // ADD Vx, Vy
					break;
				case 0x0005: // SUB Vx, Vy
					break;
				case 0x0006: // SHR Vx {, Vy}
					break;
				case 0x0007: // SUBN Vx, Vy
					break;
				case 0x000E:// SHL Vx {, Vy}
					break;
			}
			break;
		case 0x9000: // SNE Vx, Vy
			break;
		case 0xA000: // LD I, addr
			break;
		case 0xB000: // JP V0, addr
			break;
		case 0xC000: // RND Vx, byte
			break;
		case 0xD000: // DRW Vx, Vy, nibble
			break;
		case 0xE000:
			switch(opcode & 0x00FF)
			{
				case 0x009E: // SKP Vx
					break;
				case 0x00A1: // SKNP Vx
					break;
			}
			break;
		case 0xF000:
			switch(opcode & 0x00EE)
			{
				case 0x0007: // LD Vx, DT
					break;
				case 0x000A: // LD Vx, K
					break;
				case 0x0015: // LD DT, Vx
					break;
				case 0x0018: // LD ST, Vx
					break;
				case 0x001E: // ADD I, Vx
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

	cp++;
}

int main(void)
{
	// initialize

	opcode = 0;
	I = 0;
	cp = 0x200;
	sp = 0;

	delay_timer = 0;
	sound_timer = 0;

	for (int i = 0; i < 64 * 32; ++i)
	{
		gfx[i] = 0;
	}

	for (int i = 0; i < MEM_SIZE; ++i)
	{
		if (i < 16)
		{
			stack[i] = 0;
			Vx[i] = 0;
		}
		mem[i] = 0;
	}

	// load the program

	// emulator main loop

	for (;;)
	{
		if (cp > 0xfff)
			break;
		cycle();
	}

	return 0;
}
