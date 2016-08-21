int cmp(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t n;
    c->t = 4;
    if (opcode == 0xBf)
        n = c->a;
    else if (opcode == 0xb8)
        n = c->b;
    else if (opcode == 0xb9)
        n = c->c;
    else if (opcode == 0xba)
        n = c->d;
    else if (opcode == 0xbb)
        n = c->e;
    else if (opcode == 0xbc)
        n = c->h;
    else if (opcode == 0xbd)
        n = c->l;
    else if (opcode == 0xbe)
    {
        uint16_t addr = (((uint16_t)c->h) << 8) | (c -> l);
        n = c->read(addr);
        c->t = 8;
    }
    else if (opcode == 0xfe)
    {
        n = c->read(c->pc + 1);
        c->t = 8;
    }
    if (c->a == n)
        c->zero = 1;
    else
        c->zero = 0;
    c->subtract = 1;
    if (((c -> a) & 0xf) < (n & 0xf))
        c->half_carry = 1;
    else
        c->half_carry = 0;
    if (c->a < n)
        c->carry = 1;
    else
        c->carry = 0;
    return 1;
}

int xorop(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    c->t = 4;
    uint8_t data;
    if (opcode == 0xaf)
        data = c->a;
    else if (opcode == 0xa8)
        data = c->b;
    else if (opcode == 0xa9)
        data = c->c;
    else if (opcode == 0xaa)
        data = c->d;
    else if (opcode == 0xab)
        data = c->e;
    else if (opcode == 0xac)
        data = c->h;
    else if (opcode == 0xad)
        data = c->l;
    else if (opcode == 0xae)
    {
        c->t = 8;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        data = c->read(addr);
    }
    else if (opcode == 0xee)
    {
        c->t = 8;
        data = c->read(c->pc + 1);
    }

    // xor a and data and put it in a
    c->a ^= data;
    c->zero = (c->a == 0);
    c->carry = c->half_carry = c->subtract = 0;
    return 1;
}

int bit7(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x7f)
        reg = &(c->a);
    else if (opcode == 0x78)
        reg = &(c->b);
    else if (opcode == 0x79)
        reg = &(c->c);
    else if (opcode == 0x7a)
        reg = &(c->d);
    else if (opcode == 0x7b)
        reg = &(c->e);
    else if (opcode == 0x7c)
        reg = &(c->h);
    else if (opcode == 0x7d)
        reg = &(c->l);
    else if (opcode == 0x7e)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        reg = c->memory + addr;
    }
    c->zero = ((*reg) & (1 << 7)) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}


int inc(cpu *c)
{
    //flags affected
    /* Z - Set if result is zero.
    N - Reset.
    H - Set if carry from bit 3.
    C - Not affected*/
    uint8_t opcode = c->read(c->pc);
    uint8_t *reg;
    c->t = 4;
    if (opcode == 0x3c)
        reg = &(c -> a);
    else if (opcode == 0x04)
        reg = &(c -> b);
    else if (opcode == 0x0c)
        reg = &(c -> c);
    else if (opcode == 0x14)
        reg = &(c -> d);
    else if (opcode == 0x1c)
        reg = &(c -> e);
    else if (opcode == 0x24)
        reg = &(c -> h);
    else if (opcode == 0x2c)
        reg = &(c -> l);
    else if (opcode == 0x34)
    {
        c->t = 12;
        uint16_t addr = (((uint16_t)c->h)<<8) | (c->l);
        reg = c->memory + addr;
    }
    // now have to increase value in reg by 1
    // if the lower bits are all one, then half carry will be set
    if ((*reg) & 0xf == 1)
        c->half_carry = 1;
    else
        c->half_carry = 0;
    (*reg)++;
    if (*reg == 0)
        c->zero = 1;
    else
        c->zero = 0;
    c->subtract = 0;
    return 1;
}

int inc_pair(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *hi, *lo;
    c->t = 8;
    if (opcode == 0x03)
        // increment the bc pair
        hi = &(c->b), lo = &(c->c);
    else if (opcode == 0x13)
        // increment the de pair
        hi = &(c->d), lo = &(c->e);
    else if (opcode == 0x23)
        // increment the hl pair
        hi = &(c->h), lo = &(c->l);
    else if (opcode == 0x33) {
        // increment the stack pointer
        c->sp++;
        return 1;
    }

    uint16_t val = (((uint16_t)(*hi)) << 8) | (*lo);
    val++;
    *hi = val >> 8;
    *lo = val;
    return 1;
}

int dec(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *reg;
    c->t = 4;
    if (opcode == 0x3d)
        reg = &(c -> a);
    else if (opcode == 0x05)
        reg = &(c -> b);
    else if (opcode == 0x0D)
        reg = &(c -> c);
    else if (opcode == 0x15)
        reg = &(c -> d);
    else if (opcode == 0x1D)
        reg = &(c -> e);
    else if (opcode == 0x25)
        reg = &(c -> h);
    else if (opcode == 0x2d)
        reg = &(c -> l);
    else if (opcode == 0x35)
    {
        c->t = 12;
        uint16_t addr = (((uint16_t)c->h)<<8) | (c->l);
        reg = c->memory + addr;
    }

    // now have to decrease value in reg by 1
    // if the lower bits are all zero, then half carry will be set
    if ((*reg) & 0xf == 0)
        c->half_carry = 1;
    else
        c->half_carry = 0;
    (*reg)--;
    if (*reg == 0)
        c->zero = 1;
    else
        c->zero = 0;
    c->subtract = 1;
    return 1;
}

int rl(cpu* c)
{
    // NOTE: extended instruction
    // rotates through carry;
    uint16_t opcode = c->read(c->pc);
    uint8_t *reg;
    c->t = 8;
    if (opcode == 0x17)
        reg = &(c->a);
    else if (opcode == 0x10)
        reg = &(c->b);
    else if (opcode == 0x11)
        reg = &(c->c);
    else if (opcode == 0x12)
        reg = &(c->d);
    else if (opcode == 0x13)
        reg = &(c->e);
    else if (opcode == 0x14)
        reg = &(c->h);
    else if (opcode == 0x15)
        reg = &(c->l);
    else if (opcode == 0x16)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        reg = c->memory + addr;
    }

    // save the shifted value in a 16 bit integer
    uint16_t x = (((uint16_t)(*reg))<< 1);
    // if the carry flag was 1, set the 0th bit
    // in the shifted value
    if (c->carry == 1)
        x |= 1;
    // set carry according to 8th bit in shifted value
    c->carry = (x >> 8) & 1;
    // set register to shifted value
    *reg = x;
    // set the flags affected
    c->zero = (*reg == 0);
    c->subtract = 0;
    c->half_carry = 0;
    return 1;
}
