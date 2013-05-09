/*
 * Copyright (C) 2012 RRKB IK, Raivis Rengelis, raivis@rrkb.lv
 * Copyright (C) 2009-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/spi/spi.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#include <mach/hardware.h>
#include <mach/device.h>
#include <mach/pinctrl.h>
#include <mach/regs-ocotp.h>

#include "device.h"
#include "imx233_olinuxino.h"
#include "mx23_pins.h"
#include <linux/i2c/tsc2007.h>
static void __init fixup_board(struct machine_desc *desc, struct tag *tags,
			       char **cmdline, struct meminfo *mi)
{
	mx23_set_input_clk(24000000, 24000000, 32000, 50000000);
}

#if defined(CONFIG_TOUCHSCREEN_TSC2007)
#define TSC2007_IRQGPIO		MXS_PIN_TO_GPIO(PINID_GPMI_CLE)
static int tsc2007_get_pendown_state(void)
{
	return !gpio_get_value(TSC2007_IRQGPIO);
}

int tsc2007_init_hw(void)
{
  
  return 0;
}

void tsc2007_exit_hw(void)
{
  //gpio_free(TSC2007_IRQGPIO); 
}
static struct tsc2007_platform_data tsc2007_info = {
	.model			= 2007,
	.x_plate_ohms		= 530,
	.get_pendown_state = tsc2007_get_pendown_state,
	.init_platform_hw = olinuxino_tsc2007_pin_init,
	.exit_platform_hw = olinuxino_tsc2007_pin_release,
};

static struct i2c_board_info lobster_i2c_devices[] = { {
		I2C_BOARD_INFO("tsc2007", 0x48),
		.platform_data	= &tsc2007_info,
		/* irq number is run-time assigned */
	},
};


//void mx23_init_tsc2007(void)
static void i2c_device_init(void)
{
  //printk("requesting gpio");
	//gpio_request_one(TSC2007_IRQGPIO, GPIOF_DIR_IN, "tsc2007-irq");
	printk("gpio to irq");
	lobster_i2c_devices[0].irq = gpio_to_irq(TSC2007_IRQGPIO); 
	printk("registering board info");
	i2c_register_board_info(0, lobster_i2c_devices,	ARRAY_SIZE(lobster_i2c_devices));
	printk("done"); 
}
#endif

static struct spi_board_info spi_board_info[] __initdata = {
#if defined(CONFIG_SPI_SPIDEV)
	{
		.modalias       = "spidev",
		//.max_speed_hz   = 48 * 1000 * 1000,
		.bus_num	= 1,
		.chip_select    = 0,
	},
#endif
};

static void spi_device_init(void)
{
	//No IRQ for the moment...
	//spi_board_info[0].irq = gpio_to_irq(MXS_PIN_TO_GPIO(PINID_SSP1_DATA1)); 
	spi_register_board_info(spi_board_info, ARRAY_SIZE(spi_board_info));
}

#if defined(CONFIG_SND_MXS_SOC_ADC) || defined(CONFIG_SND_MXS_SOC_ADC_MODULE)
static void __init imx233_olinuxino_init_adc(void)
{
	struct platform_device *pdev;
	pdev = mxs_get_device("mxs-adc", 0);
	if (pdev == NULL)
		return;
	mxs_add_device(pdev, 3);
}
#else
static void __init imx233_olinuxino_init_adc(void)
{

}
#endif

static void __init imx233_olinuxino_device_init(void)
{
	imx233_olinuxino_init_adc();
	
	#if defined(CONFIG_TOUCHSCREEN_TSC2007)
	i2c_device_init();
	#endif
	#if defined(CONFIG_SPI_MXS)
	spi_device_init();
	#endif
}


static void __init imx233_olinuxino_init_machine(void)
{
	mx23_pinctrl_init();

	/* Init iram allocate */
#ifdef CONFIG_VECTORS_PHY_ADDR
	/* reserve the first page for irq vectors table*/
	iram_init(MX23_OCRAM_PHBASE + PAGE_SIZE, MX23_OCRAM_SIZE - PAGE_SIZE);
#else
	iram_init(MX23_OCRAM_PHBASE, MX23_OCRAM_SIZE);
#endif

	mx23_gpio_init();
	imx233_olinuxino_pins_init();
	mx23_device_init();
	imx233_olinuxino_device_init();
}

MACHINE_START(IMX233_OLINUXINO, "iMX233-OLinuXino low cost board")
	.phys_io	= 0x80000000,
	.io_pg_offst	= ((0xf0000000) >> 18) & 0xfffc,
	.boot_params	= 0x40000100,
	.fixup		= fixup_board,
	.map_io		= mx23_map_io,
	.init_irq	= mx23_irq_init,
	.init_machine	= imx233_olinuxino_init_machine,
	.timer		= &mx23_timer.timer,
MACHINE_END
