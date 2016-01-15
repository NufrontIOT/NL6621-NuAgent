#include "common.h"
#include "fatfs.h"

#include "ff.h"
#include "integer.h"
#include "diskio.h"

void edit_file(void)
{
	FATFS fs;
	FIL	file;
	FRESULT res; 
	DIR dirs;
	FILINFO finfo;
	char key = 0;
	char path[20];

	uint32_t index = 0x00;
	uint32_t reindex = 0x00;
	uint8_t file_buff[512] = {0};

	uint32_t files_num = 0;
	uint8_t length = 0;
	res = f_mount(&fs, "", 0);
	if (res != FR_OK)
	{
		printf("\r\nMount file system failed, error no:%u", res);
		return;
	}

	res = f_opendir(&dirs,"/");
	printf("\r\n------------File List------------");
	if (res == FR_OK) 
	{
		while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0]) 
		{
			if (finfo.fattrib & AM_DIR)//如果是文件夹 
			{ 
				continue;
			} 
			else 
			{   
				files_num++;
				//显示文件名,显示文件实际大小 ,文件实际大小采用四舍五入法
				printf("\r\n/%12s%7ld KB ",  &finfo.fname[0],(finfo.fsize+512)/1024);
			}
		}
		if( files_num == 0 )//无文件
		{
			printf("\r\nNo file!");    
		}
	}
	else
	{
		printf("\r\nOpen Root file document failed!");
		printf("\r\nError code: %u",res);
	}
	printf("\r\nPlease input full name, 'Enter' key finish...");
	get_file_name((uint8_t *)path,length,FI);
	res = f_open(&file,path,FA_READ | FA_WRITE);
	if (res == FR_OK)
	{	
		printf("\r\nOpen file %s success", path);
		printf("\r\nIt is edit status, please input data!");		
		printf("\r\nPress 'ESC' or 'Ctrl+C' exit edit!\r\n");
		
		while(1)
		{
			key = USART_ReceiveData();
			//key = BSP_UartGetcPolled();
			if ((key == 0x1B) && (index == 0x00))//key ESC
			{
				printf("\r\nNo data input, it is editing...");								
				continue;
			}
			else if ((key == 0x1B))	//key ESC
			{
				printf("\r\nSave data mode...");
				res = f_write(&file,file_buff,index,&reindex);
				if ((res == FR_OK) && (reindex == index))
				{
					printf("\r\nSave data success!");
					f_close(&file);
					index = 0x00;
					reindex = 0x00;									
				}
				else
				{
					printf("\r\nSave data failed!");
					printf("\r\nError code: %u",res);									
				}
				break;
			}
			else if (key == 0x03) //key Ctrl+C
			{
				printf("\r\nFinish file edit!");
				printf("\r\nSave data...");
				res = f_write(&file,file_buff,index,&reindex);
				if ((res == FR_OK) && (reindex == index))
				{
					printf("\r\nSave data success!");
					f_close(&file);
					index = 0x00;
					reindex = 0x00;									
				}
				else
				{
					printf("\r\nSave data failed!");
					printf("\r\nError code: %u",res);									
				}
				break;
			}
			else if ((key < 0x21) || (key > 0x80))
			{
				continue;
			}
			else
			{
				file_buff[index++] = key;
				printf("%c",key);
				if (index > 512)
				{
					index = 0x00;
				}
			}
		}
	}
	else
	{
		printf("\r\nOpen file failed, error code: %u",res);
	}
}
void read_file(void)
{

	FATFS fs;
	FIL	file;
	FRESULT res; 
	DIR dirs;
	FILINFO finfo;
	char path[20];
	char buffer[512] = {0};
	uint32_t i;
	uint8_t length = 0;
	uint32_t re,files_num = 0;
	res = f_mount(&fs, "", 0);
	if (res != FR_OK)
	{
		printf("\r\nMount file system failed, error code: %u",res);
		return;
	}
	res = f_opendir(&dirs,"/");
	if (res == FR_OK) 
	{
		//i = strlen(path);
		printf("\r\n-----------File list ----------");
		while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0]) 
		{
			if (finfo.fattrib & AM_DIR)//如果是文件夹 
			{ 
				continue;
			} 
			else 
			{   
				files_num++;
				//显示文件名,显示文件实际大小 ,文件实际大小采用四舍五入法
				printf("\r\n/%12s%7ld KB ",  &finfo.fname[0],(finfo.fsize+512)/1024);
 			}
		}
		if( files_num == 0 )//无文件
		{
			printf("\r\nNo file, Please create file first!");
			return;
		}
	}
	else
	{
		printf("\r\nOpen root file ducument failed!");
		printf("\r\nError code: %u",res);
	}
	printf("\r\nPlease input file name, press 'Enter' finish....");
	get_file_name((uint8_t *)path,length,FI);
	res = f_open(&file, path, FA_READ);
	printf("\r\nIt is opening file data, data as follow:\r\n");

	if (res == FR_OK)
	{
		while (1)
		{
			for(i = 0;i < 512;i++)
			{
				buffer[i] = 0x00;
			}
			res = f_read(&file,buffer,512,&re);
			printf("%s",buffer);

			if (res || re == 0)
			{
				printf("\r\nRead file data finish, open it now!");
				f_close(&file);
				break;  
			}	
		}
	}
	f_mount(NULL, "", 0);
}

