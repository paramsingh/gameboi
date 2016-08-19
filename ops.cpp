int  not_imp(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	printf("operation decimal : %d, hex: %x not implemented yet!\n", opcode, opcode);
	return 0;
}

operation op("Dummy", 0, 0, not_imp);

// Function for all load type operations
int ld(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	switch (opcode)
	{
		case 0x31:
		{
			// this loads the stack pointer with 2 byte immediate value
			// assuming that lower bits are given first
			uint16_t low = c->read(c->pc + 1);
			uint16_t high = c->read(c->pc + 2);
			low = (high << 8) | low;
			c->sp = low;
			break;
		}
		case 0x21:
		{
			// LD HL nn
			c->l = c->read(c ->pc + 1);
			c->h = c->read(c->pc + 2);
			break;
		}
		case 0x32:
		{
			// load value from a into address at hl and
			// decrement hl
			uint16_t addr = (((uint16_t)c->h) << 8) | (c -> l);
			c->write(addr, c->a);
			addr--;
			c->l = addr;
			c->h = addr >> 8;
			break;
		}
        case 0x0e:
        {
            //load 8-bit immediate value in register C
            c->c = c->read(c->pc+1);
            break;
        }
        case 0x3e:
        {
        	//load 8 bit immediate value in accumulator
        	c->a = c->read(c->pc +1);
        	break;
        }
        case 0x2e:
        {

        	//load 8 bit immediate value in register L
        	c->l = c->read(c->pc +1);
        	break;
        }
        case 0xe2:
        {
        	//Put A into address $FF00 + register C
        	c->write(0xFF00 + (uint16_t)c->c, c->a);
        	break;

        }
        case 0x77:
        {
        	//Put Accumulator value in memory addr in HL pair
        	uint16_t val = c->l | (((uint16_t)c->h) << 8);
        	c->write(val, c->a);
        	break;
        }
        case 0xe0:
        {
        	//Put A into memory address $FF00+n (n is immediate 8 bit value)
        	uint8_t n = c->read(c->pc + 1);
        	c->write(0xff00 + (uint16_t)n, c->a);
        	break;
        }
        case 0x11:
        {
        	// Put 16 bit immediate value at DE pair
        	c->e = c->read(c->pc+1);
        	c->d = c->read(c->pc+2);
        	break;
        }
        case 0x1a:
        {
        	uint16_t addr = (((uint16_t)c->d) << 8) | c->e;
        	c->a = c->read(addr);
        	break;
        }
        case 0x06:
        {
        	c->b = c->read(c->pc + 1);
        	break;
        }
        case 0x22:
        {
        	// load a into (HL) and increment HL
        	uint16_t addr = (((uint16_t)c->h)<<8) | c->l;
        	c->write(addr, c->a);
        	addr++;
        	c->h = (addr >> 8);
        	c->l = addr;
        	break;
        }
		default:
			return 0;
	}
	return 1;
}

int load_rtoa(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	uint8_t *from;
	if (opcode == 0x7f)
		from = &(c->a);
	else if (opcode == 0x78)
		from = &(c->b);
	else if (opcode == 0x79)
		from = &(c->c);
	else if (opcode == 0x7A)
		from = &(c->d);
	else if (opcode == 0x7B)
		from = &(c->e);
	else if (opcode == 0x7C)
		from = &(c->h);
	else if (opcode == 0x7D)
		from = &(c->l);
	else if (opcode == 0x7E)
	{
		uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
		from = c->memory + addr;
	}
	c->a = *from;
	return 1;
}

int load_rtob(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	uint8_t *from;
	if (opcode == 0x40)
		from = &(c->b);
	else if (opcode == 0x41)
		from = &(c->c);
	else if (opcode == 0x42)
		from = &(c->d);
	else if (opcode == 0x43)
		from = &(c->e);
	else if (opcode == 0x44)
		from = &(c->h);
	else if (opcode == 0x45)
		from = &(c->l);
	else if (opcode == 0x46)
	{
		uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
		from = c->memory + addr;
	}
	else if (opcode == 0x47)
		from = &(c->a);
	c->b = *from;
	return 1;
}

int load_rtoc(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	uint8_t *from;
	if (opcode == 0x48)
		from = &(c->b);
	else if (opcode == 0x49)
		from = &(c->c);
	else if (opcode == 0x4a)
		from = &(c->d);
	else if (opcode == 0x4b)
		from = &(c->e);
	else if (opcode == 0x4c)
		from = &(c->h);
	else if (opcode == 0x4d)
		from = &(c->l);
	else if (opcode == 0x4e)
	{
		uint16_t addr = ((uint16_t)(c->h) << 8) | (c->l);
		from = c->memory + addr;
	}
	else if (opcode == 0x4f)
		from = &(c -> a);
	c->c = *from;
	return 1;
}

