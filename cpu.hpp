struct cpu
{
	uint8_t a; // accumulator
	uint8_t b, c, d, e, h, l; // general purpose registers
	uint8_t memory[65536]; // permanently banked ROM memory
	int rom_bank; // switchable ROM bank currently in use.
	int ram_bank; // switchable ROM bank currently in use.
	uint16_t sp; // Stack Pointer
	uint16_t pc; // Program Counter
	bool carry, half_carry, subtract, zero; // flags
	int time;
	cpu();
	void status();
	uint8_t read(uint16_t);
	void write(uint16_t, uint8_t);
};

/*
0x0000-0x3FFF: Permanent ROM Bank
0x4000-0x7FFF: Area for switchable ROM banks.
0x8000-0x9FFF: Video RAM.
0xA000-0xBFFF: Area for switchable external RAM banks.
0xC000-0xCFFF: Game Boy’s working RAM bank 0 .
0xD000-0xDFFF: Game Boy’s working RAM bank 1.
0xFE00-0xFEFF: Sprite Attribute Table.
0xFF00-0xFF7F: Devices’ Mappings. Used to access I/O devices.
0xFF80-0xFFFE: High RAM Area.
0xFFFF: Interrupt Enable Register.
*/
