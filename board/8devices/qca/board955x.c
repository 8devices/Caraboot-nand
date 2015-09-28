/*
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
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

#include <common.h>
#include <command.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <config.h>
#include <version.h>
#include <atheros.h>

#include <device_mode.h>
int wasp_boot_status = 0;  // Global variable to indicate if boot is succesful
			  // negative values show failure
typedef struct gpio_led_desc {
  int id;
  int bit;	//GPIO bit
  int polarity; //0 - high active; 1 - low active
};

typedef struct switch_led_desc {
  int id;
  int offset1;
  int offset2;
  int offset3;
};

#if defined (CONFIG_FWBD2900) || defined (CONFIG_FWBD3000) || defined (CONFIG_FWBD2901)

#define ALL_LED_GPIO 0xF00000
static int  led_count=4;
static struct gpio_led_desc leds[] = {
	{ .id=0, .bit=20, .polarity=1 }, // LED_POWER
	{ .id=1, .bit=22, .polarity=1 }, // LED_ETH_GREEN
	{ .id=2, .bit=21, .polarity=1 }, // LED_ETH_YELLOW
	{ .id=3, .bit=23, .polarity=1 }, // LED_WLAN
};

static int  switch_led_count=3;
static struct switch_led_desc switch_leds[] = {
	{ .id=0, .offset1=8, .offset2=14, .offset3=20 }, // LED_SW_0
	{ .id=1, .offset1=10, .offset2=16, .offset3=22 }, // LED_SW_1
	{ .id=2, .offset1=12, .offset2=18, .offset3=24 }, // LED_SW_2
};

#else defined (CONFIG_FWBD2800)
#define ALL_LED_GPIO 0
static int  led_count=0;
static struct gpio_led_desc leds[] = {
//	{ .id=0, .bit=1, .polarity=1 }, // LEDX
};
#endif

extern int ath_ddr_initial_config(uint32_t refresh);
extern int ath_ddr_find_size(void);

#ifdef COMPRESSED_UBOOT
#	define prmsg(...)
#	define args		char *s
#	define board_str(a)	do {			\
	char ver[] = "0";				\
	strcpy(s, a " - Scorpion 1.");			\
	ver[0] += ath_reg_rd(RST_REVISION_ID_ADDRESS)	\
						& 0xf;	\
	strcat(s, ver);					\
} while (0)
#else
#	define prmsg	printf
#	define args		void
#	define board_str(a)				\
	printf(a " - Scorpion 1.%d", ath_reg_rd		\
			(RST_REVISION_ID_ADDRESS) & 0xf)
#endif

void
ath_usb1_initial_config(void)
{
#define unset(a)	(~(a))

	ath_reg_wr_nf(SWITCH_CLOCK_SPARE_ADDRESS,
		ath_reg_rd(SWITCH_CLOCK_SPARE_ADDRESS) |
		SWITCH_CLOCK_SPARE_USB_REFCLK_FREQ_SEL_SET(5));
	udelay(1000);

	ath_reg_rmw_set(RST_RESET_ADDRESS,
				RST_RESET_USB_PHY_SUSPEND_OVERRIDE_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_USB_PHY_RESET_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_USB_PHY_ARESET_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_USB_HOST_RESET_SET(1));
	udelay(1000);

	ath_reg_rmw_clear(RST_RESET_ADDRESS, RST_RESET_USB_PHY_PLL_PWD_EXT_SET(1));
	udelay(10);

	ath_reg_rmw_set(RST_RESET2_ADDRESS, RST_RESET2_USB1_EXT_PWR_SEQ_SET(1));
	udelay(10);
}

void
ath_usb2_initial_config(void)
{
	if (is_drqfn()) {
		return;
	}

	ath_reg_rmw_set(RST_RESET2_ADDRESS, RST_RESET2_USB2_MODE_SET(1));
	udelay(10);
	ath_reg_rmw_set(RST_RESET2_ADDRESS,
				RST_RESET2_USB_PHY2_SUSPEND_OVERRIDE_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET2_USB_PHY2_RESET_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET2_USB_PHY2_ARESET_SET(1));
	udelay(1000);
	ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET2_USB_HOST2_RESET_SET(1));
	udelay(1000);

	ath_reg_rmw_clear(RST_RESET2_ADDRESS, RST_RESET2_USB_PHY2_PLL_PWD_EXT_SET(1));
	udelay(10);

	ath_reg_rmw_set(RST_RESET2_ADDRESS, RST_RESET2_USB2_EXT_PWR_SEQ_SET(1));
	udelay(10);
}


void ath_gpio_config(void)
{
	/* disable the CLK_OBS on GPIO_4 and set GPIO4 as output for ext. watchdog */
	ath_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << 4));
	ath_reg_rmw_clear(GPIO_OUT_FUNCTION1_ADDRESS, GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_MASK);
	ath_reg_rmw_set(GPIO_OUT_FUNCTION1_ADDRESS, GPIO_OUT_FUNCTION1_ENABLE_GPIO_4_SET(0x80));
	ath_reg_rmw_set(GPIO_OUT_ADDRESS, (1 << 4));
	/* Set GPIO 13 as input for LED functionality to be OFF during bootup */
	ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 13));
	/* Turn off JUMPST_LED and 5Gz LED during bootup */
	ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 15));
	ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << 12));

