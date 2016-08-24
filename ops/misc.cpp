int di(cpu* c)
{
    c->interrupts_enabled = 0;
    return 1;
}

int ei(cpu* c)
{
    c->interrupts_enabled = 1;
    return 1;
}
