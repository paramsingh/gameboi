int jr(cpu* c)
{
    // add value n to the pc according to some condition
    uint8_t opcode = c->read(c->pc);

    int condition;
    if (opcode == 0x18)
        // jump unconditionally
        condition = 1;
    else if (opcode == 0x20)
        // jump if zero is reset
        condition = !(c->zero);
    else if (opcode == 0x28)
        // jump if zero is set
        condition = (c->zero);
    else if (opcode == 0x30)
        // jump if carry is reset
        condition = !(c->carry);
    else if (opcode == 0x38)
        // jump if carry is set
        condition = (c->carry);

    if (condition)
        c->pc += (int8_t)c->read(c->pc + 1);
    return 1;
}

int call(cpu *c)
{
    // Push address of next instruction onto stack and then
    //jump to address ()
    uint8_t opcode = c->read(c->pc);
    switch(opcode)
    {
        case 0xcd:
        {
            //Push address of next instruction onto stack and then
            //jump to address nn.
            c->sp --;
            c->write(c->sp, (c->pc+3)>>8);
            c->sp --;
            c->write(c->sp, (c->pc+3));
            uint16_t var = c->read(c->pc+1) | ((uint16_t)c->read(c->pc+2)<<8);
            c->pc = var;
            break;
        }
        default:
        {
            return 0;
        }
    }
    return 2;
}

int push(cpu* c)
{
    uint16_t opcode = c->read(c->pc);
    switch(opcode)
    {
        case 0xc5:
        {
            c->sp--;
            c->write(c->sp, c->b);
            c->sp--;
            c->write(c->sp, c->c);
            break;
        }
        default:
        {
            return 0;
        }
    }
    return 1;
}

int rl(cpu* c)
{
    uint16_t opcode = c->read(c->pc);
    switch (opcode)
    {
        case 0x11:
        {
            // rotate c left through carry flag
            uint16_t x = (((uint16_t)(c->c))<< 1);
            if (c->carry == 1)
                x |= 1;
            if (x & (1 << 8))
                c->carry = 1;
            c->c = x;
            if (c->c == 0)
                c->zero = 1;
            break;
        }
        case 0x17:
        {
            // rotate a left through carry flag
            uint16_t x = (((uint16_t)(c->a))<< 1);
            if (c->carry == 1)
                x |= 1;
            if (x & (1 << 8))
                c->carry = 1;
            c->a = x;
            if (c->a == 0)
                c->zero = 1;
            break;
        }
        default:
            return 0;
    }
    c->subtract = 0;
    c->half_carry = 0;
    return 1;
}

int pop(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    switch (opcode)
    {
        case 0xc1:
        {
            // pop into bc
            c->c = c->read(c->sp);
            c->b = c->read(c->sp + 1);
            c->sp += 2;
            break;
        }
        default:
            return 0;
    }
    return 1;
}

int ret(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    int condition = 1; // for unconditional return
    if (opcode == 0xc0)
        // return if z flag is reset
        condition = !(c->zero);
    else if (opcode == 0xc8)
        // return if z flag is set
        condition = c->zero;
    else if (opcode == 0xd0)
        // return if carry is reset
        condition = !(c->carry);
    else if (opcode == 0xd8)
        // return if carry is set
        condition = c->carry;

    if (condition)
    {
        uint16_t lo = c->read(c->sp);
        uint16_t hi = c->read(c->sp + 1);
        c->sp += 2;
        c->pc = (hi << 8) | lo;
        return 2;
    }
    else
        return 1;
}
