/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "common.h"
#include "diskio.h"		/* FatFs lower layer API */
//#include "BspDemo.h"	/* Example: Header file of existing SPI flash control module */

/* Nor flash driver resource */
#define NOR_FLASH_SECTOR_SIZE           (512)
#define NOR_FLASH_START_ADDR			(52 * NOR_FLASH_SECTOR_SIZE * 8)		/* 208k Bytes 0x34000*/
#define NOR_FLASH_SECTOR_COUNT			(73*8)/*(292 * 2)*/  			/* 220KBytes ~ 512KBytes */

#define malloc          OSMMalloc
#define free            OSMFree

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	if (pdrv) return STA_NOINIT;    	/* Supports only drive 0 */
	return 0;     				/* Return disk status */
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	if (pdrv) return STA_NOINIT;	/* Supports only drive 0 */			

	//BSP_QSpiInit();

	return 0; 
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	int i;
	UINT32 ReadOffset = (NOR_FLASH_SECTOR_SIZE) * sector;
	
	if (pdrv) return RES_PARERR;	/* Supports only drive 0 */
	printf("Read data from norflash(sector:%d,count:%d, addr:0x%x).\n", sector, count, NOR_FLASH_START_ADDR + ReadOffset);
	for (i = 0; i < count; i++)
	{
		QSpiFlashRead(NOR_FLASH_START_ADDR + ReadOffset, buff, NOR_FLASH_SECTOR_SIZE);
		ReadOffset += (NOR_FLASH_SECTOR_SIZE * i);
		buff += NOR_FLASH_SECTOR_SIZE;
	}

	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	int i;
    UINT32 WriteSector = sector / 8;    /* 物理扇区起始地址 */
    UINT32 WritePoint = sector % 8;     /* 物理扇区中具体的块地址 */
    UINT32 SectorCnt = count;
    UINT8 *tempbuf = NULL;

	if (pdrv) return RES_PARERR;	/* Supports only drive 0 */
    
    tempbuf = malloc(FLASH_SECTOR_SIZE);
    
    /* 一个4k flash sector 能够完成所有写数据操作 */
    if (SectorCnt <= (8 - WritePoint)) {
        QSpiFlashRead(NOR_FLASH_START_ADDR + (WriteSector * FLASH_SECTOR_SIZE), tempbuf, FLASH_SECTOR_SIZE);        
        memcpy((UINT8*)(tempbuf + (WritePoint * NOR_FLASH_SECTOR_SIZE)), (UINT8*)buff, SectorCnt * NOR_FLASH_SECTOR_SIZE);        
        QSpiWriteOneSector(NOR_FLASH_START_ADDR + (WriteSector * FLASH_SECTOR_SIZE), tempbuf);
        printf("Write one sector(sector:%d,count:%d, addr:0x%x).\n", sector, count, NOR_FLASH_START_ADDR + (WriteSector * FLASH_SECTOR_SIZE));
        
    } else { /* 写数据横跨2个以上的nor flash sector（4K） */
        /* 写前面一个物理sector */   
        QSpiFlashRead(NOR_FLASH_START_ADDR + (WriteSector * FLASH_SECTOR_SIZE), tempbuf, FLASH_SECTOR_SIZE);        
        memcpy((UINT8*)(tempbuf + (WritePoint * NOR_FLASH_SECTOR_SIZE)), (UINT8*)buff, (8 - WritePoint) * NOR_FLASH_SECTOR_SIZE);        
        QSpiWriteOneSector(NOR_FLASH_START_ADDR + (WriteSector * FLASH_SECTOR_SIZE), tempbuf);

        /* 剩下的sector个数 */
        SectorCnt -= (8 - WritePoint);
        for (i = 1; SectorCnt > 0; i++) 
        {
            QSpiFlashRead(NOR_FLASH_START_ADDR + ((WriteSector + i) * FLASH_SECTOR_SIZE), tempbuf, FLASH_SECTOR_SIZE);
            if (SectorCnt > 8)
            {
                memcpy((UINT8*)tempbuf, (UINT8*)(buff + ((count - SectorCnt) * NOR_FLASH_SECTOR_SIZE)), FLASH_SECTOR_SIZE);
                SectorCnt -= 8;
                
            } else {
                memcpy((UINT8*)tempbuf, (UINT8*)(buff + ((8 - WritePoint) * NOR_FLASH_SECTOR_SIZE)), SectorCnt * NOR_FLASH_SECTOR_SIZE); 
                SectorCnt = 0;
            }
            QSpiWriteOneSector(NOR_FLASH_START_ADDR + ((WriteSector + i) * FLASH_SECTOR_SIZE), tempbuf);            
        }        
    }

	if(tempbuf != NULL)
	    free(tempbuf);
	
	return RES_OK;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;

	if (pdrv) return RES_PARERR;	/* Supports only drive 0 */
	
	switch(cmd)
	{
		case CTRL_SYNC :
			break;
		
		case GET_SECTOR_COUNT:
			*(DWORD*)buff = NOR_FLASH_SECTOR_COUNT;
			break;
		
		case GET_SECTOR_SIZE:
			*(DWORD*)buff = NOR_FLASH_SECTOR_SIZE;
			break;
        
        case GET_BLOCK_SIZE:
            //*(DWORD*)buff = NOR_FLASH_SECTOR_SIZE;
		  	*(DWORD*)buff = 8;
			break;
		case CTRL_TRIM:
			break;
			
		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif



DWORD get_fattime(void)
{
	return 0;
}