#ifdef CONFIG_PHY_RESET_GPIO
	/* Reset PHY */
	ath_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << CONFIG_PHY_RESET_GPIO));
	ath_reg_rmw_clear(GPIO_OUT_ADDRESS, (1 << CONFIG_PHY_RESET_GPIO));
	udelay(100000);
	ath_reg_rmw_set(GPIO_OUT_ADDRESS, (1 << CONFIG_PHY_RESET_GPIO));
#endif

#ifdef CONFIG_PCIE0_RESET_GPIO
	/* Set PCIe0 HW RESET */
	ath_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << CONFIG_PCIE0_RESET_GPIO));
	ath_reg_rmw_clear(GPIO_OUT_ADDRESS, (1 << CONFIG_PCIE0_RESET_GPIO));
	udelay(100000);
	ath_reg_rmw_set(GPIO_OUT_ADDRESS, (1 << CONFIG_PCIE0_RESET_GPIO));
#endif

#ifdef CONFIG_PCIE1_RESET_GPIO
	/* Set PCIe1 HW RESET */
	ath_reg_rmw_clear(GPIO_OE_ADDRESS, (1 << CONFIG_PCIE1_RESET_GPIO));
	ath_reg_rmw_set(GPIO_OUT_ADDRESS, (1 << CONFIG_PCIE1_RESET_GPIO));
#endif

	/* GPIO16 input button */
	ath_reg_rmw_set(GPIO_OE_ADDRESS, (1 << CONFIG_RESET_BUTTON_GPIO));
	
	/* SETUP LED GPIO */
	ath_reg_rmw_clear(GPIO_OE_ADDRESS, ALL_LED_GPIO);
	ath_reg_wr_nf(GPIO_CLEAR_ADDRESS, ALL_LED_GPIO);
}

//functions for GPIO control
void show_boot_progress(int arg)
{
	debug("%s : %d arg: %d\n", __func__, __LINE__, arg);

	if (arg == BOOTSTAGE_ID_BOARD_INIT_R)		//RAM init OK
		gpio_led_switch(LED_WLAN ,0);		//RSSI4 off
	//else if (arg == BOOTSTAGE_ID_NET_ETH_INIT) 	//Flash, ethernet init OK
	else if (arg == BOOTSTAGE_ID_CHECK_ARCH) 	//Image Checksum OK
		gpio_led_switch(LED_ETH_YELLOW ,0);		//RSSI2 off
	//else if (arg == BOOTSTAGE_ID_DECOMP_SUCCESS)	//Uncompress OK
	else if (arg == BOOTSTAGE_ID_RUN_OS)		//Starting kernel..
		gpio_led_switch(LED_ETH_GREEN ,0);		//WLAN off
	else if (arg == IND_BL_AUTH_FAIL ||		//Set status for blinking
		 arg == IND_BL_TFTP_DOWNLOAD ||		//indications in show_activity()
		 arg == IND_BL_START_PROD ||
		 arg == IND_BL_TFTP_DOWNLOAD  ||
		 arg == IND_BL_DHCP_PROGRESS  ||
		 arg == 0)
		wasp_boot_status = arg;
	return;
}

