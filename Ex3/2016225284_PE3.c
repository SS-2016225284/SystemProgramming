#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<dirent.h>

ino_t	get_inode(char *);
void    printpathto(ino_t);
void    inum_to_name(ino_t , char *, int );

int main()
{
	//current inode
	ino_t current = get_inode(".");

	printpathto( current );	/* print path to here	*/
	putchar('\n');				/* then add newline	*/
	return 0;
}

void printpathto( ino_t this_inode )
/*
 *	prints path leading down to an object with this inode
 *	kindof recursive
 */
{
#if 1
	ino_t parent_inode = 0;
	char  name[128][BUFSIZ];
	int size = 0, i;

	//me
	parent_inode = this_inode;

	//if parent is not me
	while(parent_inode != (this_inode = get_inode(".."))) {
		//parent is me
		chdir("..");
		inum_to_name(parent_inode, name[size++], BUFSIZ);
		parent_inode = this_inode;
	}

	for(i = size-1; i >= 0; --i) {
		printf("/%s", name[i]);
	}
	putchar('\n');

#endif
#if 0
	//Here, original source code attatched.
	ino_t	my_inode ;
	char	its_name[BUFSIZ];

	if ( get_inode("..") != this_inode )
	{
		chdir( ".." );				/* up one dir	*/

		inum_to_name(this_inode,its_name,BUFSIZ);/* get its name*/

		my_inode = get_inode( "." );		/* print head	*/
		printpathto( my_inode );		/* recursively	*/
		printf("/%s", its_name );		/* now print	*/
							/* name of this	*/
	}
#endif
}

void inum_to_name(ino_t inode_to_find , char *namebuf, int buflen)
/*
 *	looks through current directory for a file with this inode
 *	number and copies its name into namebuf
 */
{
	DIR		*dir_ptr;		/* the directory */
	struct dirent	*direntp;		/* each entry	 */

	dir_ptr = opendir( "." );
	if ( dir_ptr == NULL ){
		perror( "." );
		exit(1);
	}

	/*
	 * search directory for a file with specified inum
	 */

	while ( ( direntp = readdir( dir_ptr ) ) != NULL )
		if ( direntp->d_ino == inode_to_find )
		{
			strncpy( namebuf, direntp->d_name, buflen);
			namebuf[buflen-1] = '\0';   /* just in case */
			closedir( dir_ptr );
			return;
		}
	fprintf(stderr, "error looking for inum %d\n", inode_to_find);
	exit(1);
}

ino_t get_inode( char *fname )
/*
 *	returns inode number of the file
 */
{
	struct stat info;

	if ( stat( fname , &info ) == -1 ){
		fprintf(stderr, "Cannot stat ");
		perror(fname);
		exit(1);
	}
	return info.st_ino;
}

