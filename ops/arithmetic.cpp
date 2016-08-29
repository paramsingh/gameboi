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
    uint8_t val = c->a - n;
    c->zero = (val == 0);
    c->subtract = 1;
    if (((c->a - n) & 0xf) > ((c->a) & 0xf))
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

int orop(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    c->t = 4;
    uint8_t data;
    if (opcode == 0xb7)
        data = c->a;
    else if (opcode == 0xb0)
        data = c->b;
    else if (opcode == 0xb1)
        data = c->c;
    else if (opcode == 0xb2)
        data = c->d;
    else if (opcode == 0xb3)
        data = c->e;
    else if (opcode == 0xb4)
        data = c->h;
    else if (opcode == 0xb5)
        data = c->l;
    else if (opcode == 0xb6)
    {
        c->t = 8;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        data = c->read(addr);
    }
    else if (opcode == 0xf6)
    {
        c->t = 8;
        data = c->read(c->pc + 1);
    }

    // or a and data and put it in a
    c->a |= data;
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
        uint8_t val = c->read(addr);
        c->zero = (val & (1 << 7)) == 0;
        c->half_carry = 1;
        c->subtract = 0;
        return 1;
    }
    c->zero = ((*reg) & (1 << 7)) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}

int andop(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    c->t = 4;
    uint8_t data;
    if (opcode == 0xa7)
        data = c->a;
    else if (opcode == 0xa0)
        data = c->b;
    else if (opcode == 0xa1)
        data = c->c;
    else if (opcode == 0xa2)
        data = c->d;
    else if (opcode == 0xa3)
        data = c->e;
    else if (opcode == 0xa4)
        data = c->h;
    else if (opcode == 0xa5)
        data = c->l;
    else if (opcode == 0xa6)
    {
        c->t = 8;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        data = c->read(addr);
    }
    else if (opcode == 0xe6)
    {
        c->t = 8;
        data = c->read(c->pc + 1);
    }

    // and a and data and put it in a
    c->a &= data;
    c->zero = (c->a == 0);
    c->half_carry = 1;
    c->carry = c->subtract = 0;
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
    if (((*reg) & 0xf) == 1)
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

int dec_pair(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *hi, *lo;
    c->t = 8;
    if (opcode == 0x0b)
        // decrement the bc pair
        hi = &(c->b), lo = &(c->c);
    else if (opcode == 0x1b)
        // decrement the de pair
        hi = &(c->d), lo = &(c->e);
    else if (opcode == 0x2b)
        // decrement the hl pair
        hi = &(c->h), lo = &(c->l);
    else if (opcode == 0x3b) {
        // decrement the stack pointer
        c->sp--;
        return 1;
    }

    uint16_t val = (((uint16_t)(*hi)) << 8) | (*lo);
    val--;
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
    if (((*reg) & 0xf) == 0)
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

    int temp = c->carry;
    c->carry = ((*reg) >> 7) & 1;
    *reg = (*reg) << 1 | temp;
    c->zero = *reg == 0;

    return 1;
}

int sub(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t data;
    c->t = 4;
    if (opcode == 0x97)
        data = c->a;
    else if (opcode == 0x90)
        data = c->b;
    else if (opcode == 0x91)
        data = c->c;
    else if (opcode == 0x92)
        data = c->d;
    else if (opcode == 0x93)
        data = c->e;
    else if (opcode == 0x94)
        data = c->h;
    else if (opcode == 0x95)
        data = c->l;
    else if (opcode == 0x96)
    {
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        data = c->read(addr);
        c->t = 8;
    }
    else if (opcode == 0xd6)
    {
        data = c->read(c->pc + 1);
        c->t = 8;
    }

    c->zero = (c->a == data);
    c->subtract = 1;
    c->carry = (c->a < data);
    c->half_carry = (c->a & 0xf) < (data & 0xf);
    c->a = c->a - data;
    return 1;
}

int add(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t data;
    c->t = 4;
    if (opcode == 0x87)
        data = c->a;
    else if (opcode == 0x80)
        data = c->b;
    else if (opcode == 0x81)
        data = c->c;
    else if (opcode == 0x82)
        data = c->d;
    else if (opcode == 0x83)
        data = c->e;
    else if (opcode == 0x84)
        data = c->h;
    else if (opcode == 0x85)
        data = c->l;
    else if (opcode == 0x86)
    {
        c->t = 8;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        data = c->read(addr);
    }
    else if (opcode == 0xc6)
    {
        c->t = 8;
        data = c->read(c->pc);
    }

    uint16_t val = data; // implicit conversion of data to 16 bits
    val += c->a;

    // set the flags
    c->zero = (val & 0xff) == 0;
    c->carry = (val >> 8) & 1;
    uint8_t half = (c->a & 0xf) + (data & 0xf);
    c->half_carry = (half >> 4) & 1;
    c->subtract = 0;

    c->a += data;
    return 1;
}

int cpl(cpu* c)
{
    c->t = 4;
    c->a = ~(c->a);
    c->subtract = c->half_carry = 1;
    return 1;
}

int swapop(cpu* c)
{
    c->t = 8;
    uint8_t opcode = c->read(c->pc);
    c->subtract = c->half_carry = c->carry = 0;
    uint8_t* reg;
    if (opcode == 0x37)
        reg = &(c->a);
    else if (opcode == 0x30)
        reg = &(c->b);
    else if (opcode == 0x31)
        reg = &(c->c);
    else if (opcode == 0x32)
        reg = &(c->d);
    else if (opcode == 0x33)
        reg = &(c->e);
    else if (opcode == 0x34)
        reg = &(c->h);
    else if (opcode == 0x35)
        reg = &(c->l);
    else if (opcode == 0x36)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t data = c->read(addr);
        uint8_t val = (data >> 4);
        val |= (data << 4);
        c->write(addr, val);
        c->zero = (val == 0);
        return 1;
    }

    uint8_t val = (*reg) >> 4;
    val |= ((*reg) << 4);
    *reg = val;
    c->zero = (val == 0);
    return 1;
}

