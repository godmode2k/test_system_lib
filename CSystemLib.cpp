/* --------------------------------------------------------------
Project:	
Purpose:	Test System Lib
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since March 17, 2014
Filename:	CSystemLib.cpp

Last modified: March 18, 2014
License: read a file "LICENSE.txt"
-----------------------------------------------------------------
Note:
-----------------------------------------------------------------
-------------------------------------------------------------- */



//! Header and Definition
// ---------------------------------------------------------------
#include "CSystemLib.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! TEST
// ---------------------------------------------------------------
int main(void) {
	//{fprintf( stdout, "main()\n" );}

	int mode = 0;
	int uid = -1, gid = -1;
	const char* path = "./test";
	const char* path_d = "./test_d";
	const char* username = "test";


	// Permission
	{
		mode = _CAPI::dec_to_oct( 700  );
		_CAPI::set_chmod( path, mode );

		if ( _CAPI::get_uid_gid(username, &uid, &gid) )
			_CAPI::set_chown( path, uid, gid );
	}

	fprintf( stdout, "main(): ------------------\n" );

	// File/Directory operation
	{
		_CAPI::set_mkdir( path_d, mode );

		//_CAPI::set_rmdir( path_d );
	}

	fprintf( stdout, "main(): ------------------\n" );

	// fork with execute the cmdline
	{
		CSystemLib* clib = new CSystemLib;
		if ( clib ) {
			{
				const char* argv[] = { "/bin/ls", "-al", NULL };
				//const char* argv[] = { "/usr/sbin/repquota", "-a", NULL };
				const int argc = ( sizeof(argv) / sizeof(argv[0]) );
				int stdout_len, stderr_len;

				clib->set_pipe( argc, (char**)argv, &stdout_len, &stderr_len, NULL, NULL );

				fprintf( stdout, "main(): argc = %d, stdout_len = %d, stderr_len = %d\n",
						argc, stdout_len, stderr_len );
			}

			fprintf( stdout, "main(): ------------------\n" );

			{
				const char* username = "test";
				int size_mb = 0;
				const char* device = "/dev/sda5";

				//clib->set_quota( username, size_mb, device );
			}

			delete clib;
			clib = NULL;
		}
	}


	return 0;
}
// ---------------------------------------------------------------


//! Class
// ---------------------------------------------------------------
CSystemLib::CSystemLib(void) : TAG(_TAG_) {
	{fprintf( stdout, "CSystemLib::CSystemLib()\n" );}
}

CSystemLib::~CSystemLib() {
	{fprintf( stdout, "CSystemLib::~CSystemLib()\n" );}
}

int CSystemLib::set_fork(const fork_type_t type) {
	{fprintf( stdout, "CSystemLib::set_fork()\n" );}
	return _CAPI::set_fork( type );
}

int CSystemLib::get_fork(void) {
	{fprintf( stdout, "CSystemLib::get_fork()\n" );}
	return _CAPI::get_fork();
}

int CSystemLib::dec_to_oct(int dec) {
	{fprintf( stdout, "CSystemLib::dec_to_oct()\n" );}
	return _CAPI::dec_to_oct( dec );
}

int CSystemLib::get_uid_gid(const char* username, int* _uid, int* _gid) {
	{fprintf( stdout, "CSystemLib::get_uid_gid()\n" );}
	return _CAPI::get_uid_gid( username, _uid, _gid );
}

int CSystemLib::set_chmod(const char* path, int mode) {
	{fprintf( stdout, "CSystemLib::set_chmod()\n" );}
	return _CAPI::set_chmod( path, mode );
}

int CSystemLib::set_chown(const char* path, int uid, int gid) {
	{fprintf( stdout, "CSystemLib::set_chown()\n" );}
	return _CAPI::set_chown( path, uid, gid );
}

int CSystemLib::set_mkdir(const char* path, int mode) {
	{fprintf( stdout, "CSystemLib::set_mkdir()\n" );}
	return _CAPI::set_mkdir( path, mode );
}

