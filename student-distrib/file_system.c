/*  @ brif This file parses the given ece391 file system image
 *          and contains basic APIs to interact with files.
 *
 */

#include "file_system.h"
#include "rtc.h"
#include "lib.h"
/* the hierarchy for ece391 file system */
ece391_file_system_t   ece391FileSystem;
file_status_array_t fileStatusArray;

/*
 * init_file_system
 *   DESCRIPTION: initialized out own ece391_file_system structure
 *   INPUTS: addr_start -- starting address of the fileimg
             addr_end -- ending address of the fileimg
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: copys the information of the fileimg into local datastructure
 */
void init_file_system(unsigned int addr_start, unsigned int addr_end){
    unsigned int addr;
    printf("\n\n\nCHECK_POINT_1\n");
    ece391FileSystem.dir_count = 0;
    ece391FileSystem.inode_count = 0;
    ece391FileSystem.data_count = 0;
    printf("CHECK_POINT_2\n");
    // check if pointer valid
    if (addr_start==0 || addr_end<=addr_start){
        printf("ECE391 file system input address invalid.\n");
        ece391FileSystem.ece391_boot_block = NULL;
        ece391FileSystem.ece391_inodes = NULL;
        ece391FileSystem.ece391_data_blocks = NULL;
        return;
    }
    // parsing the boot block
    ece391FileSystem.ece391_boot_block = (boot_block_t*) addr_start;
    printf("CHECK_POINT_3\n");
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
    printf("ece391FileSystem.dir_count %d\n", (unsigned long)ece391FileSystem.dir_count);
    printf("ece391FileSystem.inode_count %d\n", (unsigned long)ece391FileSystem.inode_count);
    printf("ece391FileSystem.data_count %d\n", (unsigned long)ece391FileSystem.data_count);

    // parsing the inodes
    addr = addr_start+BLOCK_SIZE_4KB;
    ece391FileSystem.ece391_inodes = (inode_t*) addr;
    // parsing the data blocks
    addr += BLOCK_SIZE_4KB*ece391FileSystem.ece391_boot_block->inode_count;
    ece391FileSystem.ece391_data_blocks = (data_block_t*) addr;
    // initialze the file status array for open/close operation, only for Checkpoint 2
    init_file_status_array(&fileStatusArray);
    return;
}

/*
 * read_dentry_by_name
 *   DESCRIPTION: finds the dentry corresponding to the filename
 *   INPUTS: fname -- filename we are searching for
             dentry -- the emtpy pre-allocated dentry pointer to copy information into
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success and -1 for fail
 *   SIDE EFFECTS: copys the dentry informaiton for the desired filename
 */
