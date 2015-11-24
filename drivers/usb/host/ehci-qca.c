/*
 * Qualcomm QCA955x USB driver initialization
 *
 * Copyright (C) 2015 Mantas Pucka <mantas@8devices.com>
 *
 * SPDX-License-Identifier:GPL-2.0
 */

#include <common.h>
#include <usb.h>
#include <asm/io.h>
#include <usb/ehci-fsl.h>
#include <asm/errno.h>

#include <asm/mipsregs.h>
#include <atheros.h>

#include "ehci.h"

/*
 * Create the appropriate control structures to manage
 * a new EHCI host controller.
 *
 * Excerpts from linux ehci fsl driver.
 */

int ehci_hcd_init(int index, enum usb_init_type init, struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{
	struct usb_ehci *ehci;

	ath_reg_wr (RST_RESET_ADDRESS, (ath_reg_rd(RST_RESET_ADDRESS) | RST_RESET_USB_PHY_SUSPEND_OVERRIDE_MASK));
	mdelay(10);

	ath_reg_wr (RST_RESET_ADDRESS, (ath_reg_rd(RST_RESET_ADDRESS) & ~(RST_RESET_USB_HOST_RESET_MASK)));
	mdelay(10);

	ath_reg_wr (RST_RESET_ADDRESS, (ath_reg_rd(RST_RESET_ADDRESS) & ~(RST_RESET_USB_PHY_RESET_MASK)));
	mdelay(10);

	switch (index) {
		case 0:
			ehci = (struct usb_ehci *)KSEG1ADDR(ATH_USB_EHCI_BASE_1);
			break;
		case 1:
			ehci = (struct usb_ehci *)KSEG1ADDR(ATH_USB_EHCI_BASE_2);
			break;
		default:
			printf("ERROR: wrong controller index!!\n");
			return -EINVAL;
	}

	*hccr = (struct ehci_hccr *)((uint32_t)&(ehci->caplength));
	*hcor = (struct ehci_hcor *)((uint32_t) *hccr +
			HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));
	debug("ehci-qca: init hccr %x and hcor %x hc_length %d\n",
		(uint32_t)*hccr, (uint32_t)*hcor,
		(uint32_t)HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));

	return 0;
}

/*
 * Destroy the appropriate control structures corresponding
 * the the EHCI host controller.
 */
int ehci_hcd_stop(int index)
{
	return 0;
}
