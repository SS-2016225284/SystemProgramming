#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main (int argc, char**argv) {
    struct stat srcStat;
    struct stat dstStat;
    
    printf("%d, %d\n", sizeof(argv[1]) / sizeof(argv[1][0]), sizeof(argv[2]) /sizeof(argv[2][0]));
    if(argc != 3) {
        fprintf(stderr, "usage : 2016225284_PE4 SourceFile DestinationFile\n");
        exit(0);
    }

    if(stat(argv[1],&srcStat) == -1 ||!S_ISDIR(srcStat.st_mode)) {
        fprintf(stderr, "SourceFile should be a file\n");
    }

    //no file exists
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
