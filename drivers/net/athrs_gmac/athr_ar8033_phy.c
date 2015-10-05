/*
 * Copyright (c) 2010, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


#include <config.h>
#include <linux/types.h>
#include <common.h>
#include <miiphy.h>
#include "phy.h"
#include <asm/addrspace.h>
#include <atheros.h>
#include "athrs_ar8033_phy.h"


void
athrs_ar8033_mgmt_init(void)
{
  
}

int
athrs_ar8033_phy_setup(void  *arg)
{
    return 0;
}

int
athrs_ar8033_phy_is_fdx(int ethUnit)
{
    
    unsigned int val;
    val = phy_reg_read(ethUnit, CONFIG_AR8035_PHY_ADDR, 0x11);

    return (val & (1 << 13)) ? 1 : 0;
    
}

int
athrs_ar8033_phy_is_link_alive(int phyUnit)
{
	unsigned int val;
	val = phy_reg_read(0, CONFIG_AR8035_PHY_ADDR, 0x11);

	return (val & 0x400) ? 1 : 0;
  }

int
athrs_ar8033_phy_is_up(int ethUnit)
{
    	unsigned int val;
	val = phy_reg_read(ethUnit, CONFIG_AR8035_PHY_ADDR, 0x11);

	return (val & 0x400) ? 1 : 0;
}

int
athrs_ar8033_phy_speed(int ethUnit)
{
	unsigned int val;
	val = phy_reg_read(ethUnit, CONFIG_AR8035_PHY_ADDR, 0x11);
	val = val & 0xc000;
	val = val >> 14;

	switch (val) {
		case 0:
			return _10BASET;
		case 1:
			return _100BASET;
		case 2:
			return _1000BASET;
		case 3:
		default:
			return -1;
	}
	return -1;
}

int 
athrs_ar8033_reg_init(void *arg)
{
	unsigned int id1, id2, val;
	int i;
	int mac_unit;
	
	mac_unit = *(int *)arg;
	printf("%s: MAC unit: %x\n", __func__, mac_unit);

	id1 = phy_reg_read(mac_unit, CONFIG_AR8035_PHY_ADDR, 2);
	id2 = phy_reg_read(mac_unit, CONFIG_AR8035_PHY_ADDR, 3);

	debug("%s: id1=%X, id2=%X\n", __func__, id1, id2);

	if (mac_unit == 1) {
		phy_reg_write(mac_unit, CONFIG_AR8035_PHY_ADDR, 0x1d, 0);
		val = phy_reg_read(mac_unit, CONFIG_AR8035_PHY_ADDR, 0x1E);
		phy_reg_write(mac_unit, CONFIG_AR8035_PHY_ADDR, 0x1d, 0x5);
		val = phy_reg_read(mac_unit, CONFIG_AR8035_PHY_ADDR, 0x1E);
		phy_reg_write(mac_unit, CONFIG_AR8035_PHY_ADDR, 0x1e, 0x2D47);
		val = phy_reg_read(mac_unit, CONFIG_AR8035_PHY_ADDR, 0x1E);

		debug("%s: Done %x \n",__func__, phy_reg_read(mac_unit,CONFIG_AR8035_PHY_ADDR,0x1f));
	}

	return 0;
}