void creat_dir(void)
{
	FATFS fs;        
	FRESULT res;     
	char path[20];
	uint8_t length = 0;
	res = f_mount(&fs, "", 0);
	if (res != FR_OK)
	{
		printf("\r\nMount file system failed, error code: %u",res);
		return;
	}	
	printf("\r\nPlease input ducument name, press 'Enter' make sure....");
	get_file_name((uint8_t *)path,length,DI);
	res = f_mkdir(path);
	if (res == FR_OK)
	{
		printf("\r\nCreate file ducument success.!");
	}
	else
	{
		printf("\r\nCreate file ducument failed...");
		printf("\r\nError code: %u",res);
	}
	f_mount(NULL, "", 0);
}


void get_file_name(uint8_t *file_name,uint8_t length,uint8_t type)
{
	uint8_t key = 0;
	uint8_t name_leng = 0;
	printf("\r\n");
	
	while (1)
	{
		key = USART_ReceiveData();
		//key = BSP_UartGetcPolled();
		if ((key == 13) && (name_leng == 0))
		{
			printf("\r\n");
			continue;
		}
		else if ((key == 0x2F) || (key == 0x5C))
		{
			printf("%c",key);
			continue;
		}
		else if ((key == 13) && (name_leng > 0))
		{
			printf("\r\n");
			if (type == FI)
			{
				if (check_file_name(file_name,name_leng) == 0)
				{
					break;
				}
			}
			else
			{
				break;
			}			
			
		}
		else
		{
			printf("%c",key);
			file_name[name_leng] = key;
			name_leng++;
			if (name_leng > 12)
			{
				printf("\r\nFile format:8+3, Only support 8 charater, 3 extern name!");
				printf("\r\nPlease input again...");
				name_leng = 0;
				continue;
			}
		}
	}
}

void Fatfs_Init(void)
{
	 format_disk();
}

