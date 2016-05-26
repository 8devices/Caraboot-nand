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
#define MTDPARTS_DEFAULT	"mtdparts=ar934x-nfc:3M(u-boot),2M(u-boot-env),1M(art),122M(ubi)"
#define MTDIDS_DEFAULT		"nand0=ar934x-nfc"

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

//USB
#define CONFIG_USB_EHCI_QCA
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#define CONFIG_EHCI_DESC_BIG_ENDIAN
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_EHCI_IS_TDI

//USB_BOOT
#define CONFIG_USB_BOOT
#define CFG_USB_BOOT_MAX_PARTITIONS_SCAN        16
#define CFG_USB_RECOVERY_LOAD_ADDR              0x80060000
#define CFG_MAX_USB_RECOVERY_FILE_SIZE          0x07a00000 /* 122MB */
#define CFG_USB_BOOT_FILENAME                   "8dev_uimage.bin"
#define CFG_USB_RECOVERY_FILENAME               "8dev_recovery.bin"
#define CFG_USB_RECOVERY_FW_PART_NAME      	"ubi"

//FS
#define CONFIG_FS_FAT
#define CONFIG_CMD_FAT
#define CONFIG_FS_EXT4
#define CONFIG_CMD_EXT4
#define CONFIG_DOS_PARTITION
#define CONFIG_EFI_PARTITION
#define CONFIG_PARTITION_UUIDS

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

#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_ENV_SIZE			0x20000  /*128kB*/
#define CONFIG_ENV_OFFSET		0x300000 /*3MB*/
#define CONFIG_ENV_RANGE		8 * CONFIG_ENV_SIZE

#define CONFIG_ENV_OFFSET_REDUND	CONFIG_ENV_OFFSET + CONFIG_ENV_RANGE
#define CONFIG_ENV_SIZE_REDUND		CONFIG_ENV_SIZE
#define CONFIG_ENV_OVERWRITE		/* allow env overwriting */

#define CONFIG_BOOTCOUNT_LIMIT
#define CONFIG_BOOTCOUNT_ENV

// FIT image format
#define CONFIG_FIT

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


#define CONFIG_RESET_BUTTON_GPIO 18
#define CONFIG_PHY_RESET_GPIO 17
#define CONFIG_PHY1_RESET_GPIO 23

// Disable SPI flash driver
// #define CONFIG_SPI_ATH_FLASH
// #define CFG_FLASH_BASE 0x9f000000
// #define CFG_MAX_FLASH_BANKS 1
// #define CFG_FLASH_SIZE (16*1024*1024)
// #define CFG_FLASH_SECTOR_SIZE (64*1024)
#define CONFIG_SYS_NO_FLASH
#undef CONFIG_CMD_IMLS

#define CONFIG_EXTRA_ENV_SETTINGS								\
	"bootcmd=run setup && run bootlinux\0"							\
	"setup=setenv bootargs ${args_common} ${mtdparts} ${args}\0"				\
	"bootlinux=run boot0 boot1 boot2 boot3 || reset\0"					\
	"boot0=usb_boot_file\0"									\
	"boot1=ubi part ubi\0"									\
	"boot2=ubi readvol ${loadaddr} kernel\0"						\
	"boot3=bootm ${loadaddr}\0"								\
	"bootlimit=3\0"										\
	"bootcount=0\0"										\
	"upgrade_available=0\0"									\
	"changed=0\0"										\
	"netretry=3\0"										\
	"loadaddr=0x82000000\0"									\
	"tftp_loadaddr=0x80060000\0"								\
	"mtdparts="MTDPARTS_DEFAULT"\0" 							\
	"args_common=board=RAMBUTAN console=ttyS0,115200\0" 					\
	"args=ubi.mtd=3 ro\0"									\
	"tftptimeout=5000\0"

#define CONFIG_CARABOOT_RELEASE "v3.1-dev"

#endif	/* __RAMBUTAN_CONFIG_H */
