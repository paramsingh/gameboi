#include <bits/stdc++.h>
using namespace std;
#include "cpu.hpp"
#include "cpu.cpp"
#include "ops/ops.hpp"
#include "ops/ops.cpp"

int main()
{
	cpu c;
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

			c.time += c.t;
			c.pc += inst_set[opcode].size;


		}
        else if(executed == 2)
        {
            c.time += c.t;
        }
        printf("Instruction name: %s\n", inst_set[opcode].name.c_str());
        c.status();
        //printf("pc = %x\n", c.pc);
        if (flag){
        	//getchar();
        }

	}
	return 0;
}
