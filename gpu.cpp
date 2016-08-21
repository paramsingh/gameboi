// This file contains the code for emulating the GPU
// of the Gameboy

// We will eventually use gtkmm to create a
// canvas of 160 * 144 on which we'll
// draw

// Right now have to make do with a 2d array of numbers

// this address contains the line being scanned currently
const uint16_t linereg = 0xff44;

// this address contains the status of the lcd
// bits 0 & 1 contain the mode
// 00 - hblank
// 01 - vblank
// 10 - searching sprites
// 11 - accessing vram
const uint16_t statreg = 0xff41;

gpu::gpu(cpu* m)
{
    c = m;
    memset(pixels, 0, sizeof(pixels));
    clock = 0;
    change_mode(0);
    line = 0;
    c->memory[linereg] = 0;
}

void gpu::change_mode(int id)
{
    mode = id;
    int bit1 = id & 1;
    int bit2 = (id >> 1) & 1;

    if (bit1)
        c->memory[statreg] |= 1;
    else
        c->memory[statreg] &= ~((uint8_t)1);

    if (bit2)
        c->memory[statreg] |= (1 << 1);
    else
        c->memory[statreg] &= ~((uint8_t)1);
}

void gpu::change_scanline()
{
    line++;
    c->memory[linereg] = 0;
    if (line > 153)
    {
        line = 0;
        c->memory[linereg] = 0;
    }
}

void gpu::step()
{
    // add the number of cycles passed to the clock
    clock += c->t;
    if (mode == 0) // horizontal blank mode
    {
        if (clock >= 204) // horizontal blank mode over
        {
            clock = 0;
            change_scanline();
            if (line == 143)
            {
                change_mode(1);
                // TODO: draw the screen
            }
            else
            {
                change_mode(2);
            }
        }
    }
    // vertical blank mode
    // goes on for 4560 cycles
    // i.e 456 cycles each for 10 scanlines from 144 to 153
    else if (mode == 1)
    {
        if (clock >= 456)
        {
            clock = 0;
            change_scanline();
            if (line == 0)
                change_mode(2);
        }
    }
    // scanline accessing sprites etc
    // goes on for 80 cycles
    else if (mode == 2)
    {
        if (clock >= 80)
        {
            clock = 0;
            change_mode(3);
        }
    }
    // scanline accessing vram
    // after this is done the entire line can be written to the
    // pixels array for drawing later
    else if (mode == 3)
    {
        if (clock >= 172)
        {
            clock = 0;
            change_mode(0);
            // TODO: write a scanline to the framebuffer;
        }
    }
}
