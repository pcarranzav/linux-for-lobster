/*
 * Copyleft 2013 Pablo Carranza Velez, pablocarranza@gmail.com
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
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/delay.h>

#include <mach/pinctrl.h>

#include "mx23_pins.h"

static struct pin_desc imx233_lobster_fixed_pins[] = {
		{
				.name = "DUART.RX",
				.id = PINID_PWM0,
				.fun = PIN_FUN3,
		},
		{
				.name = "DUART.TX",
				.id = PINID_PWM1,
				.fun = PIN_FUN3,
		},
#ifdef CONFIG_MXS_AUART1_DEVICE_ENABLE
		{
				.name  = "AUART1.RX",
				.id    = PINID_I2C_SDA,
				.fun   = PIN_FUN3,
		},
		{
				.name  = "AUART1.TX",
				.id    = PINID_I2C_SCL,
				.fun   = PIN_FUN3,
		},
#else
	/*	{
				.name  = "UEXT.4",
				.id    = PINID_I2C_SDA,
				.fun   = PIN_GPIO,
				.voltage = PAD_3_3V,
				.strength = PAD_8MA,
				.drive = 1,
		},
		{
				.name  = "UEXT.3",
				.id    = PINID_I2C_SCL,
				.fun   = PIN_GPIO,
				.voltage = PAD_3_3V,
				.strength = PAD_8MA,
				.drive = 1,
		},*/
#endif