int32_t read_dentry_by_name(const uint8_t *fname, dentry_t* dentry){
    unsigned int i, j;
    // Initialize find flag and index
    uint8_t find_flag = 0;
    uint32_t index=0;
    // find name string's length
    uint32_t fname_len = strlen((int8_t *)fname);
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
        fname_len=FILE_NAME_LEN;
    }
    // search for the input fname
    for(i = 0; i < ece391FileSystem.dir_count; i++){    /* traverse each directories */
        find_flag = 1;
        for(j = 0; j < fname_len; j++){     /* traverse each directory's name character */
            if(find_flag==0)
                continue;
            if(fname[j]!=ece391FileSystem.ece391_boot_block->direntries[i].filename[j]) /* Need optimizing */
                find_flag = 0;
        }
        // check EOS
        if(fname_len!=FILE_NAME_LEN && find_flag == 1){
            if(ece391FileSystem.ece391_boot_block->direntries[i].filename[fname_len]!=NULL)
                find_flag = 0;
        }
        // find the correct name
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

/*
 * read_dentry_by_index
 *   DESCRIPTION: finds the dentry corresponding to the index
 *   INPUTS: index -- index we are searching for
             dentry -- the emtpy pre-allocated dentry pointer to copy information into
 *   OUTPUTS: none
 *   RETURN VALUE: 0 for success and -1 for fail
 *   SIDE EFFECTS: copys the dentry informaiton for the desired index
 */
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

#define MAX_BLOCK_INDEX         1023        // in inodes' 4kB's memory has 1023 block numbers at large
/*
 * read_data
 *   DESCRIPTION: copys length bytes of information into the buffer
 *   INPUTS: inode -- inode containing the data
             offset -- offset to start reading from
             buf -- buffer to copy data into
             length -- number of bytes to copy
 *   OUTPUTS: none
 *   RETURN VALUE: -1 for fail and non-negative number for actual number of bytes read into buffer
 *   SIDE EFFECTS: none
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    uint32_t buf_index = 0;
    uint32_t data_block_index = 0;
    uint32_t data_byte_index = 0;
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
        printf("The file has reach its end, nothing to read.\n");
        return 0; // should return Zero
    }
    // start to copy the string
    copy_position = offset;
    data_block_index = offset / DATA_BLOCK_LEN_;
    data_byte_index = offset % DATA_BLOCK_LEN_;
    // if not reach end, keep copying
    while(copy_position < ece391FileSystem.ece391_inodes[inode].length){
        if(data_byte_index == DATA_BLOCK_LEN_){
            data_byte_index = 0;
            data_block_index ++;
        }
        // check if data_block_index is out of inodes' block boundary
        if (data_block_index >= MAX_BLOCK_INDEX){
            printf("data block index is out of inode block boundary, failed to keep reading. \n");
            return -1;
        }
        // check if data block num is out of ece391 file system boundary
        if (ece391FileSystem.ece391_inodes[inode].data_block_num[data_block_index] >= ece391FileSystem.data_count ){
            printf("Inode contains corrupted info and trys to access the memory that is out of the boundary of ece391 file syste, failed to keep reading.");
            return 0;
        }
        // copy this byte
        buf[buf_index] = ece391FileSystem.ece391_data_blocks[ece391FileSystem.ece391_inodes[inode].data_block_num[data_block_index]].data_byte[data_byte_index];
        // prepare to copy next byte
        length--;
        copy_position++;
        data_byte_index++;
        buf_index++;
        // copy finished
        if(length==0)
            return buf_index;
    }
    // end of file is reached, return the number of bytes copyed
    return ece391FileSystem.ece391_inodes[inode].length - offset;
}

// following are higher level APIs to interact with file system, these functions are expected to be called
/*
 * init_file_status_array
 *   DESCRIPTION: initialized the fileStatusArray structure
 *   INPUTS: array -- the fileStatusArray we want to initailize
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void init_file_status_array(file_status_array_t* array){
    uint32_t ii;    // traverse to initialize status file/dir array
    for(ii = 0; ii < MAX_FILE_OPEN; ii++){
        array->FILE_STATUS[ii] = STATUS_CLOSED;
        array->FILE_OFFSET[ii] = 0;
        array->CURRENT_DIR_IDX = 0;
        array->RTC_STATUS = STATUS_CLOSED;
    }
    return;
}
// for SYSTEM CALL
// following functions are for system call

/*
 * file_open
 *   DESCRIPTION: opens a non-directory file
 *   INPUTS: filename -- the file we want to open
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success and -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t file_open    (const uint8_t* filename){
    int32_t ii;    // traverse to check status file/dir array
    int32_t i;     // traverse to check name string
    int32_t new_fd = MAX_FILE_OPEN;  // if can open a file, this will record the fd
    const int32_t file_name_length = strlen((int8_t*)filename);
    int8_t already_open_flag = 0;

    if (*filename == '.') {
      printf("Cannot open directory file.\n");
      return -1;
    }

    if (file_name_length > FILE_NAME_LEN ){
        printf("Input string too long.\n");
        return -1;
    }
    // traverse the open file array
    for (ii = 0; ii < MAX_FILE_OPEN; ii++){
        // check each opened file
        if (fileStatusArray.FILE_STATUS[ii] == STATUS_OPENED){
            already_open_flag = 1; // initialize as opened
            // check if the name is the same
            for (i = 0; i < file_name_length; i++){
                if (filename[i] != fileStatusArray.FILE_TO_OPEN[ii].filename[i]){
                    already_open_flag = 0;
                    break;
                }
            }
            // if it is the same, handle the error and return on failure
            if (already_open_flag == 1){
                printf("The file is already opened.\n");
                return -1;
            }
        }
        // try to update the new_fd for later use
        else if (ii < new_fd)
            new_fd = ii;

    }
    // if the file status array is full
    if (new_fd == MAX_FILE_OPEN){
        printf("The file processing array is full.\n");
        return -1;
    }
    // now is OK to really try to look for the file in the ece391FILE_SYSTEM
    if(read_dentry_by_name(filename, (fileStatusArray.FILE_TO_OPEN) + new_fd) == -1){  /* trying to copy dentry if exist*/
        printf("The file does not exist.\n");
        return -1;
    }
    // update the opened file's status
    fileStatusArray.FILE_STATUS[new_fd] = STATUS_OPENED;
    fileStatusArray.FILE_OFFSET[new_fd] = 0;
    if (fileStatusArray.FILE_TO_OPEN[new_fd].filetype == 0) {
      fileStatusArray.RTC_STATUS = STATUS_OPENED;
    }
    return 0;
}

