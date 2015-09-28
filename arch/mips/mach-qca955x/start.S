/*
 *  Startup Code for MIPS32 CPU-core
 *
 *  Copyright (c) 2003	Wolfgang Denk <wd@denx.de>
 *  Copyright (c) 2013 Qualcomm Atheros, Inc.
 * 
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#ifdef CONFIG_ATHEROS
#	include <atheros.h>
#else
#	include <ar7240_soc.h>
#	define ATH_SPI_CLOCK	0xbf000004
#endif


#define RVECENT(f,n)		\
	b	f;	nop

#define XVECENT(f,bev)		\
	b	f;		\
	li	k0,	bev

	.set noreorder
#ifdef COMPRESSED_UBOOT
	.align 4
#endif

	.globl _start
	.text
_start:
#ifndef COMPRESSED_UBOOT
	/* U-boot entry point */
	b	reset
	 nop

	.org 0x10
#if defined(CONFIG_SYS_XWAY_EBU_BOOTCFG)
	/*
	 * Almost all Lantiq XWAY SoC devices have an external bus unit (EBU) to
	 * access external NOR flashes. If the board boots from NOR flash the
	 * internal BootROM does a blind read at address 0xB0000010 to read the
	 * initial configuration for that EBU in order to access the flash
	 * device with correct parameters. This config option is board-specific.
	 */
	.word CONFIG_SYS_XWAY_EBU_BOOTCFG
	.word 0x0
#elif defined(CONFIG_MALTA)
	/*
	 * Linux expects the Board ID here.
	 */
	.word 0x00000420	# 0x420 (Malta Board with CoreLV)
	.word 0x00000000
#endif

	.org 0x200
	/* TLB refill, 32 bit task */
1:	b	1b
	 nop

	.org 0x280
	/* XTLB refill, 64 bit task */
1:	b	1b
	 nop

	.org 0x300
	/* Cache error exception */
1:	b	1b
	 nop

	.org 0x380
	/* General exception */
1:	b	1b
	 nop

	.org 0x400
	/* Catch interrupt exceptions */
1:	b	1b
	 nop

	.org 0x480
	/* EJTAG debug exception */
1:	b	1b
	 nop

	.align 4
reset:
#if !defined(CONFIG_ATH_NAND_BR)
	/*
	 * Clearing CP0 registers - This is generally required for the MIPS-24k
	 * core used by Atheros.
	 */
	mtc0	zero, $0
	mtc0	zero, $1
	mtc0	zero, $2
	mtc0	zero, $3
	mtc0	zero, $4
	mtc0	zero, $5
	mtc0	zero, $6
	mtc0	zero, $7
	mtc0	zero, $8
	mtc0	zero, $9
	mtc0	zero, $10
	mtc0	zero, $11
	li	t0, 0x10000004
	mtc0	t0, $12
	mtc0	zero, $13
	mtc0	zero, $14
	mtc0	zero, $15
	mtc0	zero, $16

#ifdef CONFIG_MACH_QCA955x
/*
 * Workaround recommnded by MIPS for the Scorpion Freeze issue
 */
#define CONFIG_SCO_JRCD		1
#define CONFIG_SCO_IAGN		!CONFIG_SCO_JRCD

#if CONFIG_SCO_IAGN && CONFIG_SCO_JRCD
#error Both CONFIG_SCO_IAGN and CONFIG_SCO_JRCD set
#endif

#if CONFIG_SCO_JRCD
	/*
	 * JR Cache Prediction Disable. Disables JR target address prediction.
	 * Bit [0], CP0 Register 16, Select 6
	 *	0 - JR cache target address prediction is enabled.
	 *	1 - JR cache target address prediction is not enabled.
	 */65regr
	mfc0	t0,	$16,	6
	li	t1,	(1 << 0)
	or	t0,	t0,	t1
	mtc0	t0,	$16,	6
#endif

#if CONFIG_SCO_IAGN
	/*
	 * Bit [25], CP0 Register 16, Select 7
	 * Selective control of out-of-order behavior: issue ALU-side or
	 * load/store-side instructions (respectively) in program order.
	 */
	mfc0	t0,	$16,	7
	li	t1,	(1 << 25)
	or	t0,	t0,	t1
	mtc0	t0,	$16,	7
#endif
#endif /* CONFIG_MACH_QCA955x */

#define pref_on		0

#if pref_on
#define prefetch_val	3
	mfc0	t0,	$16,	7
	li	t1,	~(3 << 11)
	and	t0,	t0,	t1
	li	t1,	(prefetch_val << 11)
	or	t0,	t0,	t1
	mtc0	t0,	$16,	7
#endif

#if !defined(CONFIG_WASP_SUPPORT)
	mtc0	zero, $17
