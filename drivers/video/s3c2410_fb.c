#include <common.h>
#include <asm/arch/s3c24x0_cpu.h>
#include "s3c2410_logo_uboot.h"
#include "s3c2410_logo_sys.h"

#define MVAL			(13)
#define MVAL_USED 		(0)						//0=each frame   1=rate by MVAL
#define INVVDEN			(1)						//0=normal       1=inverted
#define BSWP			(0)						//Byte swap control
#define HWSWP			(1)						//Half word swap control
#define SCR_XSIZE_TFT	 	(LCD_XSIZE_TFT)
#define SCR_YSIZE_TFT	 	(LCD_YSIZE_TFT)
#define HOZVAL_TFT		( LCD_XSIZE_TFT - 1 )
#define LINEVAL_TFT		( LCD_YSIZE_TFT - 1 )


#define U32 unsigned int
#define M5D(n)			( ( n ) & 0x1fffff )				
#define LCD_FRAMEBUFFER         (TEXT_BASE - 0x300000) 
volatile unsigned short *tf2440_LCD_BUFFER;
volatile char vbpd = 1, vfpd = 1, vspw = 1, hbpd = 1, hfpd = 1, hspw = 1, clkval_tft = 1 ;


/**************************************************************
Clear TFT LCD With C
**************************************************************/
void tf2440_lcd_ClearScr( U32 c)
{
	unsigned int x,y ;

	for( y = 0 ; y < LCD_YSIZE_TFT ; y++ )
	{
		for( x = 0 ; x < (SCR_XSIZE_TFT) ; x++ )
		{
			*(tf2440_LCD_BUFFER+LCD_XSIZE_TFT*y+x) = c;
		}
	}
}

/**************************************************************
TFT LCD指定大小的矩形填充特定颜色单元或清屏
**************************************************************/
void tf2440_lcd_ClearScr_Rectangle( int x0,int y0,int width,int height, U32 c)
{
	unsigned int x,y ;
		
	for( y = y0 ; y < (y0 + height) ; y++ )
	{
		for( x = x0 ; x < (width + x0) ; x++ )
		{
			*(tf2440_LCD_BUFFER+LCD_XSIZE_TFT*y+x) = c;
		}
	}
}

/**************************************************************
在LCD屏幕上指定坐标点画一个指定大小的图片
**************************************************************/
void tf2440_paint_Bmp(int x0,int y0,int width,int height, unsigned char bmp[])
{
	int x, y ;
	U32 c ;
	int p = 0 ;

	for( y = y0 ; y < (height + y0) ; y++ )
	{
		for( x = x0 ; x < (width + x0) ; x++ )
		{
			c = bmp[p+1] | (bmp[p] << 8) ;

			if ( ( x <= (SCR_XSIZE_TFT) ) && ( y <= LCD_YSIZE_TFT ) )
					*(tf2440_LCD_BUFFER+LCD_XSIZE_TFT*y+x) = c;
			p = p + 2 ;
		}
	}

}

/**************************************************************
在坐标（X，Y）画点
**************************************************************/
void putPixel(U32 x, U32 y, U32 c)
{
	if( (x < LCD_XSIZE_TFT) && (y < LCD_YSIZE_TFT))
		*(tf2440_LCD_BUFFER+LCD_XSIZE_TFT*y+x) = c;
}


U32 currWidth = 0;
void drawProcessBar(U32 total, U32 current )
{
	U32 const bar_height = 8;
	U32 bar_base = LCD_YSIZE_TFT - bar_height;
	int i = (int) LCD_XSIZE_TFT / 8;
	U32 j;

	int pbcolor ;
	if(total != -1)
	{
		j = 0;
		int bar_width = (int) LCD_XSIZE_TFT * ((current * 1.0) / total);
		if (bar_width <= i)
			pbcolor = 0x7FF;					//sky blue
		else if((bar_width > i) && (bar_width <= i * 2))
			pbcolor = 0x1F;					//blue
		else if((bar_width > i * 2) && (bar_width <= i * 3))
			pbcolor = 0x0;					//black
		else if((bar_width > i * 3) && (bar_width <= i * 4))
			pbcolor = 0xF81F;					//purple
		else if((bar_width > i * 4) && (bar_width <= i * 5))
			pbcolor = 0xFFFF;					//white
		else if((bar_width > i * 5) && (bar_width <= i * 6))
			pbcolor = 0xF800;					//red
		else if((bar_width > i * 6) && (bar_width <= i * 7))
			pbcolor = 0xFFE0;					//yellow
		else if((bar_width > i * 7) && (bar_width <= i * 8))
			pbcolor = 0x7E0;					//green

		if(bar_width > currWidth)
		{
			for ( ; j < bar_height; j++)
			{
				putPixel(bar_width, j + bar_base, pbcolor);
			}
			currWidth = bar_width;
		}
	}
}