/*
 * file_close
 *   DESCRIPTION: closes a non-directory file
 *   INPUTS: fd -- the file index in the fileStatusArray of the file we want to close
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success and -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t file_close   (int32_t fd){
    // check fd valid
    if (fd >= MAX_FILE_OPEN || fd < 0){
        printf("The input index is out of boundary. \n");
        return -1;
    }
    // check if status is already closed
    if (fileStatusArray.FILE_STATUS[fd] == STATUS_CLOSED){
        printf("The file is already closed. \n");
        return -1;
    }
    // close and return on success
    fileStatusArray.FILE_STATUS[fd] = STATUS_CLOSED;
    if (fileStatusArray.FILE_TO_OPEN[fd].filetype == RTC_FILE_TYPE)
      fileStatusArray.RTC_STATUS = STATUS_CLOSED;
    return 0;
}

/*
 * file_find
 *   DESCRIPTION: finds the fd for a file, used to find the correct fd to pass into file_read or dir_read function
 *   INPUTS: filename -- the file we want to find
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure and non-negative index of the fd we finded
 *   SIDE EFFECTS: none
 */
int32_t file_find    (const uint8_t* filename){
    int32_t ii;    // traverse to check status file/dir array
    int32_t i;     // traverse to check name string
    const int32_t file_name_length = strlen((int8_t*) filename);
    int8_t already_open_flag = 0;
    if (file_name_length > FILE_NAME_LEN ){
        printf("Input string too long.\n");
        return -1;
    }
    // traverse the open file array
    for (ii = 0; ii < MAX_FILE_OPEN; ii++){
        // check each opened file
        if (fileStatusArray.FILE_STATUS[ii] == STATUS_OPENED){
            already_open_flag = 1; // initialize as opened
            // check if the name is the same
            for (i = 0; i < file_name_length; i++){
                if (filename[i] != fileStatusArray.FILE_TO_OPEN[ii].filename[i]){
                    already_open_flag = 0;
                    break;
                }
            }
            // if it is the same return the fd index
            if (already_open_flag == 1)
                return ii;
        }
    }
    // return -1 if not found
    return -1;
}

/*
 * file_read
 *   DESCRIPTION: reads nbytes of informatin from the non-directory file into the buffer, redirects to rtc_read if the input file is rtx file
 *   INPUTS: fd -- the file index we want to read
             buf -- the buffer to copy information into
             nbytes -- number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure and non-negative actual number of bytes we read
 *   SIDE EFFECTS: none
 */
int32_t file_read    (int32_t fd, void* buf, int32_t nbytes){
    int32_t file_inode_num;
    int32_t file_length;
    uint32_t new_offset;

    // check input fd valid
    if (fd < 0 || fd >= MAX_FILE_OPEN){
        printf("Input fd is invalid.\n");
        return -1;
    }
    // check if the file is opened
    if (fileStatusArray.FILE_STATUS[fd] == STATUS_CLOSED){
        printf("The file is not opened.\n");
        return -1;
    }
    // check file type
    if (fileStatusArray.FILE_TO_OPEN[fd].filetype == DIR_FILE_TYPE){
        *((int8_t*) buf) = '\0';
        return -1;
    }

    if (fileStatusArray.FILE_TO_OPEN[fd].filetype == RTC_FILE_TYPE){
        return rtc_read(fd, buf, nbytes);
    }

    // check input buf pointer is valid
    if (buf == NULL) {
      printf("Input buf is NULL.\n");
      return -1;
    }

    file_inode_num = fileStatusArray.FILE_TO_OPEN[fd].inode_num;
    file_length = ece391FileSystem.ece391_inodes[file_inode_num].length;
    new_offset = read_data(file_inode_num, fileStatusArray.FILE_OFFSET[fd],(uint8_t*) buf, nbytes);
    // if fail to read
    if (new_offset == -1){
        printf("File read fail.\n");
        return -1;
    }
    // update the offset array in the status array
    fileStatusArray.FILE_OFFSET[fd] += new_offset;
    return new_offset;
}

/*
 * file_write
 *   DESCRIPTION: read-only filesystem, nothing to do
 *   INPUTS: fd -- the file index we want to write
             buf -- the buffer to write from
             nbytes -- number fo bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t file_write   (int32_t fd, const void* buf, int32_t nbytes){
    /*check valid fd*/
    if (fd < 0 || fd > MAX_FILE_OPEN) {
      printf("ece391_WARNING::fd out of range.\n");
    }
    // redirect to rtc_write if the fd is rtc type
    else if (fileStatusArray.FILE_TO_OPEN[fd].filetype == RTC_FILE_TYPE) {
      return rtc_write(fd, buf, nbytes);
    }

    return -1;
}


