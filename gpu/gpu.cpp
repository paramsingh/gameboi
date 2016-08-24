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

// lcd control register
// address = 0xff40
// Bit 7 - LCD Display Enable (0=Off, 1=On)
// Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
// Bit 5 - Window Display Enable (0=Off, 1=On)
// Bit 4 - BG & Window Tile Data Select (0=8800-97FF, 1=8000-8FFF)
// Bit 3 - BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
// Bit 2 - OBJ (Sprite) Size (0=8x8, 1=8x16)
// Bit 1 - OBJ (Sprite) Display Enable (0=Off, 1=On)
// Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)
const uint16_t lcd_control = 0xff40;

gpu::gpu(cpu* m, gui* s)
{
    screen = s;
    c = m;
    memset(pixels, 0, sizeof(pixels));
    clock = 0;
    change_mode(1);
    line = 0;
    c->memory[linereg] = 0;
    cnt = 0;
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
        c->memory[statreg] &= ~((uint8_t)(1<<1));
}

void gpu::change_scanline()
{
    line++;
    c->memory[linereg]++;
    if (line > 153)
    {
        line = 0;
        c->memory[linereg] = 0;
    }
}

bool gpu::on()
{
    uint8_t status = c->read(lcd_control);
    return (status >> 7) & 1;
}

void gpu::step()
{

    // if gpu is off then do nothing
    if (!on())
        return;
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
                // TODO: Request an interrupt also here
                cnt++;
                draw_pixels();
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
            if (line == 0)
            {
                change_mode(2);
            }
            else
            {
                change_scanline();
                if (line == 0)
                    change_mode(2);
            }
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
            render_tiles();
        }
    }
}


void gpu::render_tiles()
{
    if (line >= 144)
        return;
    int flag = 0;

    // from where to get the tiles
    // this value will either be 0x8000 or 0x8800
    // based on bit 4 of the lcd control register
    uint16_t where;
    // if where is 0x8000, then tiles are indexed from 0 to 255
    // otherwise if where is 0x8800, then tiles are indexed from -128 to 127
    bool is_signed;

    uint8_t scrollx = c->read(0xff43), scrolly = c->read(0xff42);
    uint8_t windowx = c->read(0xff4b) - 7, windowy = c->read(0xff4a);

    int using_window = 0;
    // get the value in control register
    uint8_t control = c->read(lcd_control);

    if ((control >> 5) & 1)
    {
        if (line >= windowy)
            using_window = 1;
    }


    // test bit 4 of the control register
    if ((control >> 4) & 1)
    {
        where = 0x8000;
        is_signed = false;
    }
    else
    {
        where = 0x8800;
        is_signed = true;
    }

    // base address of where the tiles are mapped to pixels in memory
    // this depends on bit 3 of the control register
    // if bit is 1, then render from 0x9c00
    // else render from 0x9800
    uint16_t tilemap;
    if (!using_window)
    {
        if ((control >> 3) & 1)
            tilemap = 0x9c00;
        else
            tilemap = 0x9800;
    }
    else
    {
        if ((control >> 6) & 1)
            tilemap = 0x9c00;
        else
            tilemap = 0x9800;
    }

    // find which y out of the 256 x 256 background we're drawing
    uint8_t y = scrolly + line;
    if (using_window)
        y = line - windowy;

    // which row in the 32 x 32 matrix of mapping will contain
    // the y line
    uint16_t tile_row = ((uint8_t)(y / 8));

    for (int i = 0; i < 160; i++)
    {
        flag = 0;
        uint8_t x = i + scrollx;
        if (using_window)
        {
            if (i >= windowx)
                x = i - windowx;
        }

        // which column will contain the x line
        uint16_t tile_column = x / 8;

        uint16_t addr = tilemap + tile_row * 32 + tile_column;
        int16_t tilenum;
        if (is_signed)
        {
            tilenum = (int8_t) c->read(addr);
        }
        else
        {
            tilenum = c->read(addr);
        }

        uint16_t tile_address;
        // size of one tile = 16 bytes
        if (!is_signed)
        {
            tile_address = where + (tilenum * 16);
        }
        else
        {
            tile_address = where + ((tilenum + 128) * 16);
        }

        // need lno'th column of tilenum
        uint8_t lno = y % 8;
        uint8_t byte1 = c->read(tile_address + lno * 2);
        uint8_t byte2 = c->read(tile_address + lno * 2 + 1);

        // now byte1 and byte2 contains data for the yth line
        // bit 7 contains val for x = 0
        uint8_t req_bit = 7 - (x % 8);
        uint8_t bit1 = (byte1 >> req_bit) & 1;
        uint8_t bit2 = (byte2 >> req_bit) & 1;
        uint8_t color = (bit1 << 1) | bit2;
        pixels[i][line] = color;
    }
}

int gpu::getcolor(int id)
{
    // always black so far
    return 0;
}

void gpu::draw_pixels()
{
    for (int i = 0; i < 144; i++)
    {
        for (int j = 0; j < 160; j++) {
            int color = pixels[j][i];
            // TODO: get accurate color and draw correctly
            if (color == 0)
            {
                SDL_SetRenderDrawColor(screen->renderer, 0xff, 0xff, 0xff, 0xFF);
                SDL_RenderDrawPoint(screen->renderer, j, i);

            }
            if (color == 2)
            {
                SDL_SetRenderDrawColor(screen->renderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderDrawPoint(screen->renderer, j, i);
            }
        }
    }
    SDL_RenderPresent(screen->renderer);
}
