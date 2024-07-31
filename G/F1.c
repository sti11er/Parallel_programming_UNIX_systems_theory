#include <stdio.h>

#define INODES_COUNT (1 << 20)
#define BLOCKS_COUNT (1 << 13)

struct Inode {
    int size;
    long blocks[BLOCKS_COUNT];
};

struct FileSystem {
    struct Inode inodes[INODES_COUNT];
    int root_inode_index;
};

int main(void) {
    struct FileSystem fs;
    int inode_index = 10;
    struct Inode *inode = &fs.inodes[inode_index];
    inode->size = BLOCKS_COUNT * 2;
    inode->blocks[0] = 1038;
    inode->blocks[1] = 37465;

    return 0;
}