/*need header*/
/*
 * dir_open
 *   DESCRIPTION: opens a directory file
 *   INPUTS: filename -- the file we want to open
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success and -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t dir_open    (const uint8_t* filename){
  int32_t ii;    // traverse to check status file/dir array
  int32_t new_fd = MAX_FILE_OPEN;  // if can open a file, this will record the fd
  int8_t already_open_flag = 0;

  //check for bad pointer
  if (filename == NULL) return -1;
  //check for nonexisting directory
  if (*filename != '.') return -1;
  // traverse the open file array
  for (ii = 0; ii < MAX_FILE_OPEN; ii++){
      // check each opened file
      if (fileStatusArray.FILE_STATUS[ii] == STATUS_OPENED){
          already_open_flag = 1; // initialize as opened
          // check if the name is the same
          if (*filename != *(fileStatusArray.FILE_TO_OPEN[ii].filename)){
              already_open_flag = 0;
          }
          // if it is the same, handle the error and return on failure
          if (already_open_flag == 1){
              printf("The file is already opened.\n");
              return -1;
          }
      }
      // try to update the new_fd for later use
      else if (ii < new_fd)
          new_fd = ii;

  }
  // if the file status array is full
  if (new_fd == MAX_FILE_OPEN){
      printf("The file processing array is full.\n");
      return -1;
  }
  // now is OK to really try to look for the file in the ece391FILE_SYSTEM
  if(read_dentry_by_name(filename, (fileStatusArray.FILE_TO_OPEN) + new_fd) == -1){  /* trying to copy dentry if exist*/
      printf("The file does not exist.\n");
      return -1;
  }
  // update the opened file's status
  fileStatusArray.FILE_STATUS[new_fd] = STATUS_OPENED;
  fileStatusArray.FILE_OFFSET[new_fd] = 0;
  fileStatusArray.CURRENT_DIR_IDX = 0;
  return 0;
}

/*
 * dir_close
 *   DESCRIPTION: closes a directory file
 *   INPUTS: fd -- the file index in the fileStatusArray of the file we want to close
 *   OUTPUTS: none
 *   RETURN VALUE: 0 on success and -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t dir_close   (int32_t fd){
  // check fd valid
  if (fd >= MAX_FILE_OPEN || fd < 0){
      printf("The input index is out of boundary. \n");
      return -1;
  }
  // check if status is already closed
  if (fileStatusArray.FILE_STATUS[fd] == STATUS_CLOSED){
      printf("The directory is already closed. \n");
      return -1;
  }
  // close and return on success
  fileStatusArray.FILE_STATUS[fd] = STATUS_CLOSED;
  fileStatusArray.CURRENT_DIR_IDX = 0;
  return 0;
}

/*
 * dir_read
 *   DESCRIPTION: reads nbytes of informatin from the directory file into the buffer
 *   INPUTS: fd -- the file index we want to read
             buf -- the buffer to copy information into
             nbytes -- number of bytes to read
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure and 0 on success
 *   SIDE EFFECTS: none
 */
int32_t dir_read    (int32_t fd, void* buf, int32_t nbytes){
    /*index to copy to buffer*/
    int32_t i;
    /*size of a particular file*/
    int32_t filesize;
    /*stores the current dentry*/
    dentry_t temp_dentry;
    /*check for bad pointer*/
    if (buf == NULL) {
      printf("Input buf pointer is NULL.\n");
      return -1;
    }

    /*check whether reading 32 bytes*/
    if (nbytes != FILE_NAME_LEN) {
      printf("Bytes to read is not 32.\n");
      return -1;
    }

    /*read the next file*/
    if (fileStatusArray.CURRENT_DIR_IDX >= ece391FileSystem.dir_count) {
      return -1;
    }

    /*read the current file and copy its dentry information*/
    if (read_dentry_by_index(fileStatusArray.CURRENT_DIR_IDX, &temp_dentry) == -1) {
      return -1;
    }

    (fileStatusArray.CURRENT_DIR_IDX)++;

    /*copy filename into the buffer*/
    for (i = 0; i < FILE_NAME_LEN; i++) {
      ((int8_t*)buf)[i] = temp_dentry.filename[i];
    }

    if (temp_dentry.filetype != 2) filesize = 0;
    else filesize = ece391FileSystem.ece391_inodes[temp_dentry.inode_num].length;

    /*prints out the filename, filetype, and filesize of the current file in the directory*/
    printf("Filename: ");
    for (i = 0; i < FILE_NAME_LEN; i++) {
      if (temp_dentry.filename[i] == '\0') break;
      printf("%c", temp_dentry.filename[i]);
    }
    printf(", Filetype: %d, Filesize: %dbytes.\n", temp_dentry.filetype, filesize);
    return 0;
}

/*
 * dir_write
 *   DESCRIPTION: read-only filesystem, nothing to do
 *   INPUTS: fd -- the file index we want to write
             buf -- the buffer to write from
             nbytes -- number fo bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: -1 on failure
 *   SIDE EFFECTS: none
 */
int32_t dir_write   (int32_t fd, const void* buf, int32_t nbytes){
    return -1;
}
