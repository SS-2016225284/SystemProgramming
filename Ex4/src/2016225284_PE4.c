#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define BUFF_SIZE 512

//returns last / position
int lastSlash(char * str){
	int len = -1;

	len = strlen(str);
	for(;len > -1; --len){
		if(str[len] == '/')
			break;
	}
	return len;
}

int main (int argc, char**argv) {
    struct stat srcStat;
    struct stat dstStat;
    struct stat tempStat;
    char strCurrentDir[BUFF_SIZE]; //Buffer to contain current working directory
    char strTempDir[BUFF_SIZE]; //Buffer to contain temporary workign director
    char strSrcPath[BUFF_SIZE];
    char strDstPath[BUFF_SIZE];
    ino_t srcParentInode = 0;
    ino_t dstParentInode = 0;

    if(argc != 3) {
        fprintf(stderr, "usage : 2016225284_PE4 SourceFile DestinationFile\n");
        exit(0);
    }else {
        getcwd(strCurrentDir, BUFF_SIZE);
    }

    //current?
    if(lastSlash(argv[1]) == -1) {
    	int len = strlen(strCurrentDir);
    	strncpy(strSrcPath, strCurrentDir, len);
    	strSrcPath[len] = '/';
    	strSrcPath[len+1] = '\0';
    	strncat(strSrcPath, argv[1], len-1);
    } else {
    	strncpy(strSrcPath, argv[1], strlen(argv[1]));
    	strSrcPath[strlen(argv[1])] = '\0';
    }

    if(lastSlash(argv[2]) == -1) {
    	int len = strlen(strCurrentDir);
    	strncpy(strDstPath, strCurrentDir, len);
    	strDstPath[len] = '/';
    	strDstPath[len+1] = '\0';
    	strncat(strDstPath, argv[2], len-1);
    }else {
    	strncpy(strDstPath, argv[2], strlen(argv[2]));
    	strDstPath[strlen(argv[2])] = '\0';
    }
#ifdef _DEBUG
    printf("%s\n%s\n", strSrcPath, strDstPath);
#endif

    //source file should exist!
    if(stat(strSrcPath,&srcStat) == -1 ||!S_ISREG(srcStat.st_mode)) {
        fprintf(stderr, "SourceFile should be a existing file\n");
    }else {
    	int s = lastSlash(strSrcPath);
    	strncpy(strTempDir, strSrcPath, s);
    	strTempDir[s] = '\0';
    	if(stat(strTempDir, &tempStat) != -1) {
    		srcParentInode = tempStat.st_ino;
    	}
    }

    //destination file or folder?
    if(stat(strDstPath, &dstStat) != -1) {
    	if(S_ISDIR(dstStat.st_mode)) {
    		dstParentInode = dstStat.st_ino;
    	}else if(S_ISREG(dstStat.st_mode)) {
    		int s = lastSlash(strDstPath);
    		strncpy(strTempDir, strDstPath, s);
    		strTempDir[s] = '\0';
    		if(stat(strTempDir, &tempStat) != -1) {
    			dstParentInode = tempStat.st_ino;
    		}
    	}
    }else {
    	int s = lastSlash(strDstPath);
		strncpy(strTempDir, strDstPath, s);
		strTempDir[s] = '\0';
		if(stat(strTempDir, &tempStat) != -1) {
			dstParentInode = tempStat.st_ino;
		}else {
			fprintf(stderr, "No destination directory or file\n");
		}
    }

    if(srcParentInode == dstParentInode) {
    	rename(strSrcPath, strDstPath);
    }else {
    	int srcFd, dstFd, len;
    	srcFd = open(strSrcPath, O_RDONLY);
    	dstFd = open(strDstPath, O_RDWR|O_CREAT|O_TRUNC, 0644);

#ifdef _DEBUG
    	printf("%d, %d\n", srcFd, dstFd);
#endif

    	//Here, i will use strTempDir as Buffer
    	while((len = read(srcFd, strTempDir, BUFF_SIZE))) {
    		write(dstFd, strTempDir, len);
    	}
    	close(srcFd);
    	close(dstFd);
    	remove(strSrcPath);
    }

    //int rename(const char *old, const char * new)
    //open, close read, write
    //getcwd(char *buf, size_t size), which gets current directory.
    //chdir(char*)
    return 0;
}
