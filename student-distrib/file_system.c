#include "file_system.h"
#include "lib.h"
static ece391_file_system_t   ece391FileSystem;
void init_file_system(unsigned int addr_start, unsigned int addr_end){
    unsigned int addr;
    ece391FileSystem.dir_count = 0;
    ece391FileSystem.inode_count = 0;
    ece391FileSystem.data_count = 0;
    // check if pointer valid
    if (addr_start==0 || addr_end<=addr_start){
        printf("ECE391 file system input address invalid.\n");
        ece391FileSystem.ece391_boot_block = NULL;
        ece391FileSystem.ece391_inodes = NULL;
        ece391FileSystem.ece391_data_blocks = NULL;
        return;
    }
    // parsing the boot block
    ece391FileSystem.ece391_boot_block = (boot_block*) addr_start;
    // check if boot statistic valid
    /* file system directory number*/
    if (ece391FileSystem.ece391_boot_block->dir_count > DIRENTRIES_NUM){
        printf("ECE391 file system directory number invalid.\n");
        ece391FileSystem.ece391_boot_block = NULL;
        ece391FileSystem.ece391_inodes = NULL;
        ece391FileSystem.ece391_data_blocks = NULL;
        return;
    }
    /* file inodes length plus data block length in bound */
    if ((ece391FileSystem.ece391_boot_block->inode_count + \
        ece391FileSystem.ece391_boot_block->data_count + 1)*BLOCK_SIZE_4KB  > addr_end - addr_start + 1){
        printf("ECE391 file system data out of boundary.\n");
        ece391FileSystem.ece391_boot_block = NULL;
        ece391FileSystem.ece391_inodes = NULL;
        ece391FileSystem.ece391_data_blocks = NULL;
        return;
    }
    // update the file system statistic
    ece391FileSystem.dir_count = ece391FileSystem.ece391_boot_block->dir_count;
    ece391FileSystem.inode_count = ece391FileSystem.ece391_boot_block->inode_count;
    ece391FileSystem.data_count = ece391FileSystem.ece391_boot_block->data_count;
    // parsing the inodes
    addr = addr_start+BLOCK_SIZE_4KB;
    ece391FileSystem.ece391_inodes = (inode*) addr;
    // parsing the data blocks
    addr += BLOCK_SIZE_4KB*ece391FileSystem.ece391_boot_block->inode_count;
    ece391FileSystem.ece391_data_blocks = (data_block*) addr;
    return;
}
int32_t read_dentry_by_name(const uint8_t *fname, dentry_t* dentry){
    unsigned int i, j;
    // Initialize find flag and index
    uint8_t find_flag = 0;
    uint32_t index=0;
    // find name string's length
    const uint32_t fname_len = strlen(fname);
    // check if input string name valid
    if(fname_len>FILE_NAME_LEN){
        printf("Input string too long\n");
        return -1;
    }
    for(i = 0; i < ece391FileSystem.dir_count; i++){
        for(j = 0; j < fname_len; j++){
            if(fname)
        }
    }
}
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){

}
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){

}
