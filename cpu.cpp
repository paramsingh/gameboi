cpu::cpu()
{
	FILE *fp = fopen("DMG_ROM.bin", "rb");
	// Read bootstrap program from binary
	fread(memory, sizeof(uint8_t), 256, fp);
	fclose(fp);
	pc = 0;
	time = 0;
	zero = carry = half_carry = subtract = 0;
}

void cpu::status()
{
	printf("a = %x\n", a);
	printf("b = %x\n", b);
	printf("c = %x\n", c);
	printf("d = %x\n", d);
	printf("e = %x\n", e);
	printf("h = %x\n", h);
	printf("l = %x\n", l);
	printf("sp = %x\n", sp);
	printf("pc = %x\n", pc);
	printf("carry = %u\n", carry);
	printf("half_carry = %u\n", half_carry);
	printf("subtract = %u\n", subtract);
	printf("zero = %u\n", zero);
}

uint8_t cpu::read(uint16_t addr)
{
	// TODO: fix read i.e make banking work
	return memory[addr];
}

void cpu::write(uint16_t addr, uint8_t val)
{
	// TODO: make banking work
	memory[addr] = val;
}