void gpio_led_switch(int led_id, int state)
{
	// debug("%s : %d id: %d state: %d\n", __func__, __LINE__, led_id, state);

	int i;
	for (i=0; i<led_count;i++){
		if (leds[i].id==led_id) {
			if (leds[i].polarity ^ state){
				ath_reg_wr_nf(GPIO_SET_ADDRESS, (1<<leds[i].bit));
			}
			else{
				ath_reg_wr_nf(GPIO_CLEAR_ADDRESS, (1<<leds[i].bit));
			}
			break;
		}
	}
	return;
}

void switch_led_switch(int led_id, int state)
{
	// debug("%s : %d id: %d state: %d \n", __func__, __LINE__, led_id, state);

	unsigned int reg, reg_old, mask, val;
	int i;

	// STATES: OFF, ON, BLINK, DEFAULT
	unsigned int state_map[4] = {0b00, 0b10, 0b01, 0b11};

	for (i=0; i<switch_led_count;i++){
		if (switch_leds[i].id==led_id) {
			reg = athrs17_reg_read(0x5c);
			reg_old = reg;

			mask = (0x3 << switch_leds[i].offset1 |
				0x3 << switch_leds[i].offset2 | 
				0x3 << switch_leds[i].offset3);
			val = (state_map[state] << switch_leds[i].offset1 |
				state_map[state] << switch_leds[i].offset2 | 
				state_map[state] << switch_leds[i].offset3);

			reg = (reg &(~mask)) | val;

			if (reg != reg_old)
				athrs17_reg_write(0x5c, reg);
		break;
		}
	}
	return;
}

void show_activity(int arg)
{
	// printf("%s : %d arg: %d boot_status: %d\n", __func__, __LINE__, arg, wasp_boot_status);
	
	uint32_t time = 0;
	time =get_timer(0);
	int led;

	if (wasp_boot_status == IND_BL_AUTH_FAIL){
		gpio_led_switch(LED_ETH_GREEN ,(time>>9)&0x01);
		gpio_led_switch(LED_ETH_YELLOW ,(time>>9)&0x01);
	}
	return;
}

int read_reset_button(void)
{
	return ((~(ath_reg_rd(GPIO_IN_ADDRESS)>>CONFIG_RESET_BUTTON_GPIO)) & 0x01);
}

void hw_watchdog_reset(void)
{

}

int
ath_mem_config(void)
{
#if !defined(CONFIG_ATH_EMULATION)

#if !defined(CONFIG_ATH_NAND_BR)
	unsigned int type, reg32, *tap;
	extern uint32_t *ath_ddr_tap_cal(void);

	type = ath_ddr_initial_config(CFG_DDR_REFRESH_VAL);

	tap = ath_ddr_tap_cal();
	debug("tap = 0x%p\n", tap);

	tap = (uint32_t *)0xbd007f10;
	debug("Tap (low, high) = (0x%x, 0x%x)\n", tap[0], tap[1]);

	tap = (uint32_t *)TAP_CONTROL_0_ADDRESS;
	debug("Tap values = (0x%x, 0x%x, 0x%x, 0x%x)\n",
		tap[0], tap[2], tap[2], tap[3]);

	/* Take WMAC out of reset */
	reg32 = ath_reg_rd(RST_RESET_ADDRESS);
	reg32 = reg32 & ~RST_RESET_RTC_RESET_SET(1);
	ath_reg_wr_nf(RST_RESET_ADDRESS, reg32);
#endif

	ath_usb1_initial_config();
	ath_usb2_initial_config();

	ath_gpio_config();
#endif /* !defined(CONFIG_ATH_EMULATION) */

	return ath_ddr_find_size();
}

phys_size_t initdram(int board_type)
{
	return (ath_mem_config());
}

int	checkboard(args)
{
	printf("=====================================\n");
	printf(" QCA9558 U-boot\n");
	printf("-------------------------------------\n");
	return 0;
}

void _machine_restart(void)
{
	while (1) {
		ath_reg_wr(RST_RESET_ADDRESS, RST_RESET_FULL_CHIP_RESET_SET(1));
	}
}

extern int ath_gmac_enet_initialize(bd_t * bis);

int board_eth_init(bd_t *bis)
{
	return ath_gmac_enet_initialize(bis);
}
