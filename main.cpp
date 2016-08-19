#include <bits/stdc++.h>
using namespace std;
#include "cpu.hpp"
#include "cpu.cpp"
#include "ops.hpp"
#include "ops.cpp"

int main()
{
	cpu c;
	while (true)
	{
		uint16_t opcode = c.read(c.pc);
		// if opcode is 0xcb then we have to execute the next byte
		// from the extended instruction set
		if (opcode == 0xcb)
		{
			opcode = c.read(c.pc + 1) + 0xff;
			c.pc += 1;
		}
		printf("opcode = %x\n", opcode);
		printf("Instruction name: %s\n", inst_set[opcode].name.c_str());
		bool executed = inst_set[opcode].func(&c);
		// TODO:
		// timer update
		// pc update
		// interrupt checking
		if (!executed)
		{
			printf("unable to execute, stopping...\n");
			break;
		}
		else
		{
			c.time += inst_set[opcode].cycles;
			c.pc += inst_set[opcode].size;
		}
		c.status();
		printf("\n");
	}
	return 0;
}