int add_pair(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    c->t = 8;
    uint16_t hi, lo;
    if (opcode == 0x09)
        hi = c->b, lo = c->c;
    else if (opcode == 0x19)
        hi = c->d, lo = c->e;
    else if (opcode == 0x29)
        hi = c->h, lo = c->l;
    else if (opcode == 0x39)
        hi = (c->sp) >> 8, lo = (c->sp) & 0xff;

    uint16_t val = (hi << 8) | lo;
    uint16_t hl = (((uint16_t)(c->h)) << 8) | c->l;
    c->subtract = 0;
    c->half_carry = (((val & 0xfff) + (hl & 0xfff)) >> 12) & 1; // set if carry from bit 11
    c->carry = (((uint32_t)val + hl) >> 16) & 1; // set if carry from bit 15
    uint16_t ans = val + hl;
    c->h = ans >> 8;
    c->l = ans & 0xff;
    return 1;
}

int res0(cpu* c)
{
    // reset bit 0 in register r
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x80)
        reg = &(c->b);
    else if (opcode == 0x81)
        reg = &(c->c);
    else if (opcode == 0x82)
        reg = &(c->d);
    else if (opcode == 0x83)
        reg = &(c->e);
    else if (opcode == 0x84)
        reg = &(c->h);
    else if (opcode == 0x85)
        reg = &(c->l);
    else if (opcode == 0x86)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        val ^= 1;
        c->write(addr, val);
        return 1;
    }
    else if (opcode == 0x87)
        reg = &(c->a);
    (*reg) ^= 1;
    return 1;
}

int sla(cpu* c)
{
    // extended instruction
    // shift left register into carry
    // lowest bit is set to 0
    uint8_t opcode = c->read(c->pc);
    c->t = 8;
    uint8_t* reg;
    if (opcode == 0x20)
        reg = &(c->b);
    else if (opcode == 0x21)
        reg = &(c->c);
    else if (opcode == 0x22)
        reg = &(c->d);
    else if (opcode == 0x23)
        reg = &(c->e);
    else if (opcode == 0x24)
        reg = &(c->h);
    else if (opcode == 0x25)
        reg = &(c->l);
    else if (opcode == 0x26)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->carry = (val >> 7) & 1;
        val <<= 1;
        c->write(addr, val);
        c->zero = (val == 0);
        c->half_carry = c->subtract = 0;
        return 1;
    }
    else if (opcode == 0x27)
        reg = &(c->a);

    c->half_carry = c->subtract = 0;
    uint8_t val = (*reg) << 1;
    c->zero = (val == 0);
    c->carry = ((*reg) >> 7) & 1;
    *reg = val;
    return 1;
}

int bit2(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x57)
        reg = &(c->a);
    else if (opcode == 0x50)
        reg = &(c->b);
    else if (opcode == 0x51)
        reg = &(c->c);
    else if (opcode == 0x52)
        reg = &(c->d);
    else if (opcode == 0x53)
        reg = &(c->e);
    else if (opcode == 0x54)
        reg = &(c->h);
    else if (opcode == 0x55)
        reg = &(c->l);
    else if (opcode == 0x56)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->zero = (val & (1 << 2)) == 0;
        c->half_carry = 1;
        c->subtract = 0;
        return 1;
    }
    c->zero = ((*reg) & (1 << 2)) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}

int bit3(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x5f)
        reg = &(c->a);
    else if (opcode == 0x58)
        reg = &(c->b);
    else if (opcode == 0x59)
        reg = &(c->c);
    else if (opcode == 0x5a)
        reg = &(c->d);
    else if (opcode == 0x5b)
        reg = &(c->e);
    else if (opcode == 0x5c)
        reg = &(c->h);
    else if (opcode == 0x5d)
        reg = &(c->l);
    else if (opcode == 0x5e)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->zero = (val & (1 << 3)) == 0;
        c->half_carry = 1;
        c->subtract = 0;
        return 1;
    }
    c->zero = ((*reg) & (1 << 3)) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}

