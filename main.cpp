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

const int fps = 60;
const int ticks_per_frame = 1000 / 60;

// TODO: Get this stuff into a gameboy class maybe
cpu c;
gui screen;
gpu g(&c, &screen);
int flag = 0;


inline void execute_instruction() {
    int extended = 0;
    uint16_t opcode = c.read(c.pc);
    if (flag == 1) {
        printf("opcode = %02x, pc = %04x\n", opcode, c.pc);
    }
    // if opcode is 0xcb then we have to execute the next byte
    // from the extended instruction set
    if (opcode == 0xcb)
    {
        extended = 1;
        opcode = c.read(c.pc + 1) + 0xff;
        c.pc += 1;
    }

    int executed = inst_set[opcode].func(&c);
    if (executed == 0)
    {
        if (extended)
            printf("extended instruction\n");
        c.status();
        exit(0);
    }
    else if (executed == 1)
    {
        c.pc += inst_set[opcode].size;
        //if (opcode == 0xf3)
            //printf("size = %d\n", inst_set[opcode].size);
    }
    c.time += c.t;
    if (c.pc == 0x0463) {
        //flag = 1;
    }
    if (flag == 1)
    {
        printf("%04x %02x %s\n", c.pc, opcode,inst_set[opcode].name.c_str());
        c.status();
        getchar();
    }
}
int fc = 0;
void emulate() {
    //int max_cycles = 69905; //  frequency of gameboy / 60
    int cycles = 0;
    execute_instruction();
    if (c.pending_enable == 1 && c.read(c.pc - 1) != 0xfb)
    {
        c.interrupts_enabled = 1;
        c.pending_enable = 0;
    }
    if (c.pending_disable == 1 && c.read(c.pc - 1) != 0xf3)
    {
        c.interrupts_enabled = 0;
        c.pending_disable = 0;
    }
    g.step();
    c.update_timers();
    c.do_interrupts();
    cycles += c.t;
    fc++;
    //printf("fc = %d\n", fc);
}

int main()
{
    screen.init();
    int quit = 0;
    SDL_Event e;
    //LTimer fps;
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
        emulate();
    }
    return 0;
}
