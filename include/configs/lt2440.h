/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 * Gary Jennejohn <garyj@denx.de>
 * David Mueller <d.mueller@elsoft.ch>
 *
 * Configuation settings for the SAMSUNG SMDK2410 board.
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

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM920T	1	/* This is an ARM920T Core	*/
#define CONFIG_S3C24X0	1	/* in a SAMSUNG S3C24x0-type SoC	*/
#define CONFIG_S3C2410	1	/* specifically a SAMSUNG S3C2410 SoC	*/
#define CONFIG_SMDK2410	1	/* on a SAMSUNG SMDK2410 Board  */
#define CONFIG_S3C2440	1


/* input clock of PLL */
#define CONFIG_SYS_CLK_FREQ	12000000/* the SMDK2410 has 12MHz input clock */


#define USE_920T_MMU		1

#define CONFIG_USB_DEVICE   1

#ifdef CONFIG_USB_DEVICE
#define CONFIG_USE_IRQ		1
#endif

/*
 * Size of malloc() pool
 */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 128*1024)
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */

/*
 * DM9000AEP drivers
 */

#define CONFIG_DRIVER_DM9000            1
#define CONFIG_DM9000_BASE              0x18000300
#define DM9000_IO                       CONFIG_DM9000_BASE
#define DM9000_DATA                     (CONFIG_DM9000_BASE+4)
#define CONFIG_NET_MULTI        	1
#define CONFIG_DM9000_NO_SROM 	        1
#define CONFIG_NET_RETRY_COUNT		20
#define CONFIG_DM9000_USE_16BIT		1
#define CONFIG_RESET_PHY_R		1


/*
 * select serial console configuration
 */
#define CONFIG_S3C24X0_SERIAL
#define CONFIG_SERIAL1          1	/* we use SERIAL 0  on LT2440  */

/************************************************************
 * RTC
 ************************************************************/
#define	CONFIG_RTC_S3C24X0	1

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		115200


/*
 * BOOTP options
 */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME

#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_CMDLINE_TAG		1


#define CONFIG_BOOTCOMMAND	"nand read 3000A000 kernel ;bootm 3000A000"   //"nfs 0x30008000 192.168.1.31:/home/pk/sourcecode/root-fs/uImage;bootm"
#define CONFIG_BOOTDELAY	2
#define CONFIG_BOOTARGS    	"noinitrd console=ttySAC0 root=/dev/mtdblock2 rootfstype=yaffs2 "
//"root=/dev/nfs nfsroot=192.168.1.31:/home/pk/sourcecode/root-fs rw noinitrd init=/linuxrc  ip=192.168.1.16:192.168.1.31:192.168.1.1:255.255.255.0 console=ttySAC0,115200" 
#define CONFIG_ETHADDR	        08:00:3e:26:0a:5b 
#define CONFIG_NETMASK          255.255.255.0
#define CONFIG_IPADDR		192.168.1.66
#define CONFIG_SERVERIP		192.168.1.100

/*
 * Command line configuration.
 */
#include <config_cmd_default.h>

#define CONFIG_CMD_CACHE
#define CONFIG_CMD_DATE
#define CONFIG_CMD_ELF
#define CONFIG_CMD_PING  	
#define CONFIG_CMD_NAND		
//#define CONFIG_CMD_JFFS2 
#define CONFIG_CMD_LOADX  
#define CONFIG_CMD_MTDPARTS		
#define CONFIG_MTD_DEVICE


/* Command line configuration. */

#define CONFIG_CMDLINE_EDITING

#ifdef CONFIG_CMDLINE_EDITING
#undef CONFIG_AUTO_COMPLETE
#else
#define CONFIG_AUTO_COMPLETE
#endif


/*
 * FS options
 */


#define CONFIG_MTD_NAND_YAFFS2   1

#define MTDIDS_DEFAULT 		   "nand0=nandflash0"
#define MTDPARTS_DEFAULT 	   "mtdparts=nandflash0:384k(u-boot),"\
				   "256k(params),"\
				   "1408k(logo),"\
			       "3m(kernel),"\
				   "-(yaffs2)"




#if defined(CONFIG_CMD_KGDB)
#define CONFIG_KGDB_BAUDRATE	115200		/* speed to run kgdb serial port */
/* what's this ? it's not used anywhere */
#define CONFIG_KGDB_SER_INDEX	1		/* which serial port to use */
#endif

/*
 * Miscellaneous configurable options
 */
#define	CONFIG_SYS_LONGHELP				/* undef to save memory		*/
#define	CONFIG_SYS_PROMPT		"LT2440 # "	/* Monitor Command Prompt	*/
#define	CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size	*/
#define	CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16) /* Print Buffer Size */
#define	CONFIG_SYS_MAXARGS		16		/* max number of command args	*/
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE	/* Boot Argument Buffer Size	*/

#define CONFIG_SYS_MEMTEST_START	0x30000000	/* memtest works on	*/
#define CONFIG_SYS_MEMTEST_END		0x33F00000	/* 63 MB in DRAM	*/

#define	CONFIG_SYS_LOAD_ADDR		0x3000A000	/* default load address	*/


#undef CONFIG_SYS_CLKS_IN_HZ		/* everything, incl board info, in Hz */

/* the PWM TImer 4 uses a counter of 15625 for 10 ms, so we need */
/* it to wrap 100 times (total 1562500) to get 1 sec. */
#define	CONFIG_SYS_HZ			1562500

