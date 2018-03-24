#ifndef FILE_SYSTEM
#define FILE_SYSTEM

#include "types.h"
// dentry parameters
#define FILE_NAME_LEN           32
#define DENTRY_RESERVED         24
// boot_block parameters
#define BOOT_BLOCK_RESERVED     52
#define DIRENTRIES_NUM          63
// inode parameter
#define DATA_BLOCK_NUM_         1023
// data_block parameter
#define DATA_BLOCK_LEN_         4096
// file system parameter
#define FILESYS_IMG_LEN         0x7c000
#define BLOCK_SIZE_4KB          4096

#define FA_SIZE                 8
/*  This is the data structure that holds the dentry for each regular file,
 *  directory, or RTC access file. ECE391 file system has 63 dentry
 *  in total. They are placed in the first block, boot_block, in the
 *  ece391 filesys.img.
 *
 *  note: 64B without padding
 */
struct dentry{
    int8_t filename[FILE_NAME_LEN];
    int32_t filetype;
    int32_t inode_num;
    int8_t reserved[DENTRY_RESERVED];
} __attribute((packed));
typedef struct dentry dentry_t;

/*  THis is the data structure that holds both the statistics of the file
 *  system and all the dentries. This is the first block of the
 *  ece391 file system.
 *
 *  note: 4kB without padding
 */
struct boot_block{
    int32_t dir_count;
    int32_t inode_count;
    int32_t data_count;
    int8_t reserved[BOOT_BLOCK_RESERVED];
    dentry_t direntries[DIRENTRIES_NUM];
}__attribute((packed));
typedef struct boot_block boot_block_t;

/*  This is the data structure that holds inode of each regular file in
 *  ece39 file system. An inode holds the information, not the data, of
 *  a file.
 *
 *  note: 4kB without padding
 */
struct inode{
    int32_t length;     /* in bytes */
    int32_t data_block_num[DATA_BLOCK_NUM_];
}__attribute((packed));
typedef struct inode inode_t;

/*  This is the data_block of the file system.
 *
 *  note: 4kB without padding*/
struct data_block{
    int8_t data_byte[DATA_BLOCK_LEN_];
}__attribute((packed));
typedef struct data_block data_block_t;

/*  This is the whole ece391 file system. This data structure contians three consecutive
 *  array pointers.
 *
 *  note: parse the file system
 */
struct ece391_file_system{
    boot_block_t* ece391_boot_block;
    inode_t*  ece391_inodes;
    data_block_t* ece391_data_blocks;
    int32_t dir_count;
    int32_t inode_count;
    int32_t data_count;
}__attribute((packed));
typedef struct ece391_file_system ece391_file_system_t;
// extern file system
extern ece391_file_system_t   ece391FileSystem;
// utility functions
extern void init_file_system(unsigned int addr_start, unsigned int addr_end);
/* needs header!!! */
extern int32_t read_dentry_by_name(const uint8_t *fname, dentry_t* dentry);
/* needs header!!! */
extern int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
/* needs header!!! */
extern int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

/* for check point 2 */
#define MAX_FILE_OPEN   17
#define STATUS_CLOSED   67      // 'C' indecates file closed
#define STATUS_OPENED   79      // 'O' indicates file opened
/* This is the file array to record file open/close status.
 *
 *  note: only for check point 3.2
 */
 struct file_status_array{
    // files status
    dentry_t FILE_TO_OPEN[MAX_FILE_OPEN];
    uint8_t FILE_STATUS[MAX_FILE_OPEN];
    uint32_t FILE_OFFSET[MAX_FILE_OPEN];
    // directories status
    dentry_t DIR_TO_OPEN[MAX_FILE_OPEN];
    uint8_t DIR_STATUS[MAX_FILE_OPEN];
 };
 typedef struct file_status_array file_status_array_t;


// following functions are for system call
/* needs header!!! */
extern void init_file_status_array(file_status_array_t* array);
/* needs header!!! a neat helper function */
extern int32_t file_find    (const uint8_t* filename);
/* needs header!!! */
extern int32_t file_read    (int32_t fd, void* buf, int32_t nbytes);
/* needs header!!! */
extern int32_t file_write   (int32_t fd, const void* buf, int32_t nbytes);
/* needs header!!! */
extern int32_t file_open    (const uint8_t* filename);
/* needs header!!! */
extern int32_t file_close   (int32_t fd);

extern int32_t dir_read    (int32_t fd, void* buf, int32_t nbytes);
extern int32_t dir_write   (int32_t fd, const void* buf, int32_t nbytes);
extern int32_t dir_open    (const uint8_t* filename);
extern int32_t dir_close   (int32_t fd);
#endif