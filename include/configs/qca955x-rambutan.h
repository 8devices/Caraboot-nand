/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

/*
 * This file contains the configuration parameters for the 8Devices Rambutan board.
 */

#ifndef __RAMBUTAN_CONFIG_H
#define __RAMBUTAN_CONFIG_H

#define CONFIG_ATHEROS		1
#define CONFIG_SOC_QCA955X	1
#define CONFIG_MACH_QCA955x	1
#define CFG_PLL_FREQ		CFG_PLL_720_600_200

#define CONFIG_ATH_NAND_BR 1		/* NAND BootROM enabled */
#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_SYS_GENERIC_GLOBAL_DATA

#undef DEBUG

// NAND
#define CONFIG_NAND_ATHEROS 1
#define CONFIG_SYS_MAX_NAND_DEVICE 1
#define CONFIG_SYS_MAX_NAND_CHIPS 1
#define CONFIG_SYS_NAND_SELF_INIT 1
#define CONFIG_QCA_NAND_IMG 		/* Build combined NAND image (sbl+uboot) */

// UBI
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
// Libs for UBI
#define CONFIG_RBTREE 
#define CONFIG_LZO


// MTD
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define MTDPARTS_DEFAULT	"mtdparts=ath-nand:3M(u-boot),2M(u-boot-env),1M(art),122M(ubi)"
#define MTDIDS_DEFAULT		"nand0=ath-nand"

// UART 
#define CONFIG_ATH79_SERIAL 1

// ETH
#define CONFIG_ATHRS_GMAC
#define CONFIG_ATHRS_GMAC_955X

// #define CONFIG_ATHRS17_PHY
#define CONFIG_PHY_AR803X
#define CONFIG_ATHR_8033_PHY
#define CONFIG_AR8035_PHY_ADDR 0

#define CONFIG_SYS_RX_ETH_BUFFER 8
#define CFG_ATH_GMAC_NMACS 2
#define CONFIG_ATHRS_GMAC_RGMII  1
// #define ATH_RGMII_CAL
#define CONFIG_MGMT_INIT
#define ATH_MDC_GPIO
#define ATH_GMAC_GE1_IS_CONNECTED 1
#define CONFIG_PHY_ATHEROS
#define CONFIG_PHYLIB
#define CONFIG_ATHRS_GMAC_SGMII

// PCI
#define CONFIG_PCI
#define CONFIG_ATH_PCI
#define CONFIG_PCI_PLATDEV_INIT_NOOP
#define CONFIG_CMD_PCI


// Factory
#define CONFIG_FACTORY_MODE
#define CONFIG_FACTORY_IMG_FILENAME 	"rambutan.bin"
#define CONFIG_FACTORY_IMG_LOAD_ADDR	"0x82000000"

#define CONFIG_IPADDR			192.168.2.100
#define CONFIG_SERVERIP			192.168.2.254

#define CONFIG_LZMA
#define CONFIG_SYS_BOOTM_LEN (32 << 20) 	/* max uImage size 32MB */

#define CONFIG_ATH79_WATCHDOG

#define CONFIG_SHOW_ACTIVITY

// ENV
// #define CONFIG_ENV_IS_IN_UBI
// #define CONFIG_ENV_UBI_PART "ubi"
// #define CONFIG_ENV_UBI_VOLUME "uboot-env-1"
// #define CONFIG_ENV_UBI_VOLUME_REDUND "uboot-env-2"
// #define	CONFIG_ENV_IS_NOWHERE	1

#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_ENV_SIZE			0x20000  /*128kB*/
#define CONFIG_ENV_OFFSET		0x300000 /*3MB*/
#define CONFIG_ENV_RANGE		8 * CONFIG_ENV_SIZE

#define CONFIG_ENV_OFFSET_REDUND	CONFIG_ENV_OFFSET + CONFIG_ENV_RANGE
#define CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE

#define CONFIG_BOOTCOUNT_LIMIT
#define CONFIG_BOOTCOUNT_ENV

// FIT image format
#define CONFIG_FIT

// #define CONFIG_SYS_NO_FLASH 1
#define CONFIG_CMDLINE_EDITING 1 
#define CONFIG_AUTO_COMPLETE
#define CONFIG_BOOTDELAY	1	/* autoboot after 1 seconds	*/
#define CONFIG_BAUDRATE		115200	/* valid baudrates */

#define	CONFIG_TIMESTAMP		/* Print image info with timestamp */
#undef	CONFIG_BOOTARGS

/*
 * Command line configuration.
 */

#define CONFIG_CMD_MII
#define CONFIG_CMD_ATHREG
#define CONFIG_NET_RETRY_COUNT 1
#define	CONFIG_DISPLAY_BOARDINFO

// #undef CONFIG_CMD_FLASH

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_SYS_LONGHELP				/* undef to save memory      */

#define	CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size   */
#define	CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)  /* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS		16		/* max number of command args*/
#define	CONFIG_SYS_CONSOLE_INFO_QUIET

#define CONFIG_SYS_MALLOC_LEN		1*1024*1024
#define CONFIG_SYS_BOOTPARAMS_LEN	128*1024
#define CONFIG_SYS_MHZ			(720 / 2)  /* Timer runs at half CPU clock speed */
#define CONFIG_SYS_MIPS_TIMER_FREQ	(CONFIG_SYS_MHZ * 1000000)