#endif
	mtc0	zero, $18
	mtc0	zero, $19
#if !defined(CONFIG_WASP_SUPPORT)
	mtc0	zero, $20
	mtc0	zero, $21
	mtc0	zero, $22
#endif
#ifndef CONFIG_HORNET_EMU
	mtc0	zero, $23
#endif
	mtc0	zero, $24
	mtc0	zero, $25
	mtc0	zero, $26
	mtc0	zero, $27
	mtc0	zero, $28
#if defined(CONFIG_WASP_SUPPORT) || defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA953x)
	mtc0	zero, $29	# C0_TagHi
	mtc0	zero, $28, 2	# C0_DTagLo
	mtc0	zero, $29, 2	# C0_DTagHi
#endif


	/*
	 * Clear watch registers.
	 */

	mtc0	zero, CP0_WATCHLO
	mtc0	zero, CP0_WATCHHI

	/* STATUS register */
	mfc0	k0, CP0_STATUS
	li	k1, ~ST0_IE
	and	k0, k1
        mtc0	zero, CP0_CAUSE
	mtc0	k0, CP0_STATUS

	/* CAUSE register */
	mtc0	zero, CP0_CAUSE

	/* Init Timer */
	mtc0	zero, CP0_COUNT
	mtc0	zero, CP0_COMPARE

	/* CONFIG0 register */
	li	t0, CONF_CM_UNCACHED
	mtc0	t0, CP0_CONFIG
#endif /* !defined(CONFIG_ATH_NAND_BR) */
#endif /* #ifndef COMPRESSED_UBOOT */

#ifdef CONFIG_ATH_NAND_BR
	/*
	 * Clear WDT that was setup in first stage for
	 * NAND Boot-ROM
	 */
	li	t0,	0xb8060008
	sw	zero,	0(t0)
#endif /* CONFIG_ATH_NAND_BR */

    /* Initialize GOT pointer.*/
	bal     1f
	nop
	.word   _gp
	1:
	move    gp, ra
	lw      t1, 0(ra)
	move	gp, t1

#ifndef COMPRESSED_UBOOT


/**************************************************************************/

	/* Initialize any external memory.
	 */
#if defined(CONFIG_AR7100) || defined(CONFIG_AR7240) || defined(CONFIG_ATHEROS)
	la      t9, lowlevel_init
	jalr    t9
	nop
	nop
#if defined(CONFIG_MACH_HORNET)
	la      t9, hornet_ddr_init
	jalr    t9
	nop
	nop
#endif
	la	t9, rel_start
	jalr	t9
	nop
#endif

rel_start:
#if defined(CONFIG_AR7100) || defined(CONFIG_AR7240) || defined(CONFIG_ATHEROS)
	/* REMAP_DISABLE */
	li	a0,	KSEG1ADDR(ATH_SPI_CLOCK)
#if defined(CONFIG_MACH_QCA955x) || defined(CONFIG_MACH_QCA953x)
	li	t0,	0x246
#elif defined(CONFIG_WASP_SUPPORT)
	li	t0,	0x243
#else
	li	t0,	0x43
#endif
	sw	t0,	0(a0)
#endif


#if defined(CONFIG_AR9100) && defined(CFG_HOWL_1_2)
	/* Disable remap for parallel flash */
	li	t7, AR9100_FLASH_CONFIG;
	lw	t8, 0(t7);
	li	t9, 0xffbf0000;
	and	t8, t8, t9;
	li	t9, 0x22fc;
	or	t8, t8, t9;
	li	t9, 0xffcfffff; /* scale = 0 */
	and	t8, t8, t9;
	sw	t8, 0(t7);
#endif

	/* Initialize caches...
	 */
	la      t9, simple_mips_cache_reset
	jalr    t9
	nop

	/* ... and enable them.
	 */
#if defined(CONFIG_WASP_SUPPORT) && !defined(CONFIG_ATH_NAND_BR)
	li	t7,	KSEG1ADDR(AR7240_REV_ID)
	lw	t7,	0(t7)
	andi	t9,	t7,	0xf
	bne	zero,	t9,	1f
	nop

	li	t0,	CONF_CM_UNCACHED
	j	2f
	nop
#endif

1:	li	t0,	CONF_CM_CACHABLE_NONCOHERENT
2:	mtc0	t0,	CP0_CONFIG

#if !defined(CFG_INIT_STACK_IN_SRAM)
#if !defined(CONFIG_AR7100) && !defined(CONFIG_AR7240)
	/* Set up temporary stack.
	 */
	li	a0, CFG_INIT_SP_OFFSET
	la      t9, mips_cache_lock
	jalr    t9
	nop
#endif

