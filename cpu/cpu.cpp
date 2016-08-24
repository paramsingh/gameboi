
const int clock_speed = 4194304;

cpu::cpu()
{
	memset(memory, 0, sizeof(memory));
	FILE *fp = fopen("DMG_ROM.bin", "rb");
	// Read bootstrap program from binary
	fread(bootloader, sizeof(uint8_t), 256, fp);
	fclose(fp);
	// Tetris has no banking so we can read it all the way
	FILE *rom = fopen("Tetris (World).gb", "rb");
	fread(memory, sizeof(uint8_t), 32768, rom);
	fclose(rom);
	pc = 0x0000;
	time = 0;
	zero = carry = half_carry = subtract = 0;
	booting = 1;
	sp = 0xfffe;
	timer_counter = 1024;
	interrupts_enabled = 0;
}

void cpu::status()
{
	printf("a = %02x\n", a);
	printf("b = %02x\n", b);
	printf("c = %02x\n", c);
	printf("d = %02x\n", d);
	printf("e = %02x\n", e);
	printf("h = %02x\n", h);
	printf("l = %02x\n", l);
	printf("sp = %04x\n", sp);
	printf("pc = %04x\n", pc);
	printf("carry = %u\n", carry);
	printf("half_carry = %u\n", half_carry);
	printf("subtract = %u\n", subtract);
	printf("zero = %u\n", zero);
}

uint8_t cpu::read(uint16_t addr)
{
	// TODO: fix read i.e make banking work
	if (booting)
	{
		if (addr < 0x0100)
			return bootloader[addr];
		else if (pc == 0x0100)
			booting = 0;
	}
	return memory[addr];
}

void cpu::write(uint16_t addr, uint8_t val)
{
	// TODO: make banking work

	// no writing to read only memory
	if (addr < 0x8000)
		return;
	else if (addr >= 0xe000 && addr < 0xfe00) // echo ram
	{
		memory[addr] = val;
		write(addr - 0x2000, val);
	}
	else if (addr >= 0xfea0 && addr < 0xfeff) // restricted area
		return;
	else if (addr == 0xff44) // tries to write to the scanline register
		memory[addr] = 0;
	else if (addr == 0xff04) // divider register
		memory[addr] = 0;
	else
		memory[addr] = val;
}

// flag register format is the following
// Z N H C X X X

// convert the flag variables into the actual flag register
// value and return it;
uint8_t cpu::get_f()
{

	uint8_t val = 0;
	val |= (zero << 7);
	val |= (subtract << 6);
	val |= (half_carry << 5);
	val |= (carry << 4);
	return val;
}

// set the flag register to the given value
void cpu::set_f(uint8_t val)
{
	zero       = (val >> 7) & 1;
	subtract   = (val >> 6) & 1;
	half_carry = (val >> 5) & 1;
	carry      = (val >> 4) & 1;
}


// timer related functions
const uint16_t timer 	     = 0xff05;
const uint16_t timer_mod     = 0xff06;
const uint16_t timer_control = 0xff07;
const uint16_t divider_reg   = 0xff04;

int cpu::timer_on()
{
	uint8_t control = read(timer_control);
	return (control >> 2) & 1;
}

void cpu::update_timers()
{
	divider();
	if (timer_on())
	{
		timer_counter -= t;
		if (timer_counter <= 0)
		{
			setfreq();
			uint8_t val = read(timer);
			if (timer == 0xff)
			{
				write(timer, read(timer_mod));
				// TODO: Request Interrupt
			}
			else
			{
				write(timer, val + 1);
			}
		}
	}
}

void cpu::setfreq()
{
	uint8_t	freq = read(timer_control) & 0x3;
	if (freq == 0)
		timer_counter = 1024;
	else if (freq == 1)
		timer_counter = 16;
	else if (freq == 2)
		timer_counter = 64;
	else if (freq == 3)
		timer_counter = 256;
}

void cpu::divider()
{
	divide_counter += t;
	if (divide_counter >= 255)
	{
		divide_counter = 0;
		memory[divider_reg]++;
	}
}

// interrupts
const uint16_t ier = 0xffff;
const uint16_t irr = 0xff0f;

void cpu::request_interrupt(int id)
{
	uint8_t data = read(irr);
	data |= (1 << id);
	write(irr, data);
}

void cpu::service_interrupt(int id)
{
	// disable interrupts
	interrupts_enabled = 0;

	// change the irr so that cpu knows that interrupt has been
	// serviced
	uint8_t requests = read(irr);
	requests &= ~(1 << id);
	write(irr, requests);

	// push program counter into stack
	sp--;
	write(sp, (pc >> 8) & 0xff);
	sp--;
	write(sp, pc & 0xff);

	if (id == 0) // vblank
		pc = 0x40;
	else if (id == 1) // lcd
		pc = 0x48;
	else if (id == 2) // timer
		pc = 0x50;
	else if (id == 0x60) // joypad
		pc = 0x60;
}

void cpu::do_interrupts()
{
	if (interrupts_enabled)
	{
		uint8_t requests = read(irr);
		uint8_t enabled = read(ier);
		for (int id = 0; id <= 4; id++)
		{
			if ((requests >> id) & 1)
			{
				if ((enabled >> id) & 1)
					service_interrupt(id);
			}
		}
	}
}
