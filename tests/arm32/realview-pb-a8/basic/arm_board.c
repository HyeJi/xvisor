/**
 * Copyright (c) 2012 Jean-Christophe Dubois.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * @file arm_board.c
 * @author Jean-Christophe Dubois (jcd@tribudubois.net)
 * @brief various platform specific functions
 */

#include <arm_types.h>
#include <arm_io.h>
#include <arm_board.h>
#include <arm_plat.h>
#include <arm_string.h>
#include <pic/gic.h>
#include <serial/pl01x.h>

void arm_board_reset(void)
{
        arm_writel(0x0,
                   (void *)(REALVIEW_SYS_BASE+ REALVIEW_SYS_RESETCTL_OFFSET));
        arm_writel(0x04,
                   (void *)(REALVIEW_SYS_BASE+ REALVIEW_SYS_RESETCTL_OFFSET));
}

void arm_board_init(void)
{
	/* Unlock Lockable reigsters */
	arm_writel(REALVIEW_SYS_LOCKVAL,
                   (void *)(REALVIEW_SYS_BASE + REALVIEW_SYS_LOCK_OFFSET));
}

char *arm_board_name(void)
{
	return "ARM Realview-PB-A8";
}

u32 arm_board_ram_start(void)
{
	return 0x70000000;
}

u32 arm_board_ram_size(void)
{
	return 0x6000000;
}

u32 arm_board_linux_machine_type(void)
{
	return 0x769;
}

void arm_board_linux_default_cmdline(char *cmdline, u32 cmdline_sz)
{
	arm_strcpy(cmdline, "root=/dev/ram rw earlyprintk console=ttyAMA0");
	/* VirtIO Network Device */
	arm_strcat(cmdline, " virtio_mmio.device=4K@0x20100000:34");
	/* VirtIO Block Device */
	arm_strcat(cmdline, " virtio_mmio.device=4K@0x20200000:35");
	/* VirtIO Console Device */
	arm_strcat(cmdline, " virtio_mmio.device=4K@0x20300000:41");
}

u32 arm_board_flash_addr(void)
{
	return (u32)(REALVIEW_FLASH0_BASE);
}

u32 arm_board_iosection_count(void)
{
	return 6;
}

physical_addr_t arm_board_iosection_addr(int num)
{
	physical_addr_t ret = 0;

	switch (num) {
	case 0:
		ret = REALVIEW_SYS_BASE;
		break;
	case 1:
		ret = REALVIEW_GIC_CPU_BASE;
		break;
	case 2:
	case 3:
	case 4:
	case 5:
		ret = REALVIEW_FLASH0_BASE + (num - 2) * 0x100000;
		break;
	default:
		while (1);
		break;
	}

	return ret;
}

u32 arm_board_pic_nr_irqs(void)
{
	return NR_IRQS_PBA8;
}

int arm_board_pic_init(void)
{
	int rc;

	/*
	 * Initialize Generic Interrupt Controller
	 */
	rc = gic_dist_init(0, REALVIEW_GIC_DIST_BASE, IRQ_PBA8_GIC_START);
	if (rc) {
		return rc;
	}
	rc = gic_cpu_init(0, REALVIEW_GIC_CPU_BASE);
	if (rc) {
		return rc;
	}

	return 0;
}

u32 arm_board_pic_active_irq(void)
{
	return gic_active_irq(0);
}

int arm_board_pic_ack_irq(u32 irq)
{
	return 0;
}

int arm_board_pic_eoi_irq(u32 irq)
{
	return gic_eoi_irq(0, irq);
}

int arm_board_pic_mask(u32 irq)
{
	return gic_mask(0, irq);
}

int arm_board_pic_unmask(u32 irq)
{
	return gic_unmask(0, irq);
}

#define	PBA8_UART_BASE			0x10009000
#define	PBA8_UART_TYPE			PL01X_TYPE_1
#define	PBA8_UART_INCLK			24000000
#define	PBA8_UART_BAUD			115200

int arm_board_serial_init(void)
{
	pl01x_init(PBA8_UART_BASE, 
			PBA8_UART_TYPE, 
			PBA8_UART_BAUD, 
			PBA8_UART_INCLK);

	return 0;
}

void arm_board_serial_putc(char ch)
{
	if (ch == '\n') {
		pl01x_putc(PBA8_UART_BASE, PBA8_UART_TYPE, '\r');
	}
	pl01x_putc(PBA8_UART_BASE, PBA8_UART_TYPE, ch);
}

char arm_board_serial_getc(void)
{
	char ch = pl01x_getc(PBA8_UART_BASE, PBA8_UART_TYPE);
	if (ch == '\r') {
		ch = '\n';
	}
	arm_board_serial_putc(ch);
	return ch;
}

