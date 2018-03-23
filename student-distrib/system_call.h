#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include "types.h"

#define FA_SIZE                 8

typedef int32_t (*oFunc)(const uint8_t *filename);
typedef int32_t (*cFunc)(int32_t fd);
typedef int32_t (*rFunc)(int32_t fd, void *buf, int32_t nbytes);
typedef int32_t (*wFunc)(int32_t fd, const void *buf, int32_t nbytes);

typedef struct fileOperationTable {
    oFunc fotOpen;
    cFunc fotClose;
    rFunc fotRead;
    wFunc fotWrite;
} fot;

typedef struct fileDescriptor {
    fot *table;
    uint32_t *inode;
    uint32_t filePos;
    uint32_t flags;
} fd;

typedef struct fileArray {
    fd files[FA_SIZE];
    uint32_t fullFlag;
    uint32_t *inode;
} fa;

int32_t open(const uint8_t *filename);

int32_t close(int32_t fd);

int32_t read(int32_t fd, void *buf, int32_t nbytes);

int32_t write(int32_t fd, const void *buf, int32_t nbytes);

void init_fot(void);

#endif
