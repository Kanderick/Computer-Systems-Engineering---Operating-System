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

typedef struct fileDescriptor {
    struct fot *table;
    uint32_t *inode;
    uint32_t filePos;
    uint32_t flags;
} fd;

typedef struct fileArray {
    fd files[FA_SIZE];
    uint32_t fullFlag;
} fa;

typedef struct fileOperationTable {
    uint32_t *fotOpen;
    uint32_t *fotClose;
    uint32_t *fotRead;
    uint32_t *fotWrite;
} fot;

extern fot stdin, stdout, rtcTable, dirTable, regTable;
// extern file system
extern ece391_file_system_t   ece391FileSystem;
// utility functions
extern void init_file_system(unsigned int addr_start, unsigned int addr_end);
extern int32_t read_dentry_by_name(const uint8_t *fname, dentry_t* dentry);
extern int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
extern int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);
#endif