#if defined(CONFIG_AR7100) || defined(CONFIG_AR7240)
	la	t9, mips_cache_lock_24k
	jalr	t9
	nop
#endif
#endif /* !CFG_INIT_STACK_IN_SRAM */
#endif /* #ifndef COMPRESSED_UBOOT */

#if defined(CONFIG_WASP_SUPPORT) || defined(CFG_INIT_STACK_IN_SRAM)
	li	t0, CFG_INIT_SRAM_SP_OFFSET	/* Setup stack in SRAM */
#else
	li	t0, CFG_SDRAM_BASE + CFG_INIT_SP_OFFSET
#endif
	la	sp, 0(t0)

	la	t9, board_init_f
	j	t9
	nop

/*
 * void relocate_code (addr_sp, gd, addr_moni)
 *
 * This "function" does not return, instead it continues in RAM
 * after relocating the monitor code.
 *
 * a0 = addr_sp
 * a1 = gd
 * a2 = destination address
 */
	.globl	relocate_code
	.ent	relocate_code
relocate_code:
	move	sp, a0			# set new stack pointer

	move	s0, a1			# save gd in s0
	move	s2, a2			# save destination address in s2

	li	t0, CONFIG_SYS_MONITOR_BASE
	sub	s1, s2, t0		# s1 <-- relocation offset

	la	t3, in_ram
	lw	t2, -12(t3)		# t2 <-- __image_copy_end
	move	t1, a2

	add	gp, s1			# adjust gp

	/*
	 * t0 = source address
	 * t1 = target address
	 * t2 = source end address
	 */
1:
	lw	t3, 0(t0)
	sw	t3, 0(t1)
	addu	t0, 4
	blt	t0, t2, 1b
	 addu	t1, 4

	/* If caches were enabled, we would have to flush them here. */
	sub	a1, t1, s2		# a1 <-- size
	la	t9, flush_cache
	jalr	t9
	 move	a0, s2			# a0 <-- destination address

	/* Jump to where we've relocated ourselves */
	addi	t0, s2, in_ram - _start
	jr	t0
	 nop

	.word	__rel_dyn_end
	.word	__rel_dyn_start
	.word	__image_copy_end
	.word	_GLOBAL_OFFSET_TABLE_
	.word	num_got_entries

in_ram:
	/*
	 * Now we want to update GOT.
	 *
	 * GOT[0] is reserved. GOT[1] is also reserved for the dynamic object
	 * generated by GNU ld. Skip these reserved entries from relocation.
	 */
	lw	t3, -4(t0)		# t3 <-- num_got_entries
	lw	t8, -8(t0)		# t8 <-- _GLOBAL_OFFSET_TABLE_
	add	t8, s1			# t8 now holds relocated _G_O_T_
	addi	t8, t8, 8		# skipping first two entries
	li	t2, 2
1:
	lw	t1, 0(t8)
	beqz	t1, 2f
	 add	t1, s1
	sw	t1, 0(t8)
2:
	addi	t2, 1
	blt	t2, t3, 1b
	 addi	t8, 4

	/* Update dynamic relocations */
	lw	t1, -16(t0)		# t1 <-- __rel_dyn_start
	lw	t2, -20(t0)		# t2 <-- __rel_dyn_end

	b	2f			# skip first reserved entry
	 addi	t1, 8

1:
	lw	t8, -4(t1)		# t8 <-- relocation info

	li	t3, 3
	bne	t8, t3, 2f		# skip non R_MIPS_REL32 entries
	 nop

	lw	t3, -8(t1)		# t3 <-- location to fix up in FLASH

	lw	t8, 0(t3)		# t8 <-- original pointer
	add	t8, s1			# t8 <-- adjusted pointer

	add	t3, s1			# t3 <-- location to fix up in RAM
	sw	t8, 0(t3)

2:
	blt	t1, t2, 1b
	 addi	t1, 8			# each rel.dyn entry is 8 bytes

	/*
	 * Clear BSS
	 *
	 * GOT is now relocated. Thus __bss_start and __bss_end can be
	 * accessed directly via $gp.
	 */
	la	t1, __bss_start		# t1 <-- __bss_start
	la	t2, __bss_end		# t2 <-- __bss_end

1:
	sw	zero, 0(t1)
	blt	t1, t2, 1b
	 addi	t1, 4

	move	a0, s0			# a0 <-- gd
	la	t9, board_init_r
	jr	t9
	 move	a1, s2

	.end	relocate_code

#ifndef COMPRESSED_UBOOT
	/* Exception handlers.
	 */
romReserved:
	b romReserved

romExcHandle:
	b romExcHandle
#endif /* #ifndef COMPRESSED_UBOOT */
