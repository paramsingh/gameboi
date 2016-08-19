#include <bits/stdc++.h>
using namespace std;
#include "cpu.hpp"
#include "cpu.cpp"
#include "ops.hpp"
#include "ops.cpp"

int main()
{
	cpu c;
	int count= 0;
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
		int executed = inst_set[opcode].func(&c);
		// TODO:
		// timer update
		// pc update
		// interrupt checking
		if (executed == 0)
		{
			printf("opcode in hex = %x, decimal = %d\n", opcode, opcode);
			printf("unable to execute, stopping...\n");
        	//c.status();
	    	//printf("\n");
			break;
		}
		else if(executed == 1)
		{
			//printf("Instruction name: %s\n", inst_set[opcode].name.c_str());
			c.time += inst_set[opcode].cycles;
			c.pc += inst_set[opcode].size;
            //c.status();

		}
        else if(executed == 2)
        {
            c.time += inst_set[opcode].cycles;
        }
    //    printf("\n");
        //getchar();

	}
	for (int i = 0x8000; i <= 0x9fff; i++)
		printf("%x\n", c.memory[i]);
	printf("count = %d", count);
	return 0;
}
