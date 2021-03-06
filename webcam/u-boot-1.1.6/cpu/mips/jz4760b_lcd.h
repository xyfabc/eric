/*
 * linux/drivers/video/jz4750_lcd.h -- Ingenic Jz4750 On-Chip LCD frame buffer device
 *
 * Copyright (C) 2005-2008, Ingenic Semiconductor Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __JZ4760B_LCD_H__
#define __JZ4760B_LCD_H__

//#include <asm/io.h>


#define NR_PALETTE	256
#define PALETTE_SIZE	(NR_PALETTE*2)

/* use new descriptor(8 words) */
struct jz4760_lcd_dma_desc {
	unsigned int next_desc; 	/* LCDDAx */
	unsigned int databuf;   	/* LCDSAx */
	unsigned int frame_id;  	/* LCDFIDx */ 
	unsigned int cmd; 		/* LCDCMDx */
	unsigned int offsize;       	/* Stride Offsize(in word) */
	unsigned int page_width; 	/* Stride Pagewidth(in word) */
	unsigned int cmd_num; 		/* Command Number(for SLCD) */
	unsigned int desc_size; 	/* Foreground Size */
};

struct jz4760lcd_panel_t {
	unsigned int cfg;	/* panel mode and pin usage etc. */
	unsigned int slcd_cfg;	/* Smart lcd configurations */
	unsigned int ctrl;	/* lcd controll register */
	unsigned int w;		/* Panel Width(in pixel) */
	unsigned int h;		/* Panel Height(in line) */
	unsigned int fclk;	/* frame clk */
	unsigned int hsw;	/* hsync width, in pclk */
	unsigned int vsw;	/* vsync width, in line count */
	unsigned int elw;	/* end of line, in pclk */
	unsigned int blw;	/* begin of line, in pclk */
	unsigned int efw;	/* end of frame, in line count */
	unsigned int bfw;	/* begin of frame, in line count */
};


struct jz4760lcd_fg_t {
	int bpp;	/* foreground bpp */
	int x;		/* foreground start position x */
	int y;		/* foreground start position y */
	int w;		/* foreground width */
	int h;		/* foreground height */
};

struct jz4760lcd_osd_t {
	unsigned int osd_cfg;	/* OSDEN, ALHPAEN, F0EN, F1EN, etc */
	unsigned int osd_ctrl;	/* IPUEN, OSDBPP, etc */
	unsigned int rgb_ctrl;	/* RGB Dummy, RGB sequence, RGB to YUV */
	unsigned int bgcolor;	/* background color(RGB888) */
	unsigned int colorkey0;	/* foreground0's Colorkey enable, Colorkey value */
	unsigned int colorkey1; /* foreground1's Colorkey enable, Colorkey value */
	unsigned int alpha;	/* ALPHAEN, alpha value */
	unsigned int ipu_restart; /* IPU Restart enable, ipu restart interval time */

#define FG_NOCHANGE 		0x0000
#define FG0_CHANGE_SIZE 	0x0001
#define FG0_CHANGE_POSITION 	0x0002
#define FG1_CHANGE_SIZE 	0x0010
#define FG1_CHANGE_POSITION 	0x0020
#define FG_CHANGE_ALL 		( FG0_CHANGE_SIZE | FG0_CHANGE_POSITION | \
		FG1_CHANGE_SIZE | FG1_CHANGE_POSITION )
	int fg_change;
	struct jz4760lcd_fg_t fg0;	/* foreground 0 */
	struct jz4760lcd_fg_t fg1;	/* foreground 1 */
};

struct jz4760lcd_info {
	struct jz4760lcd_panel_t panel;
	struct jz4760lcd_osd_t osd;
};


/* Jz LCDFB supported I/O controls. */
#define FBIOSETBACKLIGHT	0x4688 /* set back light level */
#define FBIODISPON		0x4689 /* display on */
#define FBIODISPOFF		0x468a /* display off */
#define FBIORESET		0x468b /* lcd reset */
#define FBIOPRINT_REG		0x468c /* print lcd registers(debug) */
#define FBIOROTATE		0x46a0 /* rotated fb */
#define FBIOGETBUFADDRS		0x46a1 /* get buffers addresses */
#define FBIO_GET_MODE		0x46a2 /* get lcd info */
#define FBIO_SET_MODE		0x46a3 /* set osd mode */
#define FBIO_DEEP_SET_MODE	0x46a4 /* set panel and osd mode */
#define FBIO_MODE_SWITCH	0x46a5 /* switch mode between LCD and TVE */
#define FBIO_GET_TVE_MODE	0x46a6 /* get tve info */
#define FBIO_SET_TVE_MODE	0x46a7 /* set tve mode */

