/*-
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42): Rink Springer <rink@rink.nu> wrote
 * this file. As long as you retain this notice you can do whatever you want
 * with this stuff. If we meet some day, and you think this stuff is worth it,
 * you can buy me a beer in return Rink Springer
 * ----------------------------------------------------------------------------
 */
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Memory::ConnectedPeripheral::ConnectedPeripheral(XPeripheral* peripheral, addr_t base, uint16_t length)
	: m_base(base), m_length(length), m_peripheral(peripheral)
{
}

bool
Memory::ConnectedPeripheral::IsAddressClaimed(addr_t addr) const
{
	return addr >= m_base && addr < m_base + m_length;
}

void
Memory::ConnectedPeripheral::Poke(addr_t addr, uint8_t val)
{
	m_peripheral->Poke(addr, val);
}

uint8_t
Memory::ConnectedPeripheral::Peek(addr_t addr)
{
	return m_peripheral->Peek(addr);
}

void
Memory::ConnectedPeripheral::Reset()
{
	m_peripheral->Reset();
}

Memory::Memory()
{
	m_Memory = new uint8_t[m_MemorySize];
	memset(m_Memory, 0, m_MemorySize); /* XXX Should be random! */
}

Memory::~Memory()
{
	delete[] m_Memory;
}

void
Memory::RegisterPeripheral(XPeripheral* peripheral, addr_t base, uint16_t length)
{
	m_peripheral.push_back(ConnectedPeripheral(peripheral, base, length));
}

uint8_t
Memory::ReadByte(addr_t addr)
{
	for (TConnectedPeripheralList::iterator it = m_peripheral.begin(); it != m_peripheral.end(); it++) {
		if (it->IsAddressClaimed(addr))
			return it->Peek(addr);
	}
	/* No peripherals here, must be plain memory */
	return m_Memory[addr];
}

void
Memory::WriteByte(addr_t addr, uint8_t val)
{
	for (TConnectedPeripheralList::iterator it = m_peripheral.begin(); it != m_peripheral.end(); it++) {
		if (!it->IsAddressClaimed(addr))
			continue;
		it->Poke(addr, val);
		return;
	}
	/* No peripherals here, must be plain memory */
	if (!IsReadOnly(addr)) {
		m_Memory[addr] = val;
	}
}

uint16_t
Memory::ReadWord(addr_t addr)
{
	return ReadByte(addr) | (uint16_t)ReadByte(addr + 1) << 8;
}

void
Memory::WriteWord(addr_t addr, uint16_t val)
{
	WriteByte(addr, val & 0xff);
	WriteByte(addr + 1, val >> 8);
}

bool
Memory::ReadFromFile(const char* filename, addr_t base, uint16_t length)
{
	FILE* f = fopen(filename, "rb");
	if (f == NULL)
		return false;

	bool ok = fread((void*)&m_Memory[base], length, 1, f) > 0;

	fclose(f);
	return ok;
}

uint8_t&
Memory::operator[](addr_t addr)
{
	return *(uint8_t*)&m_Memory[addr];
}

void
Memory::Reset()
{
	for (TConnectedPeripheralList::iterator it = m_peripheral.begin(); it != m_peripheral.end(); it++) {
		it->Reset();
	}
}

bool
Memory::IsReadOnly(addr_t addr) const
{
	/* XXX We must check whether these are available! */
	if (addr >= s_basic_base && addr < s_basic_base + s_basic_length)
		return true;
	if (addr >= s_kernal_base && addr < s_kernal_base + s_basic_length)
		return true;
	if (addr >= s_charrom_base && addr < s_charrom_base + s_basic_length)
		return true;
	return false;
}

/* vim:set ts=2 sw=2: */