int CSystemLib::set_rmdir(const char* path) {
	{fprintf( stdout, "CSystemLib::set_rmdir()\n" );}
	return _CAPI::set_rmdir( path );
}

int CSystemLib::set_unlink(const char* path) {
	{fprintf( stdout, "CSystemLib::set_unlink()\n" );}
	return _CAPI::set_unlink( path );
}

int CSystemLib::set_quota(const char* username, int size_mb, const char* device) {
	{fprintf( stdout, "CSystemLib::set_quota()\n" );}
	return _CAPI::set_quota( username, size_mb, device );
}

int CSystemLib::set_pipe(int argc, char** argv, int* _stdout_len, int* _stderr_len,
	char** _stdout, char** _stderr) {
	{fprintf( stdout, "CSystemLib::set_pipe()\n" );}
	return _CAPI::set_pipe( argc, argv, _stdout_len, _stderr_len, _stdout, _stderr );
}

// ---------------------------------------------------------------


//! IMPLEMENTATION
// ---------------------------------------------------------------
namespace _CAPI {
int set_fork(const fork_type_t type) {
	//{fprintf( stdout, "set_fork()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;


	fork_type = type;
	switch ( fork_type ) {
		case FORK_TYPE_NORMAL:
			{
				fprintf( stdout, "get_fork(): type = FORK_TYPE_NORMAL (%d)\n", FORK_TYPE_NORMAL );
				ret = 0;
			} break;
	} // switch()


	return ret;
}

int get_fork(void) {
	//{fprintf( stdout, "get_fork()\n" );}
	
	switch ( fork_type ) {
		case FORK_TYPE_NORMAL:
			{
				fprintf( stdout, "get_fork(): type = FORK_TYPE_NORMAL (%d)\n", FORK_TYPE_NORMAL );
			} break;
	}


	return fork_type;
}

int dec_to_oct(int dec) {
	//{fprintf( stdout, "dec_to_oct()\n" );}

	char buf_digit[MAX_PERM_SIZE+1] = { 0, };

	const int base = 8;
	long val;

	// integer to octal value
	snprintf( buf_digit, MAX_PERM_SIZE, "%d", dec );
	val = strtol( buf_digit, NULL, base );

	printf( "dec_to_oct(): dec = %d, oct = %d(d), %ld(ld)\n", dec, val, val );


	/*
	int base = 8;
	char* endptr;
	char* str;
	long val;

	errno = 0;    // To distinguish success/failure after call
	val = strtol( str, &endptr, base );

	// Check for various possible errors
	if ( (errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0) ) {
		perror( "strtol" );
		exit( EXIT_FAILURE );
	}

	if ( endptr == str ) {
		fprintf( stderr, "No digits were found\n" );
		exit( EXIT_FAILURE);
	}

	printf( "dec = %d, oct = %d(d),  %ld(ld)\n", dec, val, val );
	*/



	return (int)val;
}

int get_uid_gid(const char* username, int* _uid, int* _gid) {
	//{fprintf( stdout, "get_uid_gid()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;
	struct passwd* pw;
   
	if ( !username ) {
		fprintf( stderr, "get_uid_gid(): username == NULL [FALSE]\n" );
		return -1;
	}

	pw = getpwnam( username );
	if ( !pw ) {
		fprintf( stderr, "get_uid_gid(): '%s' get uid/gid [FALSE]\n", username );
		return -1;
	}

	*_uid = pw->pw_uid;
	*_gid = pw->pw_gid;
	//pw->pw_dir;	// Home directory

	fprintf( stdout, "get_uid_gid(): username = %s, " \
			"uid = %d, gid = %d\n", username, pw->pw_uid, pw->pw_gid );


	return ret;
}

int set_chmod(const char* path, int mode) {
	{fprintf( stdout, "set_chmod()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;

	// 0 if success, -1 otherwise
	ret = chmod( path, mode );
	if ( ret != 0 ) {
		fprintf( stderr, "set_chmod(): '%s' [FALSE]\n", path );
	}
	else {
		fprintf( stdout, "set_chmod(): '%s' [TRUE]\n", path );
	}


	return ret;
}

int set_chown(const char* path, int uid, int gid) {
	//{fprintf( stdout, "set_chown()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;

	// 0 if success, -1 otherwise
	ret = chown( path, uid, gid );
	if ( ret != 0 ) {
		fprintf( stderr, "set_chown(): '%s' [FALSE]\n", path );
	}
	else {
		fprintf( stdout, "set_chown(): '%s' [TRUE]\n", path );
	}


	return ret;
}

int set_mkdir(const char* path, int mode) {
	//{fprintf( stdout, "set_mkdir()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;

	/*
	struct stat st;
	if ( stat(path, &st) != 0 ) {
		// Directory
		if ( (st.st_mode & S_IFMT) == S_IFDIR ) {
		}
	}
	else {
	}
	*/

	if ( access(path, F_OK) != -1 ) {
		// Exist and Accessable
		fprintf( stderr, "set_mkdir(): '%s' exist and accessable\n", path );
	}
	else {
		// 0 if success, -1 otherwise
		ret = mkdir( path, mode );
	}

	if ( ret != 0 ) {
		fprintf( stderr, "set_mkdir(): '%s' [FALSE]\n", path );
	}
	else {
		fprintf( stdout, "set_mkdir(): '%s' [TRUE]\n", path );
	}

	
	return ret;
}

int set_rmdir(const char* path) {
	//{fprintf( stdout, "set_rmdir()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;


	return -1;



	// Delete files
	while ( 1 ) {
		int _ret = -1;
		char* _path;

		_path = NULL;

		// Get directory entry
		// ...

		if ( _path ) {
			struct stat st;
			if ( stat(_path, &st) != 0 ) {
				// Directory
				if ( (st.st_mode & S_IFMT) == S_IFDIR ) {
					_ret = set_rmdir( _path );
					_ret = rmdir( _path );
				}
				// Regular file
				//else if ( (st.st_mode & S_IFMT) == S_IFREG ) {
				//}
				else {
					_ret = set_unlink( _path );
				}
			}
		}
	}

	// 0 if success, -1 otherwise
	//ret = rmdir( path );

	if ( ret != 0 ) {
		fprintf( stderr, "set_rmdir(): '%s' [FALSE]\n", path );
	}
	else {
		fprintf( stdout, "set_rmdir(): '%s' [TRUE]\n", path );
	}


	return ret;
}

int set_unlink(const char* path) {
	{fprintf( stdout, "set_unlink()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;

	ret = unlink( path );
	if ( ret != 0 ) {
		fprintf( stderr, "set_unlink(): '%s' [FALSE]\n", path );
	}
	else {
		fprintf( stdout, "set_unlink(): '%s' [TRUE]\n", path );
	}


	return ret;
}

int set_quota(const char* username, int size_mb, const char* device) {
	{fprintf( stdout, "set_quota()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;

	const char* block_soft = "0";
	char block_hard[8+1] = {0,};
	const char* inode_soft = "0";
	const char* inode_hard = "0";

	snprintf( block_hard, sizeof(block_hard), "%d", (size_mb * SIZE_UNIT_KB) );

	fprintf( stdout, "set_quota(): username = %s, " \
			"size = %d(MB), %s(KB), device = %s\n",
			username, size_mb, block_hard, device );

	{
		const char* argv[] = {
			"/usr/sbin/setquota",
			"-u",
			username,
			block_soft,
			block_hard,
			inode_soft,
			inode_hard,
			device,
			NULL
		};
		const int argc = ( sizeof(argv) / sizeof(argv[0]) );
		int stdout_len, stderr_len;

		fprintf( stdout, "set_quota(): CMDLINE = %s %s %s %s %s %s %s %s\n",
				argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7] );

		set_pipe( argc, (char**)argv, &stdout_len, &stderr_len, NULL, NULL );

		fprintf( stdout, "set_quota(): argc = %d, stdout_len = %d, stderr_len = %d\n",
				argc, stdout_len, stderr_len );
	}


	return ret;
}


int set_pipe(int argc, char** argv, int* _stdout_len, int* _stderr_len,
	char** _stdout, char** _stderr) {
	{fprintf( stdout, "set_pipe()\n" );}

	// 0 if success, -1 otherwise
	int ret = -1;

	int pid;
	int pipefd[2];			// [0]: read, [1]: write
	int pipefd_stderr[2];	// [0]: read, [1]: write


	if ( (argc <= 0) || !argv ) {
		fprintf( stderr, "set_pipe(): argc = %d, argv = %s\n",
				argc, ((argv)? "!NULL" : "NULL") );

		*_stdout_len = 0;
		*_stderr_len = 0;

		return -1;
	}

	if ( pipe(pipefd) == -1 ) {
		fprintf( stderr, "set_pipe(): Create a pipe [FALSE]\n" );

		*_stdout_len = 0;
		*_stderr_len = 0;

		return -1;
	}

	if ( pipe(pipefd_stderr) == -1 ) {
		fprintf( stderr, "set_pipe(): Create a pipe(stderr) [FALSE]\n" );

		*_stdout_len = 0;
		*_stderr_len = 0;

		return -1;
	}

	// Check the file exists and grants read, write
	// and execute permissions respectively
	if ( access(argv[0], X_OK) < 0 ) {
		fprintf( stderr, "set_pipe(): Checks permissions for a file '%s' [FALSE]\n",
				argv[0] );
		*_stdout_len = 0;
		*_stderr_len = 0;

		return -1;
	}


	pid = fork();
	if ( pid == 0 ) {
		// Child
		{fprintf( stdout, "set_pipe(): Child process: pid = %d\n", getpid() );}
		
		// execv()
		//char* argv[] = { "/bin/ls", "-al", NULL };
		// execl()
		//char* argv[] = { "/bin/ls", "ls", "-al" };

		// close read descriptor
		close( pipefd[0] );
		close( pipefd_stderr[0] );

		// copy the stdout, stderr of the process to the pipe
		dup2( pipefd[1], 1 );
		dup2( pipefd_stderr[1], 2 );

		// close write descriptor
		close( pipefd[1] );
		close( pipefd_stderr[1] );

		// execute a command
		execv( argv[0], argv );
		//execl( argv[0], argv[1], argv[2], NULL );
	}
	else if ( pid < 0 ) {
		fprintf( stdout, "set_pipe(): fork() [FALSE]\n" );

		*_stdout_len = 0;
		*_stderr_len = 0;

		return -1;
	}
	else {
		// Parent
		{fprintf( stdout, "set_pipe(): Parent process: pid = %d\n", getpid() );}

		char buf[1024] = {0,};
		int bytes = 0;
		int total_bytes = 0;

		//int status;
		//waitpid( pid, &status, WUNTRACED );

		// close write descriptor
		close( pipefd[1] );
		close( pipefd_stderr[1] );

		// stdout
		while ( (bytes = read(pipefd[0], buf, sizeof(buf))) != 0 ) {
			fprintf( stdout, "set_pipe(): stdout-> %s\n", buf );

			total_bytes += bytes;

			if ( _stdout ) {
				//! TODO:
			}
		}

		if ( _stdout_len )
			*_stdout_len = total_bytes;


		memset( (void*)buf, 0x00, sizeof(buf) );
		bytes = 0;
		total_bytes = 0;


		// stderr
		while ( (bytes = read(pipefd_stderr[0], buf, sizeof(buf))) != 0 ) {
			fprintf( stdout, "set_pipe(): stderr-> %s\n", buf );

			total_bytes += bytes;

			if ( _stderr ) {
				//! TODO:
			}
		}

		if ( _stderr_len )
			*_stderr_len = total_bytes;




		if ( _stdout_len ) {
			fprintf( stdout, "set_pipe(): stdout len = %d\n", *_stdout_len );
		}
		if ( _stderr_len ) {
			fprintf( stdout, "set_pipe(): stderr len = %d\n", *_stderr_len );
		}
	}


	return ret;
}
} // namespace _CAPI
// ---------------------------------------------------------------



// __EOF__
