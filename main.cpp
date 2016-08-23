#include <bits/stdc++.h>
using namespace std;
#include "cpu.hpp"
#include "cpu.cpp"
#include "ops/ops.hpp"
#include "ops/ops.cpp"
#include "gpu.hpp"
#include "gpu.cpp"

int main()
{
	cpu c;
	gpu g(&c);
	int count= 0;
	int flag = 0;
	while (true)
	{
		uint16_t opcode = c.read(c.pc);
		// if opcode is 0xcb then we have to execute the next byte
		// from the extended instruction set
		if (c.h == 0x7f && c.l == 0xff) {
			flag = 1;
		}
		if (opcode == 0xcb)
		{
			opcode = c.read(c.pc + 1) + 0xff;
			c.pc += 1;
		}
		int executed = inst_set[opcode].func(&c);
		// TODO:
		// timer update
		// pc update
		// interrupt checking
		if (executed == 0)
		{
			printf("opcode in hex = %x, decimal = %d\n", opcode, opcode);
			printf("unable to execute, stopping...\n");
        	c.status();
	    	printf("\n");
			break;
		}
		else if(executed == 1)
		{
			c.pc += inst_set[opcode].size;
		}
        //printf("opcode = %02x, Instruction name: %s\n", opcode,inst_set[opcode].name.c_str());
        //c.status();
        //printf("pc = %x\n", c.pc);
        c.time += c.t;
        g.step();
        if (g.cnt == 100)
        	break;
        if (c.pc == 0x0055)
        {
        	/*printf("tiles\n");
        	int i = 0x8000;
        	while (i < 0x8800)
        	{
        		for (int x = 0; x < 8; x++) {
        			printf("%02x%02x ", c.memory[i], c.memory[i + 1]);
        			i += 2;
        		}
        		printf("\n");
        	}
        	printf("tilemap\n");
        	i = 0x9800;
        	while (i < 0x9c00)
        	{
        		for (int x = 0; x < 8; x++) {
        			printf("%02x%02x ", c.memory[i], c.memory[i + 1]);
        			i += 2;
        		}
        		printf("\n");
        	}*/
        }
        if (c.pc == 0x00e0)
        	break;
	}
	return 0;
}
