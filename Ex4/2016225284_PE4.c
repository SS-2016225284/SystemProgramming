#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main (int argc, char**argv) {
    struct stat srcStat;
    struct stat dstStat;

    if(argc != 3) {
        fprintf(stderr, "usage : 2016225284_PE4 SourceFile DestinationFile\n");
        exit(0);
    }

    if(stat(argv[1],&srcStat) == -1 ||!S_ISDIR(srcStat.st_mode)) {
        fprintf(stderr, "SourceFile should be a file\n");
    }

    if(stat(argc[2], &dstStat) == -1) {
        //? 
    }else {
        fprintf(stderr, "No such directory %s\n", argc[2]);
    }

    //int rename(const char *old, const char * new)
    //open, close read, write
    return 0;
}