/*
 * LCD panel specific definition
 */
/* AUO */
#if defined(CONFIG_JZ4760_LCD_AUO_A043FL01V2)
#if defined(CONFIG_LEPUS60B)  || defined(CONFIG_LEPUS)/* board lepus */
#define SPEN		(32*1+29)       /*LCD_CS*/
#define SPCK		(32*1+28)       /*LCD_SCL*/
#define SPDA		(32*1+21)       /*LCD_SDA*/
#define LCD_RET 	(32*5+6)       /*LCD_DISP_N use for lcd reset*/
#else
#error "cpu/mips/jz4760B_lcd.h, please define SPI pins on your board."
#endif
#define __lcd_special_pin_init()		\
	do {						\
		__gpio_as_output(SPEN); /* use SPDA */	\
		__gpio_as_output(SPCK); /* use SPCK */	\
		__gpio_as_output(SPDA); /* use SPDA */	\
		__gpio_as_output(LCD_RET);		\
		udelay(50);				\
		__gpio_clear_pin(LCD_RET);		\
		udelay(100);				\
		__gpio_set_pin(LCD_RET);		\
	} while (0)
#define __lcd_special_on()			     \
	do {					     \
		udelay(50);			     \
		__gpio_clear_pin(LCD_RET);	     \
		udelay(100);			     \
		__gpio_set_pin(LCD_RET);	     \
	} while (0)

#define __lcd_special_off() \
	do { \
		__gpio_clear_pin(LCD_RET);		\
	} while (0)

#endif	/* CONFIG_JZLCD_AUO_A030FL01_V1 */

#ifndef __lcd_special_pin_init
#define __lcd_special_pin_init()
#endif
#ifndef __lcd_special_on
#define __lcd_special_on()
#endif
#ifndef __lcd_special_off
#define __lcd_special_off()
#endif


/*
 * Platform specific definition
 */
#if defined(CONFIG_LEPUS60B) /* board lepus60b */
/*====================================================================== 
 * LCD backlight
 */
#define LCD_PWM_FULL 101
/* 100 level: 0,1,...,100 */
//at 0510
#define __send_low_pulse(n)		\
	do{				\
		unsigned int i;			\
		for(i = n; i > 0; i--)	{	\
			__gpio_clear_pin(GPIO_LCD_PWM);	\
			udelay(1);	\
			__gpio_set_pin(GPIO_LCD_PWM);	\
			udelay(3);	\
		}	\
	}while(0)
#define MAX_BRIGHTNESS_STEP	16
#define CONVERT_FACTOR		(256/MAX_BRIGHTNESS_STEP)

#define __lcd_init_backlight(n)			\
	do{	\
		unsigned int tmp = (n)/CONVERT_FACTOR + 1;	\
		__gpio_as_output(GPIO_LCD_PWM);			\
		__gpio_set_pin(GPIO_LCD_PWM);			\
		udelay(30);		\
		__send_low_pulse(MAX_BRIGHTNESS_STEP-tmp);	\
	}while(0)

#define __lcd_set_backlight_level(n)	\
do {					\
__gpio_as_output(GPIO_LCD_PWM);	\
__gpio_set_pin(GPIO_LCD_PWM);	\
} while (0)
#define __lcd_close_backlight()		\
	do {					\
		__gpio_as_output(GPIO_LCD_PWM);	\
		__gpio_clear_pin(GPIO_LCD_PWM);	\
	} while (0)

#define __lcd_display_pin_init() \
	do { \
		__gpio_as_output(GPIO_LCD_VCC_EN_N);	 \
		__lcd_special_pin_init();		 \
	} while (0)

#define __lcd_display_on() \
	do { \
		__gpio_clear_pin(GPIO_LCD_VCC_EN_N);	\
		__lcd_special_on();			\
		__lcd_set_backlight_level(112);		\
	} while (0)

#define __lcd_display_off() \
	do { \
		__lcd_close_backlight();	   \
		__lcd_special_off();	 \
	} while (0)
#endif /* LEPUS60B */


/*****************************************************************************
 * LCD display pin dummy macros
 *****************************************************************************/

#ifndef __lcd_display_pin_init
#define __lcd_display_pin_init()
#endif
#ifndef __lcd_slcd_special_on
#define __lcd_slcd_special_on()
#endif
#ifndef __lcd_display_on
#define __lcd_display_on()
#endif
#ifndef __lcd_display_off
#define __lcd_display_off()
#endif
#ifndef __lcd_set_backlight_level
#define __lcd_set_backlight_level(n)
#endif

#endif /* __JZ4760B_LCD_H__ */
