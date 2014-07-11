#ifndef __CSYSTEMLIB_H__
#define __CSYSTEMLIB_H__

/* --------------------------------------------------------------
Project:	
Purpose:	Test System Lib
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since March 17, 2014
Filename:	CSystemLib.h

Last modified: March 18, 2014
License: read a file "LICENSE.txt"
-----------------------------------------------------------------
Note:
-----------------------------------------------------------------
-------------------------------------------------------------- */



//! Header and Definition
// ---------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>		// struct passwd

#include <cstring>
#include <cstdlib>
#include <limits.h>
#include <errno.h>		// variable 'error'



//! Definition
// ---------------------------------------------------------------
#define MAX_PERM_SIZE	5
#define SIZE_UNIT_KB	1000

#ifdef __cplusplus
extern "C" {
#endif
typedef enum _fork_type_t {
	FORK_TYPE_NORMAL = 0,
	FORK_TYPE_PIPE,
} fork_type_t;

int fork_type;
#ifdef __cplusplus
}
#endif



//! Prototype
// ---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
namespace _CAPI {
	int set_fork(const fork_type_t type = FORK_TYPE_NORMAL);
	int get_fork(void);

	int dec_to_oct(int dec);
	int get_uid_gid(const char* username, int* _uid, int* _gid);

	int set_chmod(const char* path, int mode);
	int set_chown(const char* path, int uid, int gid);
	int set_mkdir(const char* path, int mode);
	int set_rmdir(const char* path);
	int set_unlink(const char* path);
	int set_quota(const char* username, int size_mb, const char* device);
	int set_pipe(int argc, char** argv, int* _stdout_len = NULL, int* _stderr_len = NULL,
			char** _stdout = NULL, char** _stderr = NULL);
} // namespace _CAPI
#ifdef __cplusplus
}
#endif



//! Class
// ---------------------------------------------------------------
#define _TAG_		"CSystemLib"
class CSystemLib {
private:
	const char* TAG;
protected:
public:
	explicit CSystemLib(void);
	virtual ~CSystemLib();
public:
	int set_fork(const fork_type_t type = FORK_TYPE_NORMAL);
	int get_fork(void);

	int dec_to_oct(int dec);
	int get_uid_gid(const char* username, int* _uid, int* _gid);

	int set_chmod(const char* path, int mode);
	int set_chown(const char* path, int uid, int gid);
	int set_mkdir(const char* path, int mode);
	int set_rmdir(const char* path);
	int set_unlink(const char* path);
	int set_quota(const char* username, int size_mb, const char* device);
	int set_pipe(int argc, char** argv, int* _stdout_len = NULL, int* _stderr_len = NULL,
			char** _stdout = NULL, char** _stderr = NULL);
};



#endif // __TEST_PERM_H__
