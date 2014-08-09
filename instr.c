# include "chip8.h"

void cycle()
{
	// fetch opcode

	opcode = (mem[pc] << 8) | mem[pc + 1];

	// decode opcode

	unsigned short vx = 0;
	unsigned short vy = 0;

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
			if (Vx[(opcode & 0x0f00) >> 8] == (opcode & 0x00ff))
				pc += 2;
			break;
		case 0x4000: // SNE Vx, byte
			if (Vx[(opcode & 0x0f00) >> 8] != (opcode & 0x00ff))
				pc += 2;
			break;
		case 0x5000: // SE Vx, Vy
			if (Vx[(opcode & 0x0f00) >> 8] == Vx[(opcode & 0x00f0) >> 4])
				pc += 2;
			break;
		case 0x6000: // LD Vx, byte
			Vx[(opcode & 0x0f00) >> 8] = opcode & 0x00ff;
			break;
		case 0x7000: // ADD Vx, byte
			Vx[(opcode & 0x0f00) >> 8] &= (opcode & 0x00ff);
			break;
		case 0x8000:
			switch(opcode & 0x000f)
			{
				case 0x0000: // LD Vx, Vy
					Vx[(opcode & 0x0f00) >> 8] = Vx[(opcode & 0x00f0) >> 4];
					break;
				case 0x0001: // OR Vx, Vy
					Vx[(opcode & 0x0f00) >> 8] |= Vx[(opcode & 0x00f0) >> 4];
					break;
				case 0x0002: // AND Vx, Vy
					Vx[(opcode & 0x0f00) >> 8] &= Vx[(opcode & 0x00f0) >> 4];
					break;
				case 0x0003: // XOR Vx, Vy
					Vx[(opcode & 0x0f00) >> 8] ^= Vx[(opcode & 0x00f0) >> 4];
					break;
				case 0x0004: // ADD Vx, Vy
					/* TODO : check for carry */
					Vx[(opcode & 0x0f00) >> 8] += Vx[(opcode & 0x00f0) >> 4];
					break;
				case 0x0005: // SUB Vx, Vy
					vx = Vx[(opcode & 0x0f00) >> 8];
					vy = Vx[(opcode & 0x00f0) >> 4];

					/* if Vx > Vy then VF = 1 otherwise 0 */		
					Vx[0xe] = vx > vy;

					Vx[(opcode & 0x0f00) >> 8] -= Vx[(opcode & 0x00f0) >> 4];
					break;
				case 0x0006: // SHR Vx {, Vy}
					Vx[0xe] = Vx[(opcode & 0x0f00) >> 8] & 0x01;
					Vx[(opcode & 0x0f00) >> 8] >>= 1;
					break;
				case 0x0007: // SUBN Vx, Vy
					vx = Vx[(opcode & 0x0f00) >> 8];
					vy = Vx[(opcode & 0x00f0) >> 4];

					/* if Vy > Vx then VF = 1 otherwise 0 */		
					Vx[0xe] = vy > vx;

					Vx[(opcode & 0x0f00) >> 8] = vy - vx;
					break;
				case 0x000e:// SHL Vx {, Vy}
					Vx[0xe] = Vx[(opcode & 0x0f00) >> 8] & 0x80;
					Vx[(opcode & 0x0f00) >> 8] <<= 1;
					break;
			}
			break;
		case 0x9000: // SNE Vx, Vy
			if (Vx[(opcode & 0x0f00) >> 8] != Vx[(opcode & 0x00f0) >> 4])
				pc += 2;
			break;
		case 0xa000: // LD I, addr
			I = opcode & 0x0fff;			
			break;
		case 0xb000: // JP V0, addr
			pc = (opcode & 0x0fff) + Vx[0];
			break;
		case 0xc000: // RND Vx, byte
			/* TODO vx variable is used as a container for the random value generated for RND instruction */
			vx = 42;
			Vx[(opcode & 0x0f00) >> 8] = vx & (opcode & 0x00ff);
			break;
		case 0xd000: // DRW Vx, Vy, nibble
			/* TODO */
			is_drawing = true;
			break;
		case 0xe000:
			switch(opcode & 0x00ff)
			{
				case 0x009e: // SKP Vx
					if (key[(opcode & 0x0f00) >> 8] & Vx[(opcode & 0x0f00) >> 8])
						pc += 2;
					break;
				case 0x00a1: // SKNP Vx
					if (!(key[(opcode & 0x0f00) >> 8] & Vx[(opcode & 0x0f00) >> 8]))
						pc += 2;
					break;
			}
			break;
		case 0xf000:
			switch(opcode & 0x00ee)
			{
				case 0x0007: // LD Vx, DT
					Vx[(opcode & 0x0f00) >> 8] = delay_timer;
					break;
				case 0x000a: // LD Vx, K
					/* vx is used as iterator there */
					for(vx = 0; vx < 0xf; ++vx)
					{
						if (key[vx])
							break;
					}
					Vx[(opcode & 0x0f00) >> 8] = vx;
					break;
				case 0x0015: // LD DT, Vx
					delay_timer = Vx[(opcode & 0x0f00) >> 8];
					break;
				case 0x0018: // LD ST, Vx
					sound_timer = Vx[(opcode & 0x0f00) >> 8];
					break;
				case 0x001e: // ADD I, Vx
					I += Vx[(opcode & 0x0f00) >> 8];		
					break;
				case 0x0029: // LD F, Vx
					/* TODO */
					break;
				case 0x0033: // LD B, Vx
					/* TODO */
					break;
				case 0x0055: // LD [I], Vx
					vx = (opcode & 0x0f00);
					for (vy = 0; vy < vx; vy++)
						mem[I + vy] = Vx[vy];
					break;
				case 0x0065: // LD Vx, [I]
					vx = (opcode & 0x0f00) > 8;
					for (vy = 0; vy < vx; vy++)
						Vx[vy] = mem[I + vy];
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
