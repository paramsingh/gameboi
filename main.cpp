#include <bits/stdc++.h>
#include <SDL2/SDL.h>
using namespace std;
#include "cpu/cpu.hpp"
#include "cpu/cpu.cpp"
#include "ops/ops.hpp"
#include "ops/ops.cpp"
#include "gui/gui.hpp"
#include "gui/gui.cpp"
#include "gpu/gpu.hpp"
#include "gpu/gpu.cpp"

// TODO: Get this stuff into a gameboy class maybe
cpu c;
gui screen;
gpu g(&c, &screen);

void step() {
    int extended = 0;
    uint16_t opcode = c.read(c.pc);
    // if opcode is 0xcb then we have to execute the next byte
    // from the extended instruction set
    if (opcode == 0xcb)
    {
        extended = 1;
        opcode = c.read(c.pc + 1) + 0xff;
        c.pc += 1;
    }
    int executed = inst_set[opcode].func(&c);
    // TODO:
    // timer update
    // interrupt checking
    if (executed == 0)
    {
        if (extended == 1)
            printf("from extended instruction set\n");
        printf("opcode in hex = %x, decimal = %d\n", opcode, opcode);
        printf("unable to execute, stopping...\n");
        c.status();
        printf("\n");
        exit(0);
    }
    else if(executed == 1)
    {
        c.pc += inst_set[opcode].size;
    }
    printf("%04x %02x %s\n", c.pc, opcode,inst_set[opcode].name.c_str());
    // c.status();
    c.time += c.t;
    g.step();
}

int main()
{
    screen.init();
    int count= 0;
    int flag = 1;
    int quit = 0;
    SDL_Event e;
    while (!quit)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            //User requests quit
            if(e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }
        step();
    }
    return 0;
}
