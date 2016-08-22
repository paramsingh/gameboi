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
    {
        c->t = 12;
        c->pc += (int8_t)(c->read(c->pc + 1));
    }
    else
        c->t = 8;
    return 1;
}

int call(cpu *c)
{
    // Push address of next instruction onto stack and then
    // jump to address ()
    uint8_t opcode = c->read(c->pc);
    int condition;
    if (opcode == 0xcd)
        condition = 1;
    else if (opcode == 0xc4)
        condition = (c->zero == 0);
    else if (opcode == 0xcc)
        condition = c->zero;
    else if (opcode == 0xd4)
        condition = (c->carry == 0);
    else if (opcode == 0xdc)
        condition = c->carry;

    if (condition)
    {
        c->t = 24;
        c->sp--;
        c->write(c->sp, (c->pc + 3) >> 8);
        c->sp--;
        c->write(c->sp, (c->pc + 3));
        uint16_t next = c->read(c->pc + 2);
        next = (next << 8) | (c->read(c->pc + 1));
        c->pc = next;
        //printf("stack lo = %02x, hi = %02x\n", c->read(c->sp), c->read(c->sp + 1));
        return 2;
    }
    else
    {
        c->t = 12;
        return 1;
    }
}

int push(cpu* c)
{
    uint16_t opcode = c->read(c->pc);
    uint8_t hi, lo;
    if (opcode == 0xf5)
        hi = (c->a), lo = c->get_f();
    else if (opcode == 0xc5)
        hi = c->b, lo = c->c;
    else if (opcode == 0xd5)
        hi = c->d, lo = c->e;
    else if (opcode == 0xe5)
        hi = c->h, lo = c->l;

    // takes 16 cycles
    c->t = 16;

    // push higher bit first and then lower bit
    // stack grows downwards
    c->sp--;
    c->write(c->sp, hi);
    c->sp--;
    c->write(c->sp, lo);

    return 1;
}


int pop(cpu* c)
{
    uint8_t opcode = c->read(c->pc);
    uint8_t *hi, *lo;
    uint8_t f;
    c->t = 12;
    if (opcode == 0xc1)
        hi = &(c->b), lo = &(c->c);
    else if (opcode == 0xd1)
        hi = &(c->d), lo = &(c->e);
    else if (opcode == 0xe1)
        hi = &(c->h), lo = &(c->l);
    else if (opcode == 0xf1)
    {
        c->a = c->read(c->sp + 1);
        c->set_f(c->read(c->sp));
        c->sp += 2;
        return 1;
    }

    *lo = c->read(c->sp);
    *hi = c->read(c->sp + 1);
    c->sp += 2;
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
        if (opcode == 0xc9)
            c->t = 16;
        else
            c->t = 20;
        return 2;
    }
    else {
        c->t = 8;
        return 1;
    }
}
