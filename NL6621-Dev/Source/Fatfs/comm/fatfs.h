#ifndef _COMMON_H__
#define _COMMON_H__  1

#include "diskio.h"

#define FI 1
#define DI 2

#if 1
#define malloc OSMMalloc
#define calloc OSMCalloc
#define free   OSMFree
#endif

typedef unsigned char uint8_t;
//typedef char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;


void fat_menu_init(void);
void get_file_name(uint8_t *file_name,uint8_t length,uint8_t type);
uint8_t check_file_name(uint8_t *file_name,uint8_t length);

void format_disk(void);
void creat_file(void);
void delete_file(void);
void list_file(void);
void get_disk_info(void);
void creat_dir(void);
void edit_file(void);
void read_file(void);

void Fatfs_Init(void);

#endif