/**************************************************************
LCD PWREN 
**************************************************************/
void tf2440_lcd_PowerEnable(int invpwren , int pwren)
{
	struct s3c24x0_gpio * const gpio = s3c24x0_get_base_gpio();
	struct s3c24x0_lcd * const lcd = s3c24x0_get_base_lcd();
										
	gpio -> GPGUP = gpio -> GPGUP & ( ~( 1 << 4) ) | ( 1 << 4) ;		
	gpio -> GPGCON = gpio -> GPGCON & ( ~( 3 << 8) ) | ( 3 << 8) ;		
	gpio -> GPGDAT = gpio -> GPGDAT | (1 << 4 ) ;
										
										
	lcd -> LCDCON5 = lcd -> LCDCON5 & ( ~( 1 << 3 ) ) | ( pwren << 3 ) ;	
	lcd -> LCDCON5 = lcd -> LCDCON5 & ( ~( 1 << 5 ) ) | ( invpwren << 5 ) ;	
}

/**************************************************************
LCD ON//OFF
**************************************************************/
void tf2440_lcd_EnvidOnOff(int onoff)
{
	struct s3c24x0_lcd * const lcd = s3c24x0_get_base_lcd();

	if( onoff == 1 )
		lcd -> LCDCON1 |= 1 ;							
	else
		lcd -> LCDCON1 = lcd -> LCDCON1 & 0x3fffe ;				
}

/**************************************************************
LCD Init
**************************************************************/
void tf2440_lcd_init(void)
{
	int x, y ;
	U32 c ;
	int p = 0 ;

	struct s3c24x0_gpio * const gpio = s3c24x0_get_base_gpio();
	struct s3c24x0_lcd * const lcd = s3c24x0_get_base_lcd();

	char *s_lcd;
	tf2440_LCD_BUFFER=(volatile unsigned short *)LCD_FRAMEBUFFER;
	lcd -> LCDSADDR1 = ( ( ( U32 ) tf2440_LCD_BUFFER >> 22 ) << 21 ) | M5D ( ( U32 ) tf2440_LCD_BUFFER >> 1 ) ;
	lcd -> LCDSADDR2 = M5D( ( ( U32) tf2440_LCD_BUFFER + ( SCR_XSIZE_TFT * LCD_YSIZE_TFT * 2 ) ) >> 1 ) ;
	lcd -> LCDSADDR3 = ( ( ( SCR_XSIZE_TFT - LCD_XSIZE_TFT ) / 1 ) << 11 ) | ( LCD_XSIZE_TFT /1 ) ;



	
	vbpd =CONFIG_TF2440_LCD_VBPD;
	
	vfpd = CONFIG_TF2440_LCD_VFPD;

	vspw = CONFIG_TF2440_LCD_VSPW;

	hbpd =CONFIG_TF2440_LCD_HBPD ;

	hfpd =CONFIG_TF2440_LCD_HFPD;

	hspw = CONFIG_TF2440_LCD_HSPW;

	clkval_tft =CONFIG_TF2440_LCD_CLKVAL ;


	tf2440_lcd_ClearScr( 0x0 ) ;

	gpio -> GPCUP  = 0xffffffff ;
	gpio -> GPCCON = 0xaaaaaaaa ;						//Initialize VD[0:7]	 
	 
	gpio -> GPDUP  = 0xffffffff ;
	gpio -> GPDCON = 0xaaaaaaaa ;						//Initialize VD[15:8]

	lcd -> LCDCON1 = ( clkval_tft << 8 ) | ( MVAL_USED << 7 ) | (3 << 5 ) | ( 12 << 1 ) | 0 ;									
	lcd -> LCDCON2 = ( vbpd << 24 ) | ( LINEVAL_TFT << 14 ) | ( vfpd << 6 ) | ( vspw ) ;
	lcd -> LCDCON3 = ( hbpd << 19 ) | ( HOZVAL_TFT << 8 ) | ( hfpd ) ;
	lcd -> LCDCON4 = ( MVAL << 8 ) | ( hspw ) ;
	lcd -> LCDCON5 = ( 1 << 11) | ( 0 << 10 ) | ( 1 << 9 ) | ( 1 << 8 ) | ( 0 << 7 ) | ( 0 << 6 ) | ( 1 << 3 ) | ( BSWP << 1 ) | ( HWSWP ) ;

	lcd -> LPCSEL &= ( ~7 ) ;						// Disable LPC3480
	lcd -> TPAL = 0x0 ;							// Disable Temp Palette

	tf2440_lcd_PowerEnable( 0, 1 ) ;
}

void tf2440_logo(void)
{

	struct s3c24x0_gpio * const gpio = s3c24x0_get_base_gpio();
	struct s3c24x0_lcd * const lcd = s3c24x0_get_base_lcd();

	tf2440_lcd_ClearScr( 0xffff ) ;
	tf2440_paint_Bmp((LCD_XSIZE_TFT-320) / 2, 50, 320, 32, tf2440_sys) ;
	tf2440_lcd_EnvidOnOff ( 1 ) ;
}
void tf2440_logo_download(void)
{
	
	struct s3c24x0_gpio * const gpio = s3c24x0_get_base_gpio();
	struct s3c24x0_lcd * const lcd = s3c24x0_get_base_lcd();

	tf2440_lcd_ClearScr( 0x0) ;
	tf2440_paint_Bmp((LCD_XSIZE_TFT-260) / 2, 50, 260, 45, tf2440_uboot) ;
	tf2440_lcd_EnvidOnOff ( 1 ) ;	
}