//#define	CONFIG_SYS_HZ			1000

/* valid baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(4*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	   /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		0x30000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x04000000 /* 64 MB */

#define PHYS_FLASH_1		0x00000000 /* Flash Bank #1 */

#define CONFIG_SYS_FLASH_BASE		PHYS_FLASH_1


/*-----------------------------------------------------------------------
 * LCD CONFIG
 */
#define CONFIG_TF2440_LOGO   	1


#ifdef 	CONFIG_TF2440_LOGO

#define CONFIG_TFT320x240	1
#define CONFIG_TFT480x272	2
#define CONFIG_TFT800x480   3
#define CONFIG_TFT800x600   4
#define CONFIG_VGA800x600   5

#define TFT_LCD		CONFIG_TFT480x272

#if	(TFT_LCD==CONFIG_TFT320x240)
#define	CONFIG_TF2440_LCD_VBPD		15
#define	CONFIG_TF2440_LCD_VFPD		4
#define	CONFIG_TF2440_LCD_VSPW		3
#define	CONFIG_TF2440_LCD_HBPD		38
#define	CONFIG_TF2440_LCD_HFPD		20
#define	CONFIG_TF2440_LCD_HSPW		30
#define	CONFIG_TF2440_LCD_CLKVAL	7
#define LCD_XSIZE_TFT	320
#define LCD_YSIZE_TFT	240

#elif  (TFT_LCD==CONFIG_TFT480x272)
#define	CONFIG_TF2440_LCD_VBPD		2
#define	CONFIG_TF2440_LCD_VFPD		2
#define	CONFIG_TF2440_LCD_VSPW		10
#define	CONFIG_TF2440_LCD_HBPD		2
#define	CONFIG_TF2440_LCD_HFPD		2
#define	CONFIG_TF2440_LCD_HSPW		15
#define	CONFIG_TF2440_LCD_CLKVAL	5
#define LCD_XSIZE_TFT	480
#define LCD_YSIZE_TFT	272

#elif  (TFT_LCD==CONFIG_TFT800x480)
#define	CONFIG_TF2440_LCD_VBPD		10
#define	CONFIG_TF2440_LCD_VFPD		22
#define	CONFIG_TF2440_LCD_VSPW		13
#define	CONFIG_TF2440_LCD_HBPD		30
#define	CONFIG_TF2440_LCD_HFPD		20
#define	CONFIG_TF2440_LCD_HSPW		15
#define	CONFIG_TF2440_LCD_CLKVAL	1
#define LCD_XSIZE_TFT	800
#define LCD_YSIZE_TFT	480

#elif  (TFT_LCD==CONFIG_TFT800x600)
#define	CONFIG_TF2440_LCD_VBPD		21
#define	CONFIG_TF2440_LCD_VFPD		18
#define	CONFIG_TF2440_LCD_VSPW		1
#define	CONFIG_TF2440_LCD_HBPD		46
#define	CONFIG_TF2440_LCD_HFPD		46
#define	CONFIG_TF2440_LCD_HSPW		1
#define	CONFIG_TF2440_LCD_CLKVAL	1
#define LCD_XSIZE_TFT	800
#define LCD_YSIZE_TFT	600

#elif  (TFT_LCD==CONFIG_VGA800x600)
#define	CONFIG_TF2440_LCD_VBPD		35
#define	CONFIG_TF2440_LCD_VFPD		20
#define	CONFIG_TF2440_LCD_VSPW		5
#define	CONFIG_TF2440_LCD_HBPD		50
#define	CONFIG_TF2440_LCD_HFPD		75
#define	CONFIG_TF2440_LCD_HSPW		119
#define	CONFIG_TF2440_LCD_CLKVAL	1
#define LCD_XSIZE_TFT	800
#define LCD_YSIZE_TFT	600
#endif 
#endif

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */

#define CONFIG_FLASH_CFI_DRIVER       1
#define CONFIG_SYS_FLASH_CFI	 	1
#define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */	
#define CONFIG_SYS_MAX_FLASH_SECT 	(38)
#define CONFIG_SYS_MONITOR_BASE    	PHYS_FLASH_1


#define CONFIG_ENV_ADDR			(CONFIG_SYS_FLASH_BASE + 0x0F0000) /* addr of environment */

/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(5*CONFIG_SYS_HZ) /* Timeout for Flash Erase */
#define CONFIG_SYS_FLASH_WRITE_TOUT	(5*CONFIG_SYS_HZ) /* Timeout for Flash Write */

//#define	CONFIG_ENV_IS_IN_FLASH	1
//#define CONFIG_ENV_SIZE		0x20000	/* Total Size of Environment Sector */

#define CONFIG_ENV_IS_IN_NAND  1
#define CONFIG_ENV_OFFSET      0x60000 //将环境变量保存到nand中的0x60000位置
#define CONFIG_ENV_SIZE        0x20000 /* Total Size of Environment Sector */

/*
 * Nand Flash register and envionment organization
 */

#define CONFIG_S3C2440_NAND_SKIP_BAD	1

/* NAND flash settings */
#if defined(CONFIG_CMD_NAND)
#define CONFIG_NAND_S3C2410
#define CONFIG_NAND_S3C2440
#define CONFIG_SYS_NAND_BASE            0x4E000000 //Nand配置寄存器基地址
#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_MTD_NAND_VERIFY_WRITE    1
#define CONFIG_SYS_64BIT_VSPRINTF	1

#endif

#endif	/* __CONFIG_H */
