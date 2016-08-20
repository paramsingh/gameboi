int cmp(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t n;
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
    }
    else if (opcode == 0xfe)
    {
        n = c->read(c->pc + 1);
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

int  xorop(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    switch (opcode)
    {
            case 0xaf:
            {
                // xor a with itself
                c->a = 0;
                c->zero = 1;
                break;
            }
            default:
                return 0;
    }
    c->carry = c->half_carry = c->subtract = 0;
    return 1;
}

int  bit(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    switch (opcode)
    {
        case 0x7c:
        {
            // test bit 7 of H register
            // if it is 0, set zero flag
            // else clear zero flag
            if ((c -> h) & (1 << 7))
                c->zero = 0;
            else
                c->zero = 1;
            break;
        }
        default:
            return 0;
    }
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
