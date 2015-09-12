/*-
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42): Rink Springer <rink@rink.nu> wrote
 * this file. As long as you retain this notice you can do whatever you want
 * with this stuff. If we meet some day, and you think this stuff is worth it,
 * you can buy me a beer in return Rink Springer
 * ----------------------------------------------------------------------------
 */
#include "vic.h"
#include "hostio.h"
#include <stdio.h>
#include <string.h>

#if 1
#define TRACE(x...)
#else
#define TRACE(x...) printf(x)
#endif

VIC::VIC(CPU6502& oCPU, Memory& oMemory, HostIO& oHostIO)
	: XPeripheral(oCPU, oMemory), m_hostio(oHostIO)
{
	m_vic_regs = new uint8_t[Memory::s_vic_length];
}

VIC::~VIC()
{
	delete[] m_vic_regs;
}

void
VIC::Reset()
{
	m_screen_base = 0x400;
	memset(m_vic_regs, 0, Memory::s_vic_length); /* XXX is this correct? */
}

void
VIC::Poke(Memory::addr_t addr, uint8_t val)
{
	m_vic_regs[addr & 0xff] = val;

	switch(addr & 0xff) {
		case 0x18: // memory control register
			m_screen_base = (val >> 4) * 0x400;
			m_char_base = (val & 0xe) * 0x400;
			TRACE("VIC::Poke(): mcr, val %x => screen_base=%x char_base=%x\n", val, m_screen_base, m_char_base);
			break;
		default:
			TRACE("VIC::Poke(): unhandled addr=0x%x val=%x\n", addr, val);
	}
}

uint8_t
VIC::Peek(Memory::addr_t addr)
{
	switch(addr & 0xff) {
		case 0x12: // read/write raster value
			return 0;
		default:
			TRACE("VIC::Peek(): unhandled addr=0x%x\n", addr);
			break;
	}
	return m_vic_regs[addr & 0xff];
}

void
VIC::Update()
{
	//printf("addr_base=%x, screen_base=%x char_base=%x\n", m_addr_base, m_screen_base, m_char_base);
	for (int y = 0; y < 25; y++)
		for (int x = 0; x < 40; x++) {
			uint8_t n = ByteAt(m_addr_base + m_screen_base + (y * 40) + x);

			for (int j = 0; j < 8; j++)
				for (int i = 0; i < 8; i++) {
					//uint8_t ch = ByteAt(m_addr_base + m_char_base + n * 8 + j);
					uint8_t ch = ByteAt(0xd000 + n * 8 + j);
					m_hostio.putpixel(x * 8 + i, y * 8 + j, (ch & (1 << (8 - i))) ? 1 : 0);
				}
		}
}

/* vim:set ts=2 sw=2: */
