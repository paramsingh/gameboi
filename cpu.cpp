cpu::cpu()
{
	FILE *fp = fopen("DMG_ROM.bin", "rb");
	fread(perm_rom, sizeof(uint8_t), 256, fp);
	for(int i=0;i < 256; i++)
		printf("%x\n", perm_rom[i]);
	fclose(fp);
}

void cpu::status()
{
	printf("a = %u\n", a);
	printf("b = %u\n", b);
	printf("c = %u\n", c);
	printf("d = %u\n", d);
	printf("e = %u\n", e);
	printf("h = %u\n", h);
	printf("l = %u\n", l);
	printf("sp = %u\n", sp);
	printf("pc = %u\n", pc);
	printf("carry = %u\n", carry);
	printf("half_carry = %u\n", half_carry);
	printf("subtract = %u\n", subtract);
	printf("zero = %u\n", zero);
}