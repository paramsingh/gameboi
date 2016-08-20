// This file contains all the load operations

// Function for all load type operations
int ld(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    switch (opcode)
    {
        case 0x31:
        {
            // this loads the stack pointer with 2 byte immediate value
            // assuming that lower bits are given first
            uint16_t low = c->read(c->pc + 1);
            uint16_t high = c->read(c->pc + 2);
            low = (high << 8) | low;
            c->sp = low;
            break;
        }
        case 0x21:
        {
            // LD HL nn
            c->l = c->read(c ->pc + 1);
            c->h = c->read(c->pc + 2);
            break;
        }
        case 0x32:
        {
            // load value from a into address at hl and
            // decrement hl
            uint16_t addr = (((uint16_t)c->h) << 8) | (c -> l);
            c->write(addr, c->a);
            addr--;
            c->l = addr;
            c->h = addr >> 8;
            break;
        }
        case 0x0e:
        {
            //load 8-bit immediate value in register C
            c->c = c->read(c->pc+1);
            break;
        }
        case 0xe2:
        {
            //Put A into address $FF00 + register C
            c->write(0xFF00 + (uint16_t)c->c, c->a);
            break;

        }
        case 0x77:
        {
            //Put Accumulator value in memory addr in HL pair
            uint16_t val = c->l | (((uint16_t)c->h) << 8);
            c->write(val, c->a);
            break;
        }
        case 0xe0:
        {
            //Put A into memory address $FF00+n (n is immediate 8 bit value)
            uint8_t n = c->read(c->pc + 1);
            c->write(0xff00 + (uint16_t)n, c->a);
            break;
        }
        case 0x11:
        {
            // Put 16 bit immediate value at DE pair
            c->e = c->read(c->pc+1);
            c->d = c->read(c->pc+2);
            break;
        }
        case 0x1a:
        {
            uint16_t addr = (((uint16_t)c->d) << 8) | c->e;
            c->a = c->read(addr);
            break;
        }
        case 0x22:
        {
            // load a into (HL) and increment HL
            uint16_t addr = (((uint16_t)c->h)<<8) | c->l;
            c->write(addr, c->a);
            addr++;
            c->h = (addr >> 8);
            c->l = addr;
            break;
        }
        default:
            return 0;
    }
    return 1;
}

// 8 bit loads

int load_rtoa(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *from;
    if (opcode == 0x7f)
        from = &(c->a);
    else if (opcode == 0x78)
        from = &(c->b);
    else if (opcode == 0x79)
        from = &(c->c);
    else if (opcode == 0x7A)
        from = &(c->d);
    else if (opcode == 0x7B)
        from = &(c->e);
    else if (opcode == 0x7C)
        from = &(c->h);
    else if (opcode == 0x7D)
        from = &(c->l);
    else if (opcode == 0x7E)
    {
        uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
        from = c->memory + addr;
    }
    c->a = *from;
    return 1;
}

int load_rtob(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *from;
    if (opcode == 0x40)
        from = &(c->b);
    else if (opcode == 0x41)
        from = &(c->c);
    else if (opcode == 0x42)
        from = &(c->d);
    else if (opcode == 0x43)
        from = &(c->e);
    else if (opcode == 0x44)
        from = &(c->h);
    else if (opcode == 0x45)
        from = &(c->l);
    else if (opcode == 0x46)
    {
        uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
        from = c->memory + addr;
    }
    else if (opcode == 0x47)
        from = &(c->a);
    c->b = *from;
    return 1;
}

int load_rtoc(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *from;
    if (opcode == 0x48)
        from = &(c->b);
    else if (opcode == 0x49)
        from = &(c->c);
    else if (opcode == 0x4a)
        from = &(c->d);
    else if (opcode == 0x4b)
        from = &(c->e);
    else if (opcode == 0x4c)
        from = &(c->h);
    else if (opcode == 0x4d)
        from = &(c->l);
    else if (opcode == 0x4e)
    {
        uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
        from = c->memory + addr;
    }
    else if (opcode == 0x4f)
        from = &(c -> a);
    c->c = *from;
    return 1;
}

