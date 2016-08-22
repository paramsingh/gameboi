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
        if (false) {
        	printf("tile data #0\n");
        	for (int i = 0x8000; i <= 0x87ff; i++)
        		printf("%x\n", c.memory[i]);
        	printf("tile data #1\n");
        	for (int i = 0x8800; i <= 0x8fff; i++)
        		printf("%x\n", c.memory[i]);
        	break;
        }
        if (c.pc == 0x006a){
        	//flag = 1;
        	break;
        }
	}
	g.print_pixels();
	return 0;
}
