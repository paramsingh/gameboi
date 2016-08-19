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
		uint8_t opcode = c.read(c.pc);
		cout << inst_set[opcode].name << endl;
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
			c.status();
			getchar();
		}
	}
	return 0;
}