int bit4(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x67)
        reg = &(c->a);
    else if (opcode == 0x60)
        reg = &(c->b);
    else if (opcode == 0x61)
        reg = &(c->c);
    else if (opcode == 0x62)
        reg = &(c->d);
    else if (opcode == 0x63)
        reg = &(c->e);
    else if (opcode == 0x64)
        reg = &(c->h);
    else if (opcode == 0x65)
        reg = &(c->l);
    else if (opcode == 0x66)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->zero = (val & (1 << 4)) == 0;
        c->half_carry = 1;
        c->subtract = 0;
        return 1;
    }
    c->zero = ((*reg) & (1 << 4)) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}

int bit5(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x6f)
        reg = &(c->a);
    else if (opcode == 0x68)
        reg = &(c->b);
    else if (opcode == 0x69)
        reg = &(c->c);
    else if (opcode == 0x6a)
        reg = &(c->d);
    else if (opcode == 0x6b)
        reg = &(c->e);
    else if (opcode == 0x6c)
        reg = &(c->h);
    else if (opcode == 0x6d)
        reg = &(c->l);
    else if (opcode == 0x6e)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->zero = (val & (1 << 5)) == 0;
        c->half_carry = 1;
        c->subtract = 0;
        return 1;
    }
    c->zero = ((*reg) & (1 << 5)) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}

int bit6(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x77)
        reg = &(c->a);
    else if (opcode == 0x70)
        reg = &(c->b);
    else if (opcode == 0x71)
        reg = &(c->c);
    else if (opcode == 0x72)
        reg = &(c->d);
    else if (opcode == 0x73)
        reg = &(c->e);
    else if (opcode == 0x74)
        reg = &(c->h);
    else if (opcode == 0x75)
        reg = &(c->l);
    else if (opcode == 0x76)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->zero = (val & (1 << 6)) == 0;
        c->half_carry = 1;
        c->subtract = 0;
        return 1;
    }
    c->zero = ((*reg) & (1 << 6)) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}

int bit0(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x47)
        reg = &(c->a);
    else if (opcode == 0x40)
        reg = &(c->b);
    else if (opcode == 0x41)
        reg = &(c->c);
    else if (opcode == 0x42)
        reg = &(c->d);
    else if (opcode == 0x43)
        reg = &(c->e);
    else if (opcode == 0x44)
        reg = &(c->h);
    else if (opcode == 0x45)
        reg = &(c->l);
    else if (opcode == 0x46)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->zero = (val & 1) == 0;
        c->half_carry = 1;
        c->subtract = 0;
        return 1;
    }
    c->zero = ((*reg) & 1) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}

int bit1(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t* reg;
    c->t = 8;
    if (opcode == 0x4f)
        reg = &(c->a);
    else if (opcode == 0x48)
        reg = &(c->b);
    else if (opcode == 0x49)
        reg = &(c->c);
    else if (opcode == 0x4a)
        reg = &(c->d);
    else if (opcode == 0x4b)
        reg = &(c->e);
    else if (opcode == 0x4c)
        reg = &(c->h);
    else if (opcode == 0x4d)
        reg = &(c->l);
    else if (opcode == 0x4e)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->zero = (val & (1 << 1)) == 0;
        c->half_carry = 1;
        c->subtract = 0;
        return 1;
    }
    c->zero = ((*reg) & (1 << 1)) == 0;
    c->half_carry = 1;
    c->subtract = 0;
    return 1;
}


int srl(cpu* c)
{
    // extended instruction
    // shift right register into carry
    // highest bit is set to 0
    uint8_t opcode = c->read(c->pc);
    c->t = 8;
    uint8_t* reg;
    if (opcode == 0x38)
        reg = &(c->b);
    else if (opcode == 0x39)
        reg = &(c->c);
    else if (opcode == 0x3a)
        reg = &(c->d);
    else if (opcode == 0x3b)
        reg = &(c->e);
    else if (opcode == 0x3c)
        reg = &(c->h);
    else if (opcode == 0x3d)
        reg = &(c->l);
    else if (opcode == 0x3e)
    {
        c->t = 16;
        uint16_t addr = c->h;
        addr = (addr << 8) | c->l;
        uint8_t val = c->read(addr);
        c->carry = val & 1;
        val >>= 1;
        c->write(addr, val);
        c->zero = (val == 0);
        c->half_carry = c->subtract = 0;
        return 1;
    }
    else if (opcode == 0x3f)
        reg = &(c->a);

    c->half_carry = c->subtract = 0;
    uint8_t val = (*reg) >> 1;
    c->zero = (val == 0);
    c->carry = (*reg) & 1;
    *reg = val;
    return 1;
}
