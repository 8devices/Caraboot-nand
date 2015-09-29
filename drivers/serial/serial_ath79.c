#include <common.h>
#include <serial.h>

#include <asm/addrspace.h>
#include <asm/types.h>
#include <config.h>
#include <atheros.h>
#include <watchdog.h>

int ath79_serial_init(void)
{

	uint32_t div, val;

	div = ath_uart_freq() / (16 * CONFIG_BAUDRATE);
#ifdef CONFIG_SCO_SLAVE_CONNECTED 
	val = ath_reg_rd(GPIO_OE_ADDRESS) & (~0xcbf410u);
#else
	val = ath_reg_rd(GPIO_OE_ADDRESS) & (~0xcffc10u);
#endif
	ath_reg_wr(GPIO_OE_ADDRESS, val);

	ath_reg_rmw_set(GPIO_OUT_FUNCTION2_ADDRESS,
			GPIO_OUT_FUNCTION2_ENABLE_GPIO_10_SET(0x16));

	ath_reg_rmw_clear(GPIO_IN_ENABLE0_ADDRESS,
			GPIO_IN_ENABLE0_UART_SIN_SET(0xff));

	ath_reg_rmw_set(GPIO_IN_ENABLE0_ADDRESS,
			GPIO_IN_ENABLE0_UART_SIN_SET(0x9));

	val = ath_reg_rd(GPIO_OUT_ADDRESS) | 0xcffc10u;
	ath_reg_wr(GPIO_OUT_ADDRESS, val);

	val = ath_reg_rd(GPIO_SPARE_ADDRESS);
	ath_reg_wr(GPIO_SPARE_ADDRESS, (val | 0x8402));

	ath_reg_wr(GPIO_OUT_ADDRESS, 0x2f);

	/*
	 * set DIAB bit
	 */
	ath_uart_wr(OFS_LINE_CONTROL, 0x80);

	/* set divisor */
	ath_uart_wr(OFS_DIVISOR_LSB, (div & 0xff));
	ath_uart_wr(OFS_DIVISOR_MSB, ((div >> 8) & 0xff));

	/* clear DIAB bit*/
	ath_uart_wr(OFS_LINE_CONTROL, 0x00);

	/* set data format */
	ath_uart_wr(OFS_DATA_FORMAT, 0x3);

	ath_uart_wr(OFS_INTR_ENABLE, 0);

	return 0;
}

int ath79_serial_tstc (void)
{
	return(ath_uart_rd(OFS_LINE_STATUS) & 0x1);
}

int ath79_serial_getc(void)
{
	while(!ath79_serial_tstc()){
		WATCHDOG_RESET();
	}

	return ath_uart_rd(OFS_RCV_BUFFER);
}


void ath79_serial_putc(const char byte)
{
	if (byte == '\n') ath79_serial_putc ('\r');

	while (((ath_uart_rd(OFS_LINE_STATUS)) & 0x20) == 0x0);

	ath_uart_wr(OFS_SEND_BUFFER, byte);
}

void ath79_serial_setbrg (void)
{
}

void ath79_serial_puts (const char *s)
{
	while (*s)
	{
		ath79_serial_putc (*s++);
	}
}

static struct serial_device ath79_serial_drv = {
	.name	= "ath79_serial",
	.start	= ath79_serial_init,
	.stop	= NULL,
	.setbrg	= ath79_serial_setbrg,
	.putc	= ath79_serial_putc,
	.puts	= ath79_serial_puts,
	.getc	= ath79_serial_getc,
	.tstc	= ath79_serial_tstc,
};

void ath79_serial_initialize(void)
{
	serial_register(&ath79_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
	return &ath79_serial_drv;
}
