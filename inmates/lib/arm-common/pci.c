/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Copyright (c) Retotech AB, 2017
 * Copyright (c) Siemens AG, 2017
 *
 * Authors:
 *  Jonas Weståker <jonas@retotech.se>
 *  Henning Schild <henning.schild@siemens.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 * Alternatively, you can use or redistribute this file under the following
 * BSD license:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <inmate.h>

#define PCI_MMIO_SIZE	0xfffff
static u32 mmio_base;

void pci_set_mmio_base(u32 base)
{
	mmio_base = base;
}

u32 pci_read_config(u16 bdf, unsigned int addr, unsigned int size)
{
	u32 reg_addr = mmio_base | ((u32)bdf << 8) | (addr & 0xfc);
	if (!mmio_base || reg_addr + size >= (mmio_base + PCI_MMIO_SIZE))
		return -1;

	switch (size) {
	case 1:
		return mmio_read8((u8 *)(reg_addr + (addr & 0x3)));
	case 2:
		return mmio_read16((u16 *)(reg_addr + (addr & 0x3)));
	case 4:
		return mmio_read32((u32 *)(reg_addr));
	default:
		return -1;
	}
}

void pci_write_config(u16 bdf, unsigned int addr, u32 value, unsigned int size)
{
	u32 reg_addr = mmio_base | ((u32)bdf << 8) | (addr & 0xfc);
	if (!mmio_base || reg_addr + size >= (mmio_base + PCI_MMIO_SIZE))
		return;
	switch (size) {
	case 1:
		mmio_write8((u8 *)(reg_addr + (addr & 0x3)), value);
		break;
	case 2:
		mmio_write16((u16 *)(reg_addr + (addr & 0x3)), value);
		break;
	case 4:
		mmio_write32((u32 *)(reg_addr), value);
		break;
	}
}
