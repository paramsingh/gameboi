struct cpu
{
	uint8_t a; // accumulator 
	uint8_t b, c, d, e, h, l; // general purpose registers
	uint8_t perm_rom[2048]; // permanently banked ROM memory
	int rom_bank; // switchable ROM bank currently in use.
	int ram_bank; // switchable ROM bank currently in use.
	uint8_t video_ram[1024]; // Video RAM
	uint8_t work_ram_0[512]; // Game Boy’s working RAM bank 0.
	uint8_t work_ram_1[512]; // Game Boy’s working RAM bank 1.
	uint8_t sprites[32]; // Sprite Attribute Table.
	uint8_t device_map[16]; // Devices’ Mappings. Used to access I/O devices.
	uint8_t high_ram[16]; // High RAM Area.
	uint8_t ien; // Interrupt Enable Register.
	uint16_t sp; // Stack Pointer
	uint16_t pc; // Program Counter
	bool carry, half_carry, subtract, zero; // flags

	cpu();
	void status();
};
/*
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