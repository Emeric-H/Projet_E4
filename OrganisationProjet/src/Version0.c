//Code created thanks to : https://maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/ 
//and thanks to : https://maastaar.net/fuse/linux/filesystem/c/2019/09/28/writing-less-simple-yet-stupid-filesystem-using-FUSE-in-C/ 

//to compile, use : 
//	gcc [name].c -o [name] -lfuse3




// Launch the program by typing:
//   ./[name] -f -s -d ./[mountPoint]
// 
// -f: instructs the FUSE program to stay in the "f"oreground rather than detaching and going to the background.
// -s: This option activates the "s"ingle-threaded mode of FUSE. By default, FUSE uses threads to handle file system operations asynchronously. However, with the -s option, the FUSE program will operate in single-threaded mode, meaning it will handle only one operation at a time. This can be useful in certain situations.
// -d: This option activates the "d"ebug mode. When the -d option is used, the FUSE program will emit additional debugging messages, which can be helpful in understanding the behavior of the FUSE file system.
// ./[mountPoint]: specifies the mount point where the FUSE file system will be attached.
// 
// Then
// 	you will see a lot of logs
// 	on an other terminal go to your mount point then try different commands ;}


// Unmount the fileSystem
//  fusermount -u [mountPoint]


// All the information of the filesystem (e.g. created directories and files) will reside in the memory and will not be stored persistently on the disk, once the filesystem is unmounted, all of its contents will go.

// For the sake of simplicity, the new files and directories can be only created in the root directory of the filesystem “/”, we will call this limitation “root-level-only objects” to ease our discussions later on. 
// An object is either a directory or a file. 
// However, the concept will be the same if you want to make it possible to create files and directories in a sub-directory, but more advanced data structure should be used instead.


// Point of root-level-only : 
// 	in is_dir
// 		path++ : when the user needs to deal with an object (file, directory) in the filesystem, the full path of this object will be sent to our filesystem by FUSE
// 		For example, when the use whishes to write some content to the file "bar" which resides in the directory "foo", FUSE sends a write request to our filesystem with the full path "/foo/bar" of the file.

#define FUSE_USE_VERSION 30

#include <fuse3/fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

//The goal of the data structure is to keep the list of directories and files which are created by the user, also, the content of these files will be maintained by one of these data structures. 
//Because we aim to create a simple filesystem here, our data structures will be simple arrays.


char dir_list[256][256]; //first arry, hich maintains the names of directories that have been created by the user
int curr_dir_idx= -1;// current index of dir_list start whith -1 because initially there should be no entry

char files_list[ 256 ][ 256 ];// second array, which maintains the contthe names of the files that have been created by the user
int curr_file_idx = -1;// current index of file_list

char files_content[ 256 ][ 256 ];// maintains the contents of the files
int curr_files_content_idx = -1;// current index of file_content

// each array can store 256 strings and each string has the maximum length of 256 bytes
// That means we can only create 256 directories and 256 files
// The length of filename, directory name or file content can be only 256 character

void add_dir(const char *dir_name)
{
	curr_dir_idx++;
	strcpy(dir_list[curr_dir_idx], dir_name);
}// adds a new directory to the list of directories

int is_dir(const char *path)
{
	path++; //Elimination "/" in the path

	printf("iAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA : %d  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", path);

	for(int curr_idx= 0; curr_idx <= curr_dir_idx; curr_idx++)
	{
		if(strcmp(path, dir_list[curr_idx]) == 0)
		{
			return 1;
		}
	}
	return 0;
}// Check if this path is an available directory in the filesystem 
// by trying to find the name of directory in the directory list (dir_list)
// if the name is right it returns 1, else returns 0

void add_file(const char *filename)
{
	curr_file_idx++;
	strcpy(files_list[curr_file_idx], filename);

	curr_files_content_idx;
	strcpy(files_content[curr_files_content_idx], "");
}// adds a new file in the filesystem by adding a new entry in the array file_list wwith the name of the new file
// It also initializes the content of this file to be an empty string 
// and stores this content int the array files_content
//
// We use the index to refer to objects -- that way we have access to it's name and its content (cf inode ?)
// file_list[index) = name, file_content[index] = content

int is_file(const char *path)
{
	path++; // Eliminating "/" in the path

	for(int curr_idx= 0; curr_idx <= curr_file_idx; curr_idx++)
	{
		if(strcmp(path, files_list[curr_idx]) == 0)
		{
			return 1;
		}
	}
	return 0;
}// takes a path and see if there is a file with the same name exists in the filesystem
// It returns 1 if the file exists
// same as is_dir but regarding files


int get_file_index(const char *path)
{
	path++; // Eliminating "/" in the path

	for(int curr_idx= 0; curr_idx <= curr_file_idx; curr_idx++)
	{
		if(strcmp(path, files_list[curr_idx]) == 0)
		{
			return curr_idx;
		}
	}
	return -1;
}// Retruns the file_index that is valid to be used with the both arrays files_list, files_content
// uses sequential search on the array files_list to find if there is some file which has the given name
// if such file is found, index will be returned


void write_to_file(const char *path, const char *new_content)
{
	int file_idx= get_file_index(path);

	if(file_idx == 1) // No such file
	{
		return;
	}

	strcpy(files_content[file_idx], new_content);
}// takes the path of the file that the user wishes to write some content on 
// Then it writes this content on the file

static int do_getattr(const char *path, struct stat *st)
{
	printf("[getattr] Called\n");
	printf("\tAttributes of %s requested\n", path);

	// GNU's definitions of the attributes (http://www.gnu.org/software/libc/manual/html_node/Attribute-Meanings.html):
	// 		st_uid: 	The user ID of the file’s owner.
	//		st_gid: 	The group ID of the file.
	//		st_atime: 	This is the last access time for the file.
	//		st_mtime: 	This is the time of the last modification to the contents of the file.
	//		st_mode: 	Specifies the mode of the file. This includes file type information (see Testing File Type) and the file permission bits (see Permission Bits).
	//		st_nlink: 	The number of hard links to the file. This count keeps track of how many directories have entries for this file. If the count is ever decremented to zero, then the file itself is discarded as soon
	//						as no process still holds it open. Symbolic links are not counted in the total.
	//		st_size:	This specifies the size of a regular file in bytes. For files that are really devices this field isn’t usually meaningful. For symbolic links this specifies the length of the file name the link refers to.


	st->st_uid= getuid(); // The owner of the file/directory is the user who mounted the filesystem
	st->st_gid= getgid(); // The group of the file/directory is the same as the group of the user who mounted the filesystem
	st->st_atime= time(NULL); // The last "a"ccess of the file/directory is right now
	st->st_mtime= time(NULL); // The last "m"odification of the file/directory is right now

	if(strcmp(path, "/") == 0 || is_dir(path) == 1)
	{
		st->st_mode= S_IFDIR | 0755;
		st->st_nlink= 2; // Why "two" hardlinks instead of "one"? The answer is here: http://unix.stackexchange.com/a/101536
	}
	else if (is_file(path) ==1)
	{
		st->st_mode= S_IFREG | 0644;
		st->st_nlink= 1;
		st->st_size= 1024;
	}
	else
	{
		return -ENOENT;
	}

	return 0;
}


static int do_readdir(const char* path, void* buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
	printf("--> Getting The List of Files of %s\n", path);

	filler(buffer, ".", NULL, 0, FUSE_BUF_NO_SPLICE); //Current Directory
	filler(buffer, "..", NULL, 0, FUSE_BUF_NO_SPLICE); //Parent Directory

	if(strcmp( path, "/") == 0) // If the user is trying to show the files/directories of the root directory, show the following
	{
		for(int curr_idx= 0; curr_idx <= curr_dir_idx; curr_idx++)
		{
			filler(buffer, dir_list[curr_idx], NULL, 0, FUSE_BUF_NO_SPLICE);
		}
		for(int curr_idx= 0; curr_idx <= curr_file_idx; curr_idx)
		{
			filler(buffer, files_list[curr_idx], NULL, 0, FUSE_BUF_NO_SPLICE);
		}
	}
	return 0;
}

static int do_read(const char* path, char* buffer, size_t size, off_t offset, struct fuse_file_info* fi)
{
	int file_idx= get_file_index(path);

	if(file_idx == -1)
	{
		return -1;
	}

	char *content= files_content[file_idx];

	memcpy(buffer, content + offset, size);

	return strlen(content) - offset;
}

static int do_mkdir(const char *path, mode_t mode)
{
	path++;
	add_dir(path);

	return 0;
}

static int do_mknod(const char *path, mode_t mode, dev_t rdev)
{
	path++;
	add_file(path);

	return 0;
}

static int do_write(const char *path, const char *buffer, size_t size, off_t offset, struct fuse_file_info *info)
{
	write_to_file(path, buffer);

	return size;
}

static struct fuse_operations operation = {
	.getattr= do_getattr,
	.readdir= do_readdir,
	.read= do_read,
	.mkdir= do_mkdir,
	.mknod= do_mknod,
	.write= do_write,
};

int main(int argc, char* argv[])
{
	return fuse_main_real(argc, argv, &operation,sizeof(operation), NULL);
}


