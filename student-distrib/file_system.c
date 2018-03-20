#include "file_system.h"
#include "lib.h"
/* the hierarchy for ece391 file system */
static ece391_file_system_t   ece391FileSystem;

/* Need header */
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

/* Need header */
int32_t read_dentry_by_name(const uint8_t *fname, dentry_t* dentry){
    unsigned int i, j;
    // Initialize find flag and index
    uint8_t find_flag = 0;
    uint32_t index=0;
    // find name string's length
    const uint32_t fname_len = strlen(fname);
    // check if the file system is initialized
    if(ece391FileSystem.ece391_boot_block == NULL || \
       ece391FileSystem.ece391_inodes == NULL ||\
       ece391FileSystem.ece391_data_blocks == NULL){
        printf("ECE391 file system is not valid. Please initialze it first.\n");
        return -1;
    }
    // check if input string name valid
    if(fname_len>FILE_NAME_LEN){
        printf("Input string too long.\n");
        return -1;
    }
    // search for the input fname
    for(i = 0; i < ece391FileSystem.dir_count; i++){    /* traverse each directories */
        find_flag = 1;
        for(j = 0; j < fname_len; j++){     /* traverse each directory's name string */
            if(find_flag==0)
                continue;
            if(fname[j]!=ece391FileSystem.ece391_boot_block->direntries[i].filename[j]) /* Need optimizing */
                find_flag = 0;
        }
        if(find_flag==1){
            index = i;
            break;
        }
    }
    // name not found
    if(find_flag==0){
        printf("Directory not found.\n");
        return -1;
    }
    // pass found index to function read_dentry_by_index
    return read_dentry_by_index(index, dentry);
}
/* Need header */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
    unsigned int i;
    // check if the file system is initialized
    if(ece391FileSystem.ece391_boot_block == NULL || \
       ece391FileSystem.ece391_inodes == NULL ||\
       ece391FileSystem.ece391_data_blocks == NULL){
        printf("ECE391 file system is not valid. Please initialze it first.\n");
        return -1;
    }
    // check if index is in bondary
    if(index >= ece391FileSystem.dir_count){
        printf("Input index outof bondary. \n");
        return -1;
    }
    // copy the dir name
    for(i = 0; i < FILE_NAME_LEN; i++){
        dentry->filename[i] = ece391FileSystem.ece391_boot_block->direntries[index].filename[i]; /* Need optimizing */
    }
    // copy the file type
    dentry->filetype = ece391FileSystem.ece391_boot_block->direntries[index].filetype;
    // if filetype is regular then copy the inode
    if(dentry->filetype == 2)
        dentry->inode_num = ece391FileSystem.ece391_boot_block->direntries[index].inode_num;
    return 0;

}
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    uint32_t buf_index = 0;
    uint32_t data_block_index = 0;
    uint32_t data_byte_index=0;
    uint32_t copy_position;
    // check if the file system is initialized
    if(ece391FileSystem.ece391_boot_block == NULL || \
       ece391FileSystem.ece391_inodes == NULL ||\
       ece391FileSystem.ece391_data_blocks == NULL){
        printf("ECE391 file system is not valid. Please initialze it first.\n");
        return -1;
    }

    // check if inode input is in boundary
    if(inode >= ece391FileSystem.inode_count){
        printf("Input inode out of bondary. \n");
        return -1;
    }

    // check if offset is in boundary
    if(offset >= ece391FileSystem.ece391_inodes[inode].length){
        printf("invalid input offset. \n");
        return -1;
    }
    copy_position = offset;
    data_block_index = offset / DATA_BLOCK_LEN_;
    data_byte_index = offset % DATA_BLOCK_LEN_;
    while(copy_position < ece391FileSystem.ece391_inodes[inode].length){
        if(data_byte_index == DATA_BLOCK_LEN_){
            data_byte_index = 0;
            data_block_index ++;
        }
        buf[buf_index] = ece391FileSystem.ece391_data_blocks[ece391FileSystem.ece391_inodes[inode].data_block_num[data_block_index]].data_byte[data_byte_index];
        length--;
        copy_position++;
        data_byte_index++;
        buf_index++;
        // copy finished
        if(length==0)
            return buf_index;
    }
    // end of file is reached
    return 0;

}