void format_disk(void)
{
	FATFS fs;
	uint8_t res = 0;
	res = f_mount(&fs, "", 0);
	if (res != FR_OK)
	{
		printf("\r\nMount file system failed,Error code: %u\n",res);
		return;
	}	
	printf("\r\nIt is formating, please waiting...\n");

	res = f_mkfs("", 1, 4096);
	if (res == FR_OK)
	{
		printf("\r\nFormat success...\n");
	}
	else
	{
		printf("\r\nFormat failure...");
		printf("\r\nError code: %u\r\n",res);
	}
	f_mount(NULL, "", 0);
}
void creat_file(void)
{
	FIL file;
	FIL *pf = &file;
	FATFS fs;
	uint8_t res = FR_OK;
	uint8_t name[16] = {0};
	uint8_t length = 0;
	printf("\r\nPlease input file name: 8+3 format:");
	printf("\r\nFor example:123.dat\r\n");
	get_file_name(name,length,FI);	
	res = f_mount(&fs, "", 0);		                           /* Mount a Logical Drive 0 */
	if (res != FR_OK)
	{
		printf("\r\nMount file system failed,Error code: %u\n",res);
		return;
	}	
	res = f_open(pf, (TCHAR *)name, FA_READ | FA_WRITE | FA_CREATE_NEW);
	if (res == FR_OK)
	{
		printf("\r\nCreate file success!\n");
		res = f_close(pf);
		if (res != FR_OK)
		{
			printf("\r\nCreate file success, But close failure!\n");
			printf("\r\nError code: %u",res);				
		}				
	}
	else
	{
		printf("\r\nCreate file failed!");
		printf("\r\nError code: %u\n",res);	
	}
	f_mount(NULL, "", 0);
}
void delete_file(void)
{
	FATFS fs;
	FRESULT res = FR_OK;
	DIR dirs;
	FILINFO finfo;
	uint8_t name[16] = {0};
	uint8_t length = 0;
	uint32_t files_num = 0;

	res = f_mount(&fs, "", 0);		      /* Mount a Logical Drive 0*/	 
	if (res != FR_OK)
	{
		printf("\r\nMount file system failed,Error code: %u",res);
		return;
	}

	res = f_opendir(&dirs,"/");		 /* Create a Directroy Object */
	if (res == FR_OK) 
	{
		printf("\r\n-----------File list-------");
		while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0]) 
		{
			if (finfo.fattrib & AM_DIR)//如果是文件夹 
			{ 
				continue;
			} 
			else 
			{   
				files_num++;
				//显示文件名,显示文件实际大小 ,文件实际大小采用四舍五入法
				printf("\r\n/%12s%7ld KB ",  &finfo.fname[0],(finfo.fsize+512)/1024);
 			}
		}
		if( files_num == 0 )//无文件
		{
			printf("\r\nNo file, Please create if first!");
			return;
		}
	}

	get_file_name(name,length,FI);    /* Get file name */
	
	res = f_unlink((TCHAR *)name);	  /* Delete a File or Directory */

	if (res == FR_OK)
	{
		printf("\r\nDelete file success!");
	}
	else if (res == FR_NO_FILE)
	{
		printf("\r\nCan not find file or document!");
	}
	else if (res == FR_NO_PATH)
	{
		printf("\r\nCan not find file document!");
	}
	else
	{
		printf("\r\nError code: %u",res);
	}
	f_mount(NULL, "", 0);
}
void list_file(void)
{
	FATFS fs;
	FILINFO finfo;
	FRESULT res = FR_OK;
	DIR dirs;
	int i;
	int files_num=0;
	res = f_mount(&fs, "", 0);
	if (res != FR_OK)
	{
		printf("\r\nMount file system failed, Error code: %u",res);
		return;
	}	
	res = f_opendir(&dirs,"/");							/* Create a Directroy Object */
	printf("\r\n------------File list------------");
	if (res == FR_OK)
	{
		while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0]) 	 /* Read Directory Entry in Sequense*/
		{
			i = strlen(finfo.fname);
			if (finfo.fattrib & AM_DIR)//如果是文件夹 
			{
				files_num++;
				printf("\r\n/%s", &finfo.fname[0]);
				switch(i)//作用：输出文件名左对齐
				{
				case 1:printf(" ");
				case 2:printf(" ");
				case 3:printf(" ");
				case 4:printf(" ");
				case 5:printf(" ");
				case 6:printf(" ");
				case 7:printf(" ");
				case 8:printf("%15s"," ");
				}	
			} 
			else 
			{
				continue;
			}	
		}
	}
	else
	{
		printf("\r\nOpen root document failed!");
		printf("\r\nError code: %u",res);
	}
	res = f_opendir(&dirs, "/");
	if (res == FR_OK) 
	{
		while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0]) 
		{
			if (finfo.fattrib & AM_DIR) 
			{ 
				continue;
			} 
			else 
			{   
				files_num++;				
				printf("\r\n/.%12s%7ld KB ",  &finfo.fname[0],(finfo.fsize+512)/1024);				
			}
		}
		if( files_num==0 )//无文件
		{
			printf("\r\nNo file!");    
		}
	}
	else
	{
		printf("\r\nOpen root document failed!");
		printf("\r\nError code: %u",res);
	}
	f_mount(NULL, "", 0);

}
	FATFS fs;
	FATFS *fls = &fs;
	FRESULT res;
	DWORD fre_clust;
void get_disk_info(void)
{
	
	
	res = f_mount(&fs, "", 0);						 /* Mount a Logical Drive 0 */
	if (res != FR_OK)
	{
		printf("\r\nMount file system failed,Error code: %u",res);
		return;
	}	

	res = f_getfree("/",&fre_clust,&fls);		 /* Get Number of Free Clusters */
	if (res == FR_OK) 
	{
	                                             /* Print free space in unit of MB (assuming 4096 bytes/sector) */
        printf("\r\n%d KB Total Drive Space.\r\n"
               "%d KB Available Space.\r\n",
               (fls->n_fatent-2)*4,fre_clust*4);
	}
	else
	{
		printf("\r\nGet disk info failure!");
		printf("\r\nError code: %u",res);
	}																									   
	f_mount(NULL, "", 0);		/*Unmount a Logical Drive 0 */ 
}
uint8_t check_file_name(uint8_t *file_name,uint8_t length)
{
	uint8_t res;
	if (length > 13)
	{
		res = 1;
	}
	else
	{
		if (file_name[length - 4] == '.')
		{
			res = 0;
		}
		else
		{
			res = 2;
		}
	}
	return res;

}



