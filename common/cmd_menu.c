

#include <common.h>
#include <command.h>
#include <def.h>
#include <nand.h>

extern char console_buffer[];
extern int readline (const char *const prompt);
extern char awaitkey(unsigned long delay, int* error_p);
extern void download_nkbin_to_flash(void);

/**
 * Parses a string into a number.  The number stored at ptr is
 * potentially suffixed with K (for kilobytes, or 1024 bytes),
 * M (for megabytes, or 1048576 bytes), or G (for gigabytes, or
 * 1073741824).  If the number is suffixed with K, M, or G, then
 * the return value is the number multiplied by one kilobyte, one
 * megabyte, or one gigabyte, respectively.
 *
 * @param ptr where parse begins
 * @param retptr output pointer to next char after parse completes (output)
 * @return resulting unsigned int
 */
static unsigned long memsize_parse2 (const char *const ptr, const char **retptr)
{
	unsigned long ret = simple_strtoul(ptr, (char **)retptr, 0);
    int sixteen = 1;

	switch (**retptr) {
		case 'G':
		case 'g':
			ret <<= 10;
		case 'M':
		case 'm':
			ret <<= 10;
		case 'K':
		case 'k':
			ret <<= 10;
			(*retptr)++;
            sixteen = 0;
		default:
			break;
	}

    if (sixteen)
        return simple_strtoul(ptr, NULL, 16);
    
	return ret;
}


void param_menu_usage()
{
    printf("\r\n##### Parameter Set Menu #####\r\n");
    printf("[v] View the parameters\r\n");
    printf("[s] Set parameter \r\n");
    printf("[d] Delete parameter \r\n");
    printf("[w] Save your  parameters to flash memeory \r\n");
    printf("[q] Quit \r\n");
    printf("Enter your selection: ");
}


