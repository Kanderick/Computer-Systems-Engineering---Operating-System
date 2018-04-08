#include "loader.h"
#include "file_system.h"

/* NOTE Limitation of this loader is that it assumes that and executable is always smaller than 4MB, which is specified in the MP3.3 documentation. However, this loader can be more flexible with a more complexed paging initialization strategy. */

 /* load_user_image
  * Purpose Load an user executable image into proper memory location.
  * Inputs  filename User program file name.
  * Outputs Error message when necessary.
  * Return  Starting Address of the executable if valid,
  *         NULL if invalid or error occured.
  * Side Effects None
  */
uint32_t* load_user_image(const uint8_t* filename) {
    /* Check if file exist */
    int32_t fd = file_find(filename);
    if (fd < 0) {
        ERROR_MSG;
        printf("File not opened.\n");
        return NULL;
    }

    /* Get file length, check if it exceed maximum allowable image size */
    int32_t nbytes = get_file_length(filename);
    if (nbytes > LOAD_EXE_MAX_SIZE) {
        ERROR_MSG;
        printf("Executable image size exceed maximum allowance. Size %dB, %dB allowed.\n", nbytes, LOAD_EXE_MAX_SIZE);
        return NULL;
    } else if (nbytes == -1) {
        ERROR_MSG;
        printf("Failed to get file length.\n");
        return NULL;
    }

    /* Read file data into proper location */
    uint32_t* buffer = (uint32_t*)LOAD_EXE_START_ADDR;
    int32_t read_bytes = file_read(fd, buffer, nbytes);
    if (read_bytes == -1) {
        ERROR_MSG;
        printf("File read failed.\n");
        return NULL;
    } else if (read_bytes != nbytes) {
        WARNING_MSG;
        printf("File read size does not match file size. Read %dB/%dB in file.\n", read_bytes, nbytes);
        // WARNING Function not returned here. Change to ERROR_MSG and return if necessary.
    }

    /* Check if file is a valid executable */
    uint8_t* file_identifier = (uint8_t*)LOAD_EXE_START_ADDR;
    uint8_t i;
    for (i = 0; i < EXE_IDENTIFIER_SIZE; i++) {
        if (file_identifier[i] != EXE_IDENTIFIER_TABLE[i]) {
            ERROR_MSG;
            printf("File identifier %d doesn't match. %u instead of %u.\n", i, file_identifier[i], EXE_IDENTIFIER_TABLE[i]);
            printf("Not an executable image.\n");;
            return NULL;
        }
    }

    /* Get program starting address */
    uint32_t* starting_addr = (uint32_t*)buffer[EXE_START_COUNT];
    /* Check if we got an starting address after EXE_START_ADDR, before the end of the page */
    if (((uint32_t)starting_addr < LOAD_EXE_START_ADDR) || (uint32_t)starting_addr > LOAD_PAGE_END_ADDR) {
        ERROR_MSG;
        printf("Executable image starting address(0x%#x) is invalid. Should be within 0x%#x - 0x%#x.\n", (uint32_t)starting_addr, LOAD_EXE_START_ADDR, LOAD_PAGE_END_ADDR);
        return NULL;
    }

    return starting_addr;
}
