/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2025        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#ifdef _WIN32

#include "ff.h"			/* Basic definitions of FatFs */
#include "diskio.h"		/* Declarations FatFs MAI */
#include "disk_img_win.h"

/* Example: Declarations of the platform and disk functions in the project */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include <time.h>

/* Example: Mapping of physical drive number for each drive */
#define DEV_FLASH	0	/* Map FTL to physical drive 0 */
#define DEV_MMC		1	/* Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Map USB MSD to physical drive 2 */


#define SECTOR_SIZE     512

static FILE* disk_fp = NULL;
static DWORD disk_status_val = STA_NOINIT;
static uint32_t sector_count = 0;

/* Открываем/создаём образ */
static int open_disk_image(void) 
{
    if (disk_fp) return 1;
    disk_fp = fopen(fatfs_image_path, "rb+");
    if (!disk_fp) 
    {
        disk_fp = fopen(fatfs_image_path, "wb+");
        if (!disk_fp) return 0;
        // Опционально: заполнить образ нулями или FAT-структурой
    }
    fseek(disk_fp, 0, SEEK_END);
    long size = ftell(disk_fp);
    sector_count = (uint32_t)(size / SECTOR_SIZE);
    disk_status_val &= ~STA_NOINIT;
    return 1;
}


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    if (pdrv != 0) return RES_PARERR;
    return disk_status_val;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    if (pdrv != 0) return RES_PARERR;
    if (open_disk_image()) {
        disk_status_val &= ~(STA_NOINIT | STA_NODISK);
        return RES_OK;
    }
    return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
    if (pdrv != 0 || !buff || sector + count > sector_count) return RES_PARERR;
    if (fseek(disk_fp, sector * SECTOR_SIZE, SEEK_SET) != 0) return RES_ERROR;
    if (fread(buff, SECTOR_SIZE, count, disk_fp) != count) return RES_ERROR;
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
    if (pdrv != 0 || !buff || sector + count > sector_count) return RES_PARERR;
    if (fseek(disk_fp, sector * SECTOR_SIZE, SEEK_SET) != 0) return RES_ERROR;
    if (fwrite(buff, SECTOR_SIZE, count, disk_fp) != count) return RES_ERROR;
    return RES_OK;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    if (pdrv != 0) return RES_PARERR;
    switch (cmd) {
        case CTRL_SYNC:
            if (disk_fp) fflush(disk_fp);
            return RES_OK;
        case GET_SECTOR_COUNT:
            *(LBA_t*)buff = sector_count;
            return RES_OK;
        case GET_SECTOR_SIZE:
            *(WORD*)buff = SECTOR_SIZE;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *(DWORD*)buff = 1; // Для симулятора не критично
            return RES_OK;
        default:
            return RES_PARERR;
    }
}

#endif
