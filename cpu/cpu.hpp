struct cpu
{
	uint8_t a; // accumulator
	uint8_t b, c, d, e, h, l; // general purpose registers
	uint8_t bootloader[256];  // saves the bootstrap program
	uint8_t memory[65536]; // permanently banked ROM memory
	int rom_bank; // switchable ROM bank currently in use.
	int ram_bank; // switchable ROM bank currently in use.
	uint16_t sp; // Stack Pointer
	uint16_t pc; // Program Counter
	bool carry, half_carry, subtract, zero; // flags
	bool booting; // stores if we're in the bootstrap program or not
	int time;
	int enable_interrupts;

	// constructor
	cpu();



	// read and write 8 bit values into memory
	uint8_t read(uint16_t);
	void write(uint16_t, uint8_t);

	// variable to save the number of cycles taken by last instruction
	// need to store this because it is used in gpu emulation also
	uint8_t t;

	// get the value of the flag register at the current time
	uint8_t get_f();
	// set the value of the flag register
	void set_f(uint8_t);


	// Interrupts
	int interrupts_enabled;
	int pending_enable;
	int pending_disable;
	void do_interrupts();
	void request_interrupt(int id);
	void service_interrupt(int id);


	// timer related stuff
	int timer_counter;
	int divide_counter;
	int timer_on();
	void update_timers();
	void divider();
	void setfreq();

	// dma transfer
	void dma(uint8_t);

	// joypad
	uint8_t joypad_state; // high bits for direction and low bits for standard buttons
	uint8_t get_joypad_state();
	void key_pressed(int key);

	// debugging
	void status();
	void memory_dump(char*, uint16_t, uint16_t);
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
