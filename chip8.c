#include "chip8.h"

void initialize()
{
	opcode = 0;
	I = 0;
	pc = 0x200;
	sp = 0;

	delay_timer = 0;
	sound_timer = 0;

	is_drawing = false;

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

	/*for (int i = 0x200; i < 0xfff; ++i)
	{
		printf("%c", mem[i]);
	}*/

	if (fclose(fileno) != 0)
	{
		perror("fclose");
		exit(1);
	}
}

void draw()
{
	printf("Is drawing something somewhere /o/\n");
}

int main(int argc, char* argv[])
{
	printf("Starting Chip8 Emulator by Nicolas Barray\n");

	printf("Initialize ... ");
	// initialize
	initialize();
	printf("OK\n");

	printf("Load program ... ");
	// load the program
	load_program(argv[argc - 1]);
	printf("OK\n");

	// emulator's main loop

	printf("Executing program now.\n");
	for (; pc <= 0xfff;)
	{
		cycle();

		if (is_drawing)
			draw();	
	}
	
	printf("End of program, bye !\n");
	return 0;
}
