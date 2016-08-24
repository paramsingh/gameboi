int di(cpu* c)
{
    c->pending_disable = 1;
    return 1;
}

int ei(cpu* c)
{
    c->pending_enable = 1;
    return 1;
}

int scf(cpu* c)
{
    c->t = 4;
    c->carry = 1;
    c->half_carry = c->subtract = 0;
    return 1;
}