#define CONFIG_SYS_SDRAM_BASE		0x80000000     /* Cached addr */
#define	CONFIG_SYS_LOAD_ADDR		0x81000000     /* default load address	*/

#define CONFIG_SYS_MEMTEST_START	0x80100000
#define CONFIG_SYS_MEMTEST_END		0x80800000

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#define CONFIG_SYS_MAX_FLASH_SECT	(512)	/* max number of sectors on one chip */

#define PHYS_FLASH_1		0x9f000000 /* Flash Bank #1 */
#define CONFIG_SYS_FLASH_BANKS_LIST {PHYS_FLASH_1}


/* The following #defines are needed to get flash environment right */
//#define	CONFIG_SYS_TEXT_BASE 0xbfc00000
#define	CONFIG_SYS_MONITOR_BASE	CONFIG_SYS_TEXT_BASE

#define	CFG_MONITOR_BASE CONFIG_SYS_MONITOR_BASE
//#define	CONFIG_SYS_MONITOR_LEN		(192 << 10)
#define CONFIG_SYS_INIT_SP_OFFSET	0x1000


#define CONFIG_RESET_BUTTON_GPIO 19
#define CONFIG_PHY_RESET_GPIO 17
#define CONFIG_PHY1_RESET_GPIO 23
// TODO fixup spi flash driver
#define CONFIG_SPI_ATH_FLASH
#define CFG_FLASH_BASE 0x9f000000
#define CFG_MAX_FLASH_BANKS 1
#define CFG_FLASH_SIZE (16*1024*1024)
#define CFG_FLASH_SECTOR_SIZE (64*1024)


#define CONFIG_EXTRA_ENV_SETTINGS								\
	"bootcmd=run setup && run bootlinux\0"							\
	"altbootcmd=if test $changed = 0; then run do_change; else run do_recovery; fi\0"	\
	"setup=if test $active = 1; then run setup1; else run setup2; fi\0" 			\
	"setup1=partname=1 && setenv bootargs ${args_common} ${mtdparts} ${args1}\0"		\
	"setup2=partname=2 && setenv bootargs ${args_common} ${mtdparts} ${args2}\0"		\
	"bootlinux=run boot1 boot2 boot3 boot4 || reset\0"					\
	"boot1=echo Booting from partition: ${partname}\0"					\
	"boot2=ubi part ubi\0"									\
	"boot3=ubi readvol ${loadaddr} kernel${partname}\0"					\
	"boot4=bootm ${loadaddr}\0"								\
	"do_change=run change1 change2 change3; reset\0"					\
	"change1=if test $active = 1; then setenv active 2; else setenv active 1; fi\0"		\
	"change2=setenv bootcount; setenv changed 1; saveenv\0"					\
	"change3=echo Active partition changed to [$active]\0"					\
	"do_recovery=run rec1 rec2 rec3 rec4 rec5 rec6 rec7 rec8 rec9 rec10 rec11; reset\0"	\
	"rec1=echo Doing firmware recovery!\0"							\
	"rec2=setenv active 1 && setenv changed 0 && setenv bootcount 0\0"			\
	"rec3=saveenv\0"									\
	"rec4=tftpboot ${tftp_loadaddr} ${recovery_file}\0"					\
	"rec5=ubi part ubi\0"									\
	"rec6=imxtract ${tftp_loadaddr} rootfs $loadaddr\0"					\
	"rec7=ubi writevol ${loadaddr} rootfs1 ${filesize}\0"					\
	"rec8=ubi writevol ${loadaddr} rootfs2 ${filesize}\0"					\
	"rec9=imxtract ${tftp_loadaddr} kernel ${loadaddr}\0"					\
	"rec10=ubi writevol ${loadaddr} kernel1 ${filesize}\0"					\
	"rec11=ubi writevol ${loadaddr} kernel2 ${filesize}\0"					\
	"do_ubi_init=ubi part ubi; run ubii1 ubii2 ubii3\0"					\
	"ubii1=ubi create kernel1 300000 s; ubi create kernel2 300000 s\0"			\
	"ubii2=ubi create rootfs1 2000000 s; ubi create rootfs2 2000000 s\0"			\
	"ubii3=ubi create data 2A00000 d; ubi create cfg 300000 d\0"				\
	"bootlimit=3\0"										\
	"bootcount=0\0"										\
	"upgrade_available=1\0"									\
	"changed=0\0"										\
	"netretry=2\0"										\
	"loadaddr=0x82000000\0"									\
	"tftp_loadaddr=0x80060000\0"								\
	"bootfile=fwupdate.bin\0"								\
	"recovery_file=fwupdate.bin\0"								\
	"mtdparts="MTDPARTS_DEFAULT"\0" 							\
	"args_common=console=ttyS0,115200 init=/linuxrc\0" 					\
	"args1=ubi.mtd=3 root=ubi0:rootfs1 rootfstype=ubifs ro\0"				\
	"args2=ubi.mtd=3 root=ubi0:rootfs2 rootfstype=ubifs ro\0"				\
	"tftptimeout=5000\0"


#endif	/* __RAMBUTAN_CONFIG_H */