#if defined(CONFIG_I2C_MXS) || \
		defined(CONFIG_I2C_MXS_MODULE)
	/*	{
				.name = "I2C_SCL",
				.id = PINID_LCD_ENABLE,
				.fun = PIN_FUN2,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "I2C_SDA",
				.id = PINID_LCD_HSYNC,
				.fun = PIN_FUN2,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},*/
		{
				.name  = "I2C_SDA",
				.id    = PINID_I2C_SDA,
				.fun   = PIN_FUN1,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name  = "I2C_SCL",
				.id    = PINID_I2C_SCL,
				.fun   = PIN_FUN1,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		
#else
	/*	{
				.name = "UEXT.5",
				.id = PINID_LCD_ENABLE,
				.fun = PIN_GPIO,
				.voltage = PAD_3_3V,
				.strength = PAD_12MA,
				.drive = 1,
		},
		{
				.name = "UEXT.6",
				.id = PINID_LCD_HSYNC,
				.fun = PIN_GPIO,
				.voltage = PAD_3_3V,
				.strength = PAD_12MA,
				.drive = 1,
		},*/
#endif
#if defined(CONFIG_FB_MXS) || defined(CONFIG_FB_MXS_MODULE)
		{
				.name  = "LCD_D00",
				.id	= PINID_LCD_D00,
				.fun	= PIN_FUN1,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name  = "LCD_D01",
				.id	= PINID_LCD_D01,
				.fun	= PIN_FUN1,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name  = "LCD_D02",
				.id	= PINID_LCD_D02,
				.fun	= PIN_FUN1,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name  = "LCD_D03",
				.id	= PINID_LCD_D03,
				.fun	= PIN_FUN1,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name  = "LCD_D04",
				.id	= PINID_LCD_D04,
				.fun	= PIN_FUN1,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name  = "LCD_D05",
				.id	= PINID_LCD_D05,
				.fun	= PIN_FUN1,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name  = "LCD_D06",
				.id	= PINID_LCD_D06,
				.fun	= PIN_FUN1,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name  = "LCD_D07",
				.id	= PINID_LCD_D07,
				.fun	= PIN_FUN1,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "LCD_RESET",
				.id = PINID_LCD_RESET,
				.fun = PIN_FUN1,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "LCD_VSYNC",
				.id   = PINID_LCD_VSYNC,
				.fun  = PIN_FUN1,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "LCD_HSYNC",
				.id = PINID_LCD_HSYNC,
				.fun = PIN_FUN1,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "LCD_ENABLE",
				.id = PINID_LCD_ENABLE,
				.fun = PIN_FUN1,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "LCD_DOTCLK",
				.id = PINID_LCD_DOTCK,
				.fun = PIN_FUN1,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "LCD_BACKLIGHT",
				.id = PINID_PWM2,
				.fun = PIN_FUN1,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
#else
		{
				.name = "GPIO.35",
				.id	= PINID_LCD_D00,
				.fun	= PIN_GPIO,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.33",
				.id	= PINID_LCD_D01,
				.fun	= PIN_GPIO,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.31",
				.id	= PINID_LCD_D02,
				.fun	= PIN_GPIO,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.29",
				.id	= PINID_LCD_D03,
				.fun	= PIN_GPIO,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.27",
				.id	= PINID_LCD_D04,
				.fun	= PIN_GPIO,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.25",
				.id	= PINID_LCD_D05,
				.fun	= PIN_GPIO,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.23",
				.id	= PINID_LCD_D06,
				.fun	= PIN_GPIO,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.21",
				.id	= PINID_LCD_D07,
				.fun	= PIN_GPIO,
				.strength = PAD_12MA,
				.voltage	= PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.14",
				.id = PINID_LCD_RESET,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.08",
				.id   = PINID_LCD_VSYNC,
				.fun  = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.06",
				.id = PINID_LCD_DOTCK,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
#endif
		{
				.name = "GPIO.30",
				.id = PINID_GPMI_CE0N,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.28",
				.id = PINID_GPMI_CE1N,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.26",
				.id = PINID_GPMI_WPN,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.24",
				.id = PINID_GPMI_RDN,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
	/*	{
				.name = "UEXT.9",
				.id = PINID_GPMI_WRN, //Used in SPI2
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},*/
		{
				.name = "UEXT.10",
				.id = PINID_GPMI_RDY0,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		/*{
				.name = "UEXT.8",
				.id = PINID_GPMI_RDY1, // Used in SPI2
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},*/
		{
				.name = "GPIO.04",
				.id = PINID_GPMI_ALE,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
				.output = 1,
				.data = 1,
		},
		{ //This is the TSC2007 IRQ pin
				.name = "GPIO.03",
				.id = PINID_GPMI_CLE,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.05",
				.id = PINID_GPMI_D07,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.07",
				.id = PINID_GPMI_D06,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.09",
				.id = PINID_GPMI_D05,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.11",
				.id = PINID_GPMI_D04,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
	/*	{
				.name = "GPIO.13",
				.id = PINID_GPMI_D03, // Used in SPI2
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},*/
		/*{
				.name = "GPIO.15",
				.id = PINID_GPMI_D02,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.17",
				.id = PINID_GPMI_D01,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},*/
	/*	{
				.name = "GPIO.19",
				.id = PINID_GPMI_D00, // Used in SPI2
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		}, */
		{
				.name = "GPIO.16",
				.id = PINID_LCD_WR,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.18",
				.id = PINID_LCD_RS,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
		{
				.name = "GPIO.20",
				.id = PINID_LCD_CS,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive	= 1,
		},
#if defined(CONFIG_MMC_MXS_NODETECT)
		{
				.name = "LED1",
				.id = PINID_SSP1_DETECT,
				.fun = PIN_GPIO,
				.strength = PAD_12MA,
				.voltage = PAD_3_3V,
				.drive = 1,
				.output = 1,
				.data = 1,
		},
#endif
#if defined(CONFIG_TOUCHSCREEN_TSC2007)

#endif
};

#if defined(CONFIG_MMC_MXS) || defined(CONFIG_MMC_MXS_MODULE)
static struct pin_desc imx233_lobster_mmc_pins[] = {
		/* Configurations of SSP0 SD/MMC port pins */
		{
				.name = "SSP2_DATA0",
				.id = PINID_GPMI_D00,
				.fun = PIN_FUN3,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.pullup = 1,
				.drive = 1,
				.pull = 1,
		},
		{
				.name = "SSP2_DATA1",
				.id = PINID_GPMI_D01,
				.fun = PIN_FUN3,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.pullup = 1,
				.drive = 1,
				.pull = 1,
		},
		{
				.name = "SSP2_DATA2",
				.id = PINID_GPMI_D02,
				.fun = PIN_FUN3,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.pullup = 1,
				.drive = 1,
				.pull = 1,
		},
		{
				.name = "SSP2_DATA3",
				.id = PINID_GPMI_D03,
				.fun = PIN_FUN3,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.pullup = 1,
				.drive = 1,
				.pull = 1,
		},
		{
				.name = "SSP2_CMD",
				.id = PINID_GPMI_RDY1,
				.fun = PIN_FUN3,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.pullup = 1,
				.drive = 1,
				.pull = 1,
		},
		{
				.name = "SSP2_SCK",
				.id	= PINID_GPMI_WRN,
	 			.fun	= PIN_FUN3,
				.strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.pullup = 0,
				.drive = 1,
				.pull = 0,
		},
};
#endif

#if defined(CONFIG_SPI_MXS_SSP2)
static struct pin_desc lobster_spi2_pins[] = {
	{
	 .name	= "SSP2_DATA0",
	 .id	= PINID_GPMI_D00,
	 .fun	= PIN_FUN3,
	 .strength = PAD_8MA,
				.voltage = PAD_3_3V,
				.pullup = 1,
				.drive = 1,
				.pull = 1,
	 },
	{
	 .name	= "SSP2_DATA3",
	 .id	= PINID_GPMI_D03,
	 .fun	= PIN_FUN3,
	 .strength	= PAD_4MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 .pullup = 1,
	 .pull = 1,
	 },
	{
	 .name	= "SSP2_CMD",
	 .id	= PINID_GPMI_RDY1,
	 .fun	= PIN_FUN3,
	 .strength	= PAD_4MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 .pullup = 1,
	 .pull = 1,
	 },
	{
	 .name	= "SSP2_SCK",
	 .id	= PINID_GPMI_WRN,
	 .fun	= PIN_FUN3,
	 .strength	= PAD_8MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 .pullup = 1,
	 .pull=1,
	 },
};
#endif

static void mxs_request_pins(struct pin_desc *pins, int nr)
{
	int i;
	struct pin_desc *pin;

	/* configure the pins */
	for (i = 0; i < nr; i++) {
		pin = &pins[i];
		if (pin->fun == PIN_GPIO)
		{
			gpio_request(MXS_PIN_TO_GPIO(pin->id), pin->name);
			gpio_export(MXS_PIN_TO_GPIO(pin->id), true);
		}
		else
			mxs_request_pin(pin->id, pin->fun, pin->name);
		if (pin->drive) {
			mxs_set_strength(pin->id, pin->strength, pin->name);
			mxs_set_voltage(pin->id, pin->voltage, pin->name);
		}
		if (pin->pull)
			mxs_set_pullup(pin->id, pin->pullup, pin->name);
		if (pin->fun == PIN_GPIO) {
			if (pin->output)
				gpio_direction_output(MXS_PIN_TO_GPIO(pin->id),
						pin->data);
			else
				gpio_direction_input(MXS_PIN_TO_GPIO(pin->id));
		}
	}
}


#if defined(CONFIG_TOUCHSCREEN_TSC2007)
#define TSC2007_IRQGPIO		MXS_PIN_TO_GPIO(PINID_GPMI_CLE)
int lobster_tsc2007_pin_init(void)
{
	/* intr */
	gpio_request(TSC2007_IRQGPIO, "TSC2007_IRQ");
	gpio_export(TSC2007_IRQGPIO, true);
	gpio_direction_input(TSC2007_IRQGPIO);
	return 0;
}
void lobster_tsc2007_pin_release(void)
{
	return;
}
#endif

static void mxs_release_pins(struct pin_desc *pins, int nr)
{
	int i;
	struct pin_desc *pin;

	/* release the pins */
	for (i = 0; i < nr; i++) {
		pin = &pins[i];
		if (pin->fun == PIN_GPIO)
			gpio_free(MXS_PIN_TO_GPIO(pin->id));
		else
			mxs_release_pin(pin->id, pin->name);
	}
}

#if defined(CONFIG_MMC_MXS) || defined(CONFIG_MMC_MXS_MODULE)
int mxs_mmc_hw_init_mmc0(void)
{
	int ret = 0;

	mxs_request_pins(imx233_lobster_mmc_pins, ARRAY_SIZE(imx233_lobster_mmc_pins));
	mdelay(100);

	return 0;
}

int mxs_mmc_get_wp_mmc0(void)
{
	return 0;
}

void mxs_mmc_hw_release_mmc0(void)
{
	mxs_release_pins(imx233_lobster_mmc_pins, ARRAY_SIZE(imx233_lobster_mmc_pins));
}

void mxs_mmc_cmd_pullup_mmc0(int enable)
{
	mxs_set_pullup(PINID_GPMI_RDY1, enable, "mmc0_cmd");
}
#else
int mxs_mmc_hw_init_mmc0(void)
{
	return 0;
}

int mxs_mmc_get_wp_mmc0(void)
{
}

void mxs_mmc_hw_release_mmc0(void)
{
}

void mxs_mmc_cmd_pullup_mmc0(int enable)
{
}
#endif

#if ((!defined(CONFIG_SPI_MXS_SSP2)) || defined(CONFIG_MXS_DUAL_SPI) )&& (defined(CONFIG_SPI_MXS) || defined(CONFIG_SPI_MXS_MODULE))
static struct pin_desc lobster_spi1_pins[] = {
	{
	 .name	= "SSP1_DATA0",
	 .id	= PINID_SSP1_DATA0,
	 .fun	= PIN_FUN1,
	 .strength	= PAD_8MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 //.pull = 1,
	 //.pullup = 1,
	 },
	 {
	 .name	= "SSP1_DATA1",
	 .id	= PINID_SSP1_DATA1,
	 .fun	= PIN_GPIO,
	 .strength	= PAD_8MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 	 .pull = 1,
	 .pullup = 1,
	 .output = 1,
	 .data = 1,
	 },
	 {
	 .name	= "SSP1_DATA2",
	 .id	= PINID_SSP1_DATA2,
	 .fun	= PIN_GPIO,
	 .strength	= PAD_8MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 	 .pull = 1,
	 .pullup = 1,
	 	 .output = 1,
	 .data = 1,
	 },
	{
	 .name	= "SSP1_DATA3",
	 .id	= PINID_SSP1_DATA3,
	 .fun	= PIN_FUN1,
	 .strength	= PAD_8MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 	 .pull = 1,
	 .pullup = 1,
	 },
	{
	 .name	= "SSP1_CMD",
	 .id	= PINID_SSP1_CMD,
	 .fun	= PIN_FUN1,
	 .strength	= PAD_8MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	//	 .pull = 1,
	// .pullup = 1,
	 },
	{
	 .name	= "SSP1_SCK",
	 .id	= PINID_SSP1_SCK,
	 .fun	= PIN_FUN1,
	 .strength	= PAD_8MA,
	 .voltage	= PAD_3_3V,
	 .drive 	= 1,
	 },
};
#endif

#if defined(CONFIG_SPI_MXS_SSP2)
int mxs_spi2_pin_init(void)
{
	mxs_request_pins(lobster_spi2_pins, ARRAY_SIZE(lobster_spi2_pins));

	return 0;
}
int mxs_spi2_pin_release(void)
{

	/* release the pins */
	mxs_release_pins(lobster_spi2_pins, ARRAY_SIZE(lobster_spi2_pins));

	return 0;
}
#endif
#if !defined(CONFIG_SPI_MXS) || (!defined(CONFIG_SPI_MXS_SSP2) || defined(CONFIG_MXS_DUAL_SPI))
int mxs_spi_enc_pin_init(void)
{
	#if (defined(CONFIG_SPI_MXS) || defined(CONFIG_SPI_MXS_MODULE))
	printk("Requesting SPI pins.");
	mxs_request_pins(lobster_spi1_pins, ARRAY_SIZE(lobster_spi1_pins));
	#endif
	return 0;
}
int mxs_spi_enc_pin_release(void)
{
	#if (defined(CONFIG_SPI_MXS) || defined(CONFIG_SPI_MXS_MODULE))
	mxs_release_pins(lobster_spi1_pins, ARRAY_SIZE(lobster_spi1_pins));
	#endif
	return 0;
}
#endif

void __init imx233_lobster_pins_init(void)
{
	mxs_request_pins(imx233_lobster_fixed_pins, ARRAY_SIZE(imx233_lobster_fixed_pins));
}
