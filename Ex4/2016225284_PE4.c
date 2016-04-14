#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 512 

int main (int argc, char**argv) {
    struct stat srcStat;
    struct stat dstStat;
    struct stat tempStat;
    char strCurrentDir[BUFF_SIZE]; //Buffer to contain current working directory
    char strTempDir[BUFF_SIZE]; //Buffer to contain temporary workign director
    ino_t srcParentInode = 0;
    ino_t dstParentInode = 0;

    if(argc != 3) {
        fprintf(stderr, "usage : 2016225284_PE4 SourceFile DestinationFile\n");
        exit(0);
    }else {
        getcwd(strCurrentDir, BUFF_SIZE);
    }

    //source file should exist!
    if(stat(argv[1],&srcStat) == -1 ||!S_ISDIR(srcStat.st_mode)) {
        fprintf(stderr, "SourceFile should be a existing file\n");
    }else {
        //source file exists, find parent inode
        chdir("..");
        getcwd(strTempDir, BUFF_SIZE);
        stat(strTempDir, &tempStat);
        srcParentInode = tempStat.st_ino;                    
    }

    //destination file or folder?
    if(stat(argv[2], &dstStat) == -1) {
        //is it directory?
        //or is it new file destination?
    }else {
        fprintf(stderr, "No such directory %s\n", argv[2]);
    }

    //int rename(const char *old, const char * new)
    //open, close read, write
    //getcwd(char *buf, size_t size), which gets current directory.
    //chdir(char*)
    return 0;
}