int load_rtod(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *from;
    if (opcode == 0x50)
        from = &(c->b);
    else if (opcode == 0x51)
        from = &(c->c);
    else if (opcode == 0x52)
        from = &(c->d);
    else if (opcode == 0x53)
        from = &(c->e);
    else if (opcode == 0x54)
        from = &(c->h);
    else if (opcode == 0x55)
        from = &(c->l);
    else if (opcode == 0x56)
    {
        uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
        from = c->memory + addr;
    }
    else if (opcode == 0x57)
        from = &(c -> a);
    c->d = *from;
    return 1;
}

int load_rtoe(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *from;
    if (opcode == 0x58)
        from = &(c->b);
    else if (opcode == 0x59)
        from = &(c->c);
    else if (opcode == 0x5a)
        from = &(c->d);
    else if (opcode == 0x5b)
        from = &(c->e);
    else if (opcode == 0x5c)
        from = &(c->h);
    else if (opcode == 0x5d)
        from = &(c->l);
    else if (opcode == 0x5e)
    {
        uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
        from = c->memory + addr;
    }
    else if (opcode == 0x5f)
        from = &(c -> a);
    c->e = *from;
    return 1;
}

int load_rtoh(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *from;
    if (opcode == 0x60)
        from = &(c->b);
    else if (opcode == 0x61)
        from = &(c->c);
    else if (opcode == 0x62)
        from = &(c->d);
    else if (opcode == 0x63)
        from = &(c->e);
    else if (opcode == 0x64)
        from = &(c->h);
    else if (opcode == 0x65)
        from = &(c->l);
    else if (opcode == 0x66)
    {
        uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
        from = c->memory + addr;
    }
    else if (opcode == 0x67)
        from = &(c -> a);
    c->h = *from;
    return 1;
}

int load_rtol(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *from;
    if (opcode == 0x68)
        from = &(c->b);
    else if (opcode == 0x69)
        from = &(c->c);
    else if (opcode == 0x6A)
        from = &(c->d);
    else if (opcode == 0x6B)
        from = &(c->e);
    else if (opcode == 0x6C)
        from = &(c->h);
    else if (opcode == 0x6D)
        from = &(c->l);
    else if (opcode == 0x6E)
    {
        uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
        from = c->memory + addr;
    }
    else if (opcode == 0x6F)
        from = &(c -> a);
    c->l = *from;
    return 1;
}

int load_rtomem(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *from;
    if (opcode == 0x70)
        from = &(c->b);
    else if (opcode == 0x71)
        from = &(c->c);
    else if (opcode == 0x72)
        from = &(c->d);
    else if (opcode == 0x73)
        from = &(c->e);
    else if (opcode == 0x74)
        from = &(c->h);
    else if (opcode == 0x75)
        from = &(c->l);
    else if (opcode == 0x76)
    {
        uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
        from = c->memory + addr;
    }
    else if (opcode == 0x77)
        from = &(c -> a);
    uint16_t addr = (((uint16_t)(c->h)) << 8) |(c -> l);
    c->write(addr, *from);
    return 1;
}

int load_atomem(cpu* c)
{
    // write a to the memory address in register pairs
    // or immediate values according to opcodes
    // does not affect any flags

    // note: opcode 0x77 LD (HL) A is reimplemented here
    // but the one used is present in the function load_rtomem
    // the one here is not used in the emulation process.
    uint8_t opcode = c->read(c->pc);
    uint16_t hi, lo;
    if (opcode == 0x02)
        hi = c->b, lo = c->c;
    else if (opcode == 0x12)
        hi = c->d, lo = c->e;
    else if (opcode == 0x77)
        hi = c->h, lo = c->l;
    else if (opcode == 0xea)
        lo = c->read(c->pc + 1), lo = c->read(c->pc + 2);

    // get the address from the high and low bytes.
    uint16_t addr = (hi << 8) | lo;
    // write the accumulator to the address
    c->write(addr, c->a);
    return 1;
}


int load_memtor(cpu* c)
{
    // loads immediate value to register
    // no flags are updated
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    if (opcode == 0x06)
        reg = &(c->b);
    else if (opcode == 0x0e)
        reg = &(c->c);
    else if (opcode == 0x16)
        reg = &(c->d);
    else if (opcode == 0x1e)
        reg = &(c->e);
    else if (opcode == 0x26)
        reg = &(c->h);
    else if (opcode == 0x2e)
        reg = &(c->l);
    else if (opcode == 0x3e)
        reg = &(c->a);

    *reg = c->read(c->pc + 1);
    return 1;
}