int  xorop(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	switch (opcode)
	{
			case 0xaf:
			{
				// xor a with itself
				c->a = 0;
				c->zero = 1;
				break;
			}
			default:
				return 0;
	}
	c->carry = c->half_carry = c->subtract = 0;
	return 1;
}

int  bit(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	switch (opcode)
	{
		case 0x7c:
		{
			// test bit 7 of H register
			// if it is 0, set zero flag
			// else clear zero flag
			if ((c -> h) & (1 << 7))
				c->zero = 0;
			else
				c->zero = 1;
			break;
		}
		default:
			return 0;
	}
	return 1;
}

int jr(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	switch (opcode)
	{
		case 0x20:
		{
			// jr nz nn
			// add nn to next value of pc
			// and jump there
			if (c->zero == 0)
			{
				int8_t diff = c->read(c->pc + 1);
				c->pc += diff;
			}
			break;
		}
		default:
			return 0;
	}
	return 1;
}

int inc(cpu *c)
{
	//flags affected
	/* Z - Set if result is zero.
	N - Reset.
	H - Set if carry from bit 3.
	C - Not affected*/
	uint8_t opcode = c->read(c->pc);
	switch(opcode)
	{
		case 0x0c:
		{
			//Increment register C.
			if(c->c & 0xf == 0xf)
				c->half_carry = 1;
			else
				c->half_carry = 0;
			c->c++;
			if(c->c == 0)
				c->zero = 1;
			else
				c->zero = 0;
			break;

		}
		case 0x04:
		{
			//Increment register B.
			if(c->b & 0xf == 0xf)
				c->half_carry = 1;
			else
				c->half_carry = 0;
			c->b++;
			if(c->b == 0)
				c->zero = 1;
			else
				c->zero = 0;
			break;
		}
		default:
		{
			return 0;
		}
	}
	c->subtract = 0;
	return 1;
}