void param_menu_shell(void)
{
    char c;
    char cmd_buf[256];
    char name_buf[20];
    char val_buf[256];
    
    while (1)
    {
        param_menu_usage();
        c = awaitkey(-1, NULL);
        printf("%c\n", c);
        switch (c)
        {
            case 'v':
            {
                strcpy(cmd_buf, "printenv ");
                printf("Name(Hint:Press 'Enter' to view all paramters): ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);
                run_command(cmd_buf, 0);
                break;
            }
            
            case 's':
            {
                sprintf(cmd_buf, "setenv ");

                printf("Name: ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);

                printf("Value: ");
                readline(NULL);
                strcat(cmd_buf, " ");
                strcat(cmd_buf, console_buffer);

                run_command(cmd_buf, 0);
                break;
            }
            
            case 'd':
            {
                sprintf(cmd_buf, "setenv ");

                printf("Name: ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);

                run_command(cmd_buf, 0);
                break;
            }
            
            case 'w':
            {
                sprintf(cmd_buf, "saveenv");
                run_command(cmd_buf, 0);
                break;
            }
            
            case 'q':
            {
                return;
                break;
            }
        }
    }
}


void main_menu_usage(void)
{
    printf("\r\n#####      LT2440 U-Boot -2010.03     		  #####\r\n");
    printf("\r#####     Welcome to Embeded World   		  #####\r\n");
    printf("\r#####      	             designed by: LuTong  #####\r\n");
    printf("\r#####     		     www.lt-net.cn        #####\r\n");
    
    printf("[u] Download u-boot or STEPLDR.nb1 to Nand Flash\r\n");
    printf("[e] Download e-boot to Nand Flash\r\n");
 if (isNORFlash())
    printf("[n] Download u-boot to Nor Flash\r\n");
 
    printf("[k] Download Linux Kernel uImage to Nand Flash\r\n");

#ifdef CONFIG_CMD_JFFS2
    printf("[j] Download JFFS2  Image to Nand Flash\r\n");
#endif
    printf("[y] Download YAFFS2 Image to Nand Flash\r\n");
    printf("[g] Download to SDRAM and Run \r\n");
    printf("[d] Download Linux Kernel to SDRAM and RUN\r\n");
    printf("[f] Format the Nand Flash\r\n");
    printf("[l] Low-level Format the entire Nand Flash\r\n");
    printf("[s] Set the boot parameters\r\n");
    printf("[b] Boot the system\r\n");
    printf("[r] Reboot U-Boot\r\n");
    printf("[q] Quit from menu\r\n");
    printf("Enter your selection: ");
}


void menu_shell(void)
{
    char c;
    char cmd_buf[200];
    char *p = NULL;
    unsigned long size;
    unsigned long offset;
    struct mtd_info *mtd = &nand_info[nand_curr_device];

    while (1)
    {
        main_menu_usage();
        c = awaitkey(-1, NULL);
        printf("%c\n", c);
        switch (c)
        {
	    case 'u':
		{
                	strcpy(cmd_buf, "usbslave 1 0x3000a000; nand erase u-boot; nand write 0x3000a000 u-boot ");
                	run_command(cmd_buf, 0);
                	break;
		}

            case 'n':
            {
                if (isNORFlash())
                {
                    strcpy(cmd_buf, "usbslave 1 0x3000a000; protect off all; erase 0 5FFFF; cp.b 0x3000a000 0 $(filesize)");
                    run_command(cmd_buf, 0);
                }
			break;
            }

	    case 'e':
		{
                	strcpy(cmd_buf, "usbslave 1 0x3000a000; nand erase 80000 80000; nand write 0x3000a000 80000 80000 ");
                	run_command(cmd_buf, 0);
                	break;
		}
            
            case 'k':
            {
                strcpy(cmd_buf, "usbslave 1 0x3000a000; nand erase kernel; nand write 0x3000a000 kernel ");
                run_command(cmd_buf, 0);
                break;
            }
#if 0
            case 'j':
            {
                strcpy(cmd_buf, "usbslave 1 0x30008000; nand erase jffs2; nand write.jffs2 0x30008000 jffs2 $(filesize)");
                run_command(cmd_buf, 0);
                break;
            }
#endif
            case 'y':
            {
                strcpy(cmd_buf, "usbslave 1 0x3000a000; nand erase yaffs2; nand write.yaffs2 0x3000a000 yaffs2 $(filesize)");
                run_command(cmd_buf, 0);
                break;
            }

            case 'g':
            {
                extern volatile U32 downloadAddress;
                extern int download_run;
                
                download_run = 1;
                strcpy(cmd_buf, "usbslave 1");
                run_command(cmd_buf, 0);
                download_run = 0;
                sprintf(cmd_buf, "go %x", downloadAddress);
                run_command(cmd_buf, 0);
                break;
            }

			case 'd':
			{

				strcpy(cmd_buf, "usbslave 1 0x3000a000;bootm 0x3000a000");
				run_command(cmd_buf, 0);
			}

            case 'b':
            {
                printf("Booting Linux Nand Flash\n");
                strcpy(cmd_buf, "nand read.jffs2 0x3000a000 kernel; bootm 0x3000a000");
                run_command(cmd_buf, 0);
                break;
            }

            case 'f':
            {
                strcpy(cmd_buf, "nand erase ");

                printf("Start address: ");
                readline(NULL);
                strcat(cmd_buf, console_buffer);

                printf("Size(eg. 4000000, 0x4000000, 64m and so on): ");
                readline(NULL);
                p = console_buffer;
                size = memsize_parse2(p, &p);
                sprintf(console_buffer, " %x", size);
                strcat(cmd_buf, console_buffer);

                run_command(cmd_buf, 0);
                break;
            }

            case 's':
            {
                param_menu_shell();
                break;
            }
            case 'l':
            {
		//printf("Warning : the whole nand flash will be erased ,press y to erase ,any other key to return!");
		//c = awaitkey(-1, NULL);
		//if(c=='y')
		//{
                strcpy(cmd_buf, "nand scrub");
		run_command(cmd_buf, 0);
		//}
                break;
            }

            case 'r':
            {
				strcpy(cmd_buf, "reset");
				run_command(cmd_buf, 0);
                break;
            }
            
            case 'q':
            {
                return;    
                break;
            }

        }
                
    }
}

int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    menu_shell();
    return 0;
}

U_BOOT_CMD(
	menu,	3,	0,	do_menu,
	"menu - display a menu, to select the items to do something\n",
	" - display a menu, to select the items to do something"
);

