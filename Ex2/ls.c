#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

void do_ls();
void dostat();
void show_file_info();
void mode_to_letters();
char * uid_to_name();
char * gid_to_name();
void do_ls_r(const char* dirname, int depth);

int main(int argc, char *argv[])
{
	int nArgc;
	bool bRecursive;
	//just want to show current folder
	if(argc == 1) {
		do_ls(".");
		return 0;
	}

	nArgc = argc;
	while(--nArgc) {
		//options
		if(argv[nArgc][0] == '-') {
			if(argv[nArgc][1] == 'R') {
				bRecursive = true;
			}else {
				fprintf(stderr, "No other options is available except -R\n");
				return 0;
			}
		}
	}

	if(argc == 2 && bRecursive) {
		do_ls_r(".", 0);
	}

	nArgc = argc;
	while(--nArgc) {
		if(argv[nArgc][0] != '-') {
			do_ls_r(argv[nArgc], 0);
		}
	}

    return 0;
}

void do_ls(const char* dirname)
{
    DIR* dir_ptr;
    struct dirent* dir_entry_ptr;

    if ((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr, "Failed to open dir %s\n", dirname);
    else {
    	int count = 0;
        while ((dir_entry_ptr = readdir(dir_ptr)) != NULL) {
        	printf("%-20s", dir_entry_ptr->d_name);
        	if(++count%4 == 0) {
        		printf("\n");
        	}
        }
	printf("\n");
        closedir(dir_ptr);
    }
}

void do_ls_r(const char* dirname, int depth) {
	DIR * dir_ptr;
	struct dirent* dir_entry_ptr;

	if (NULL == (dir_ptr = opendir(dirname))) {
		fprintf(stderr, "invalid directory or file %s\n", dirname);
		return;
	}
	if (NULL ==(dir_entry_ptr = readdir(dir_ptr)))
		return;

	do {
		if (dir_entry_ptr->d_type == DT_DIR) {
			char path[1024];
			int len = snprintf(path, sizeof(path)-1, "%s/%s", dirname, dir_entry_ptr->d_name);
			path[len] = 0;
			if (strcmp(dir_entry_ptr->d_name, ".") == 0 || strcmp(dir_entry_ptr->d_name, "..") == 0)
				continue;
			printf("%*s[%s]\n", depth*2, "", dir_entry_ptr->d_name);
			//stdout;
			fflush(stdout);
			do_ls_r(path, depth + 1);
	        }
	        else
	            printf("%*s- %s\n", depth*2, "", dir_entry_ptr->d_name);
	    } while (dir_entry_ptr = readdir(dir_ptr));
	    closedir(dir_ptr);
}

void dostat(char *filename)
{
    struct stat info;
    if (stat(filename, &info) == -1)
        perror(filename);
    else
        show_file_info(filename, &info);
}

void show_file_info(char* filename, struct stat * info_p)
{
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];
    mode_to_letters(info_p->st_mode, modestr);

    printf("%s", modestr);
    printf("%4d", (int)info_p->st_nlink);
    printf("%-8s", uid_to_name(info_p->st_uid));
    printf("%-8s", gid_to_name(info_p->st_gid));
    printf("%8ld", (long)info_p->st_size);
    printf("%.12s", 4+ctime(&info_p->st_mtime));
    printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
    strcpy(str, "---------");
    if (S_ISDIR(mode)) str[0] = 'd';
    if (S_ISCHR(mode)) str[0] = 'c';
    if (S_ISBLK(mode)) str[0] = 'b';

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

#include <pwd.h>

char *uid_to_name(uid_t uid)
{
    struct passwd *getpwuid(), * pw_ptr;
    static char numstr[10];

    if ((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}

#include <grp.h>

char *gid_to_name(gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];

    if ((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    } else
        return grp_ptr->gr_name;
}

