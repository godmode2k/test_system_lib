/* --------------------------------------------------------------
Project:	stat_file
Purpose:	Gets file info involved device(major, minor) and partition name
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since Dec 19, 2014
Filename: 	stat_file.c

Last modified: Dec 19, 2014
License: n/a
-----------------------------------------------------------------
Note:
-----------------------------------------------------------------
	1. Build:
		$ gcc -o stat_file stat_file.c -Wall -O2
	2. Execute:
		$ ./stat_file filename
	

	$ stat filename				// device (major, minor)
	$ cat /proc/partitions		// major, minor, name
	$ df filename
-------------------------------------------------------------- */



//! Header
// ---------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>		// stat
#include <sys/types.h>		// major, minor
// ---------------------------------------------------------------

//! Definition
// ---------------------------------------------------------------
#define PROC_PARTITION		"/proc/partitions"
// ---------------------------------------------------------------

//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------

//! Main
// ---------------------------------------------------------------
int main(int argc, char* argv[]) {
	char buf[255] = {0,};
	FILE* fp = NULL;
	struct stat st;


	if ( argc < 2 ) {
		fprintf( stdout, "%s: filename\n", argv[0] );
		return -1;
	}

	if ( stat(argv[1], &st) != 0 )
		return -1;

	{
		const unsigned long st_dev = st.st_dev;

		fprintf( stdout, "file = %s\n", argv[1] );
		fprintf( stdout, "inode = %lu\n", st.st_ino );
		fprintf( stdout, "device = %#x(h)/%ld(d)\n",
				st_dev, st_dev );
		fprintf( stdout, "device major = %d, minor = %d\n",
				major(st.st_dev), minor(st.st_dev) );
	}


	fp = fopen( PROC_PARTITION, "r" );
	if ( fp != NULL ) {
		fprintf( stdout, "%s:\n", PROC_PARTITION );

		do {
			if ( fgets(buf, sizeof(buf), fp) != NULL ) {
				int ret_scan = 0;
				int major, minor, block;
				char buf_name[255] = {0,};

				/*
				ret_scan = sscanf( buf, "%d %d %d %s",
						&major, &minor, &block, buf_name ); 

				if ( ret_scan < 4 ) continue;

				fprintf( stdout, "major: %d, minor: %d, block: %d, name: %s\n",
						major, minor, block, buf_name );
				*/

				ret_scan = sscanf( buf, "%d %d %*d %s",
						&major, &minor, buf_name ); 

				if ( ret_scan < 3 ) continue;

				if ( (major(st.st_dev) != major) ||
					 (minor(st.st_dev) != minor) )
					continue;

				fprintf( stdout, "    major: %d, minor: %d, name: %s\n",
						major, minor, buf_name );
			}
		} while ( !feof(fp) );

		if ( fp != NULL ) {
			fclose( fp );
			fp = NULL;
		}
	}
	else {
		fprintf( stdout, "Open a file: %s [FAIL]", PROC_PARTITION );
		return -1;
	}

	return 0;
}
// ---------------------------------------------------------------