int inc_pair(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	uint8_t *hi, *lo;
	if (opcode == 0x03)
		// increment the bc pair
		hi = &(c->b), lo = &(c->c);
	else if (opcode == 0x13)
		// increment the de pair
		hi = &(c->d), lo = &(c->e);
	else if (opcode == 0x23)
		// increment the hl pair
		hi = &(c->h), lo = &(c->l);
	else if (opcode == 0x33) {
		// increment the stack pointer
		c->sp++;
		return 1;
	}

	uint16_t val = (((uint16_t)(*hi)) << 8) | (*lo);
	val++;
	*hi = val >> 8;
	*lo = val;
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
	uint16_t opcode	= c->read(c->pc);
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

int dec(cpu* c)
{
	uint8_t opcode = c->read(c->pc);
	uint8_t *reg;
	if (opcode == 0x3d)
		reg = &(c -> a);
	else if (opcode == 0x05)
		reg = &(c -> b);
	else if (opcode == 0x0D)
		reg = &(c -> c);
	else if (opcode == 0x15)
		reg = &(c -> d);
	else if (opcode == 0x1D)
		reg = &(c -> e);
	else if (opcode == 0x25)
		reg = &(c -> h);
	else if (opcode == 0x2d)
		reg = &(c -> l);
	else if (opcode == 0x35)
	{
		uint16_t addr = (((uint16_t)c->h)<<8) | (c->l);
		reg = c->memory + addr;
	}

	// now have to decrease value in reg by 1
	// if the lower bits are all zero, then half carry will be set
	if ((*reg) & 0xf == 0)
		c->half_carry = 1;
	else
		c->half_carry = 0;
	(*reg)--;
	if (*reg == 0)
		c->zero = 1;
	else
		c->zero = 0;
	c->subtract = 1;
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
operation inst_set[512] = {
	// 0
	op,
	// 1
	op,
	// 2
	op,
	// 3
	operation("INC BC", 1, 8, inc_pair),
	// 4
	operation("INC B", 1, 4, inc),
	// 5
	operation("DEC B", 1, 4, dec),
	// 6
	operation("LD B n", 1, 8, ld),
	// 7
	op,
	// 8
	op,
	// 9
	op,
	// 10
	op,
	// 11
	op,
	// 12
	operation("INC C",1,4,inc),
	// 13
	operation("DEC C", 1, 4, dec),
	// 14
	operation("LD C n", 2, 8, ld),
	// 15
	op,
	// 16
	op,
	// 17
	operation("LD DE nn", 3, 12,ld),
	// 18
	op,
	// 19
	operation("INC DE", 1, 8, inc_pair),
	// 20
	op,
	// 21
	operation("DEC D", 1, 4, dec),
	// 22
	op,
	// 23
	operation("RLA", 1, 4, rl),
	// 24
	op,
	// 25
	op,
	// 26
	operation("LD A (DE)", 1, 8, ld),
	// 27
	op,
	// 28
	op,
	// 29
	operation("DEC E", 1, 4, dec),
	// 30
	op,
	// 31
	op,
	// 32
	operation("JR NZ nn", 2, 8, jr),
	// 33
	operation("LD HL nn", 3, 12, ld),
	// 34
	operation("LD (HL+), A", 1, 8, ld),
	// 35
	operation("INC HL", 1, 8, inc_pair),
	// 36
	op,
	// 37
	operation("DEC H", 1, 4, dec),
	// 38
	op,
	// 39
	op,
	// 40
	op,
	// 41
	op,
	// 42
	op,
	// 43
	op,
	// 44
	op,
	// 45
	operation("DEC L", 1, 4, dec),
	// 46
	operation("LD L n",2, 8, ld),
	// 47
	op,
	// 48
	op,
	// 49
	operation("LD SP nn", 3, 12, ld),
	// 50
	operation("LDD HL A", 1, 8, ld),
	// 51
	operation("INC SP", 1, 8, inc_pair),
	// 52
	op,
	// 53
	operation("DEC (HL)", 1, 12, dec),
	// 54
	op,
	// 55
	op,
	// 56
	op,
	// 57
	op,
	// 58
	op,
	// 59
	op,
	// 60
	op,
	// 61
	operation("DEC A", 1, 4, dec),
	// 62
	operation("LD A #", 2, 8, ld),
	// 63
	op,
	// 64
	operation("LD B B", 1, 4, load_rtob),
	// 65
	operation("LD B, C", 1, 4, load_rtob),
	// 66
	operation("LD B, D", 1, 4, load_rtob),
	// 67
	operation("LD B, E", 1, 4, load_rtob),
	// 68
	operation("LD B, H", 1, 4, load_rtob),
	// 69
	operation("LD B, L", 1, 4, load_rtob),
	// 70
	operation("LD B, (HL)", 1, 8, load_rtob),
	// 71
	operation("LD B, A", 1, 4, load_rtob),
	// 72
	operation("LD C B", 1, 4, load_rtoc),
	// 73
	operation("LD C C", 1, 4, load_rtoc),
	// 74
	operation("LD C D", 1, 4, load_rtoc),
	// 75
	operation("LD C E", 1, 4, load_rtoc),
	// 76
	operation("LD C H", 1, 4, load_rtoc),
	// 77
	operation("LD C, L", 1, 4, load_rtoc),
	// 78
	operation("LD C, (HL)", 1, 8, load_rtoc),
	// 79
	operation("LD C, A", 1, 4, load_rtoc),
	// 80
	op,
	// 81
	op,
	// 82
	op,
	// 83
	op,
	// 84
	op,
	// 85
	op,
	// 86
	op,
	// 87
	op,
	// 88
	op,
	// 89
	op,
	// 90
	op,
	// 91
	op,
	// 92
	op,
	// 93
	op,
	// 94
	op,
	// 95
	op,
	// 96
	op,
	// 97
	op,
	// 98
	op,
	// 99
	op,
	// 100
	op,
	// 101
	op,
	// 102
	op,
	// 103
	op,
	// 104
	op,
	// 105
	op,
	// 106
	op,
	// 107
	op,
	// 108
	op,
	// 109
	op,
	// 110
	op,
	// 111
	op,
	// 112
	op,
	// 113
	op,
	// 114
	op,
	// 115
	op,
	// 116
	op,
	// 117
	op,
	// 118
	op,
	// 119
	operation("LD (HL) n", 1, 8, ld),
	// 120
	operation("LD A, B", 1, 4, load_rtoa),
	// 121
	operation("LD A, C", 1, 4, load_rtoa),
	// 122
	operation("LD A, D", 1, 4, load_rtoa),
	// 123
	operation("LD A, E", 1, 4, load_rtoa),
	// 124
	operation("LD A, H", 1, 4, load_rtoa),
	// 125
	operation("LD A, L", 1, 4, load_rtoa),
	// 126
	operation("LD A, (HL)", 1, 8, load_rtoa),
	// 127
	operation("LD A, A", 1, 4, load_rtoa),
	// 128
	op,
	// 129
	op,
	// 130
	op,
	// 131
	op,
	// 132
	op,
	// 133
	op,
	// 134
	op,
	// 135
	op,
	// 136
	op,
	// 137
	op,
	// 138
	op,
	// 139
	op,
	// 140
	op,
	// 141
	op,
	// 142
	op,
	// 143
	op,
	// 144
	op,
	// 145
	op,
	// 146
	op,
	// 147
	op,
	// 148
	op,
	// 149
	op,
	// 150
	op,
	// 151
	op,
	// 152
	op,
	// 153
	op,
	// 154
	op,
	// 155
	op,
	// 156
	op,
	// 157
	op,
	// 158
	op,
	// 159
	op,
	// 160
	op,
	// 161
	op,
	// 162
	op,
	// 163
	op,
	// 164
	op,
	// 165
	op,
	// 166
	op,
	// 167
	op,
	// 168
	op,
	// 169
	op,
	// 170
	op,
	// 171
	op,
	// 172
	op,
	// 173
	op,
	// 174
	op,
	// 175
	operation("XOR A", 1, 4, xorop),
	// 176
	op,
	// 177
	op,
	// 178
	op,
	// 179
	op,
	// 180
	op,
	// 181
	op,
	// 182
	op,
	// 183
	op,
	// 184
	op,
	// 185
	op,
	// 186
	op,
	// 187
	op,
	// 188
	op,
	// 189
	op,
	// 190
	op,
	// 191
	op,
	// 192
	operation("RET NZ", 1, 8, ret),
	// 193
	operation("POP BC", 1, 12, pop),
	// 194
	op,
	// 195
	op,
	// 196
	op,
	// 197
	operation("PUSH BC", 1, 16, push),
	// 198
	op,
	// 199
	op,
	// 200
	operation("RET Z", 1, 8, ret),
	// 201
	operation("RET", 1, 8, ret),
	// 202
	op,
	// 203
	op,
	// 204
	op,
	// 205
	operation("CALL nn",3,12,call),
	// 206
	op,
	// 207
	op,
	// 208
	operation("RET NC", 1, 8, ret),
	// 209
	op,
	// 210
	op,
	// 211
	op,
	// 212
	op,
	// 213
	op,
	// 214
	op,
	// 215
	op,
	// 216
	operation("RET C", 1, 8, ret),
	// 217
	op,
	// 218
	op,
	// 219
	op,
	// 220
	op,
	// 221
	op,
	// 222
	op,
	// 223
	op,
	// 224
	operation("LD ($FF00 +n),A", 2,12,ld),
	// 225
	op,
	// 226
	operation("LD ($FF00+C),A",1, 8, ld),
	// 227
	op,
	// 228
	op,
	// 229
	op,
	// 230
	op,
	// 231
	op,
	// 232
	op,
	// 233
	op,
	// 234
	op,
	// 235
	op,
	// 236
	op,
	// 237
	op,
	// 238
	op,
	// 239
	op,
	// 240
	op,
	// 241
	op,
	// 242
	op,
	// 243
	op,
	// 244
	op,
	// 245
	op,
	// 246
	op,
	// 247
	op,
	// 248
	op,
	// 249
	op,
	// 250
	op,
	// 251
	op,
	// 252
	op,
	// 253
	op,
	// 254
	op,
	// 255
	op,
	// 256
	op,
	// 257
	op,
	// 258
	op,
	// 259
	op,
	// 260
	op,
	// 261
	op,
	// 262
	op,
	// 263
	op,
	// 264
	op,
	// 265
	op,
	// 266
	op,
	// 267
	op,
	// 268
	op,
	// 269
	op,
	// 270
	op,
	// 271
	op,
	// 272
	operation("RL C", 1, 8, rl),
	// 273
	op,
	// 274
	op,
	// 275
	op,
	// 276
	op,
	// 277
	op,
	// 278
	op,
	// 279
	op,
	// 280
	op,
	// 281
	op,
	// 282
	op,
	// 283
	op,
	// 284
	op,
	// 285
	op,
	// 286
	op,
	// 287
	op,
	// 288
	op,
	// 289
	op,
	// 290
	op,
	// 291
	op,
	// 292
	op,
	// 293
	op,
	// 294
	op,
	// 295
	op,
	// 296
	op,
	// 297
	op,
	// 298
	op,
	// 299
	op,
	// 300
	op,
	// 301
	op,
	// 302
	op,
	// 303
	op,
	// 304
	op,
	// 305
	op,
	// 306
	op,
	// 307
	op,
	// 308
	op,
	// 309
	op,
	// 310
	op,
	// 311
	op,
	// 312
	op,
	// 313
	op,
	// 314
	op,
	// 315
	op,
	// 316
	op,
	// 317
	op,
	// 318
	op,
	// 319
	op,
	// 320
	op,
	// 321
	op,
	// 322
	op,
	// 323
	op,
	// 324
	op,
	// 325
	op,
	// 326
	op,
	// 327
	op,
	// 328
	op,
	// 329
	op,
	// 330
	op,
	// 331
	op,
	// 332
	op,
	// 333
	op,
	// 334
	op,
	// 335
	op,
	// 336
	op,
	// 337
	op,
	// 338
	op,
	// 339
	op,
	// 340
	op,
	// 341
	op,
	// 342
	op,
	// 343
	op,
	// 344
	op,
	// 345
	op,
	// 346
	op,
	// 347
	op,
	// 348
	op,
	// 349
	op,
	// 350
	op,
	// 351
	op,
	// 352
	op,
	// 353
	op,
	// 354
	op,
	// 355
	op,
	// 356
	op,
	// 357
	op,
	// 358
	op,
	// 359
	op,
	// 360
	op,
	// 361
	op,
	// 362
	op,
	// 363
	op,
	// 364
	op,
	// 365
	op,
	// 366
	op,
	// 367
	op,
	// 368
	op,
	// 369
	op,
	// 370
	op,
	// 371
	op,
	// 372
	op,
	// 373
	op,
	// 374
	op,
	// 375
	op,
	// 376
	op,
	// 377
	op,
	// 378
	op,
	// 379
	operation("BIT 7, h", 1, 8, bit),
	// 380
	op,
	// 381
	op,
	// 382
	op,
	// 383
	op,
	// 384
	op,
	// 385
	op,
	// 386
	op,
	// 387
	op,
	// 388
	op,
	// 389
	op,
	// 390
	op,
	// 391
	op,
	// 392
	op,
	// 393
	op,
	// 394
	op,
	// 395
	op,
	// 396
	op,
	// 397
	op,
	// 398
	op,
	// 399
	op,
	// 400
	op,
	// 401
	op,
	// 402
	op,
	// 403
	op,
	// 404
	op,
	// 405
	op,
	// 406
	op,
	// 407
	op,
	// 408
	op,
	// 409
	op,
	// 410
	op,
	// 411
	op,
	// 412
	op,
	// 413
	op,
	// 414
	op,
	// 415
	op,
	// 416
	op,
	// 417
	op,
	// 418
	op,
	// 419
	op,
	// 420
	op,
	// 421
	op,
	// 422
	op,
	// 423
	op,
	// 424
	op,
	// 425
	op,
	// 426
	op,
	// 427
	op,
	// 428
	op,
	// 429
	op,
	// 430
	op,
	// 431
	op,
	// 432
	op,
	// 433
	op,
	// 434
	op,
	// 435
	op,
	// 436
	op,
	// 437
	op,
	// 438
	op,
	// 439
	op,
	// 440
	op,
	// 441
	op,
	// 442
	op,
	// 443
	op,
	// 444
	op,
	// 445
	op,
	// 446
	op,
	// 447
	op,
	// 448
	op,
	// 449
	op,
	// 450
	op,
	// 451
	op,
	// 452
	op,
	// 453
	op,
	// 454
	op,
	// 455
	op,
	// 456
	op,
	// 457
	op,
	// 458
	op,
	// 459
	op,
	// 460
	op,
	// 461
	op,
	// 462
	op,
	// 463
	op,
	// 464
	op,
	// 465
	op,
	// 466
	op,
	// 467
	op,
	// 468
	op,
	// 469
	op,
	// 470
	op,
	// 471
	op,
	// 472
	op,
	// 473
	op,
	// 474
	op,
	// 475
	op,
	// 476
	op,
	// 477
	op,
	// 478
	op,
	// 479
	op,
	// 480
	op,
	// 481
	op,
	// 482
	op,
	// 483
	op,
	// 484
	op,
	// 485
	op,
	// 486
	op,
	// 487
	op,
	// 488
	op,
	// 489
	op,
	// 490
	op,
	// 491
	op,
	// 492
	op,
	// 493
	op,
	// 494
	op,
	// 495
	op,
	// 496
	op,
	// 497
	op,
	// 498
	op,
	// 499
	op,
	// 500
	op,
	// 501
	op,
	// 502
	op,
	// 503
	op,
	// 504
	op,
	// 505
	op,
	// 506
	op,
	// 507
	op,
	// 508
	op,
	// 509
	op,
	// 510
	op,
	// 511
	op
};


operation::operation(string name, int size, int cycles, int (*f)(cpu*))
{
	this->name = name;
	this->size = size;
	this->cycles = cycles;
	func = f;
}
