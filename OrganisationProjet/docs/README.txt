// First README of SUS


This code has been created thanks to : https://maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/ 
and thanks to : https://maastaar.net/fuse/linux/filesystem/c/2019/09/28/writing-less-simple-yet-stupid-filesystem-using-FUSE-in-C/ 


----------OUR AMBITION ----------
Our SECURE USER SYSTEM also called SUS is a fileSystem created to secure your data. Google and every company who are providing cloud support have an acces to your datas. With SUS, we wish to enable you to prevent your data to be seen and only you and the people you whant can have access to it. It provides a file system who is linked to a cloud (for now dropbox) and it encrypt your data when sent. Also, when they are received by the cloud, they are all in the same hierarchy. WHat that means is : they don't even know how you organise your file system. SUS is responsible for the encryption and decryption of your datas and it will also take in charge the arborescence you see and make it invisible for the cloud storage.

---------- HOW TO USE SUS ----------
First, make sure you have all the files and directories of our SUS project.

To use the code, you have to use the Makefile. 
First you want to compile and add the right directories, so use the command line "make". 
Then if you want to use our program, use the command line : "make run".
	It will launch the program on the terminal.
	On an other terminal you can go to you mount point and try our SUS !
	To unmount SUS, you can ^C in the first terminal or use : "fusermount -u [mountPoint]" on the other terminal by replacing mountPoint by your mountPoint
		(the name of the mount point can be changed in the makefile)

When you whant to delete what you have done, you can use the "make clean" command.


---------- DESCRIPTION OF THE MAKEFILE ----------
In the Makefile, there is 6 variables, if you want, you can change them, there is an accurate description of each variable in the makefile itself.

To compile the makefile will use : 
	gcc [name].c -o [name] -lfuse3
and to launch the program it will type:
	./[name] -f -s -d ./[mountPoint]
 
-f: instructs the FUSE program to stay in the "f"oreground rather than detaching and going to the background.
-s: This option activates the "s"ingle-threaded mode of FUSE. By default, FUSE uses threads to handle file system operations asynchronously. However, with the -s option, the FUSE program will operate in single-threaded mode, meaning it will handle only one operation at a time. This can be useful in certain situations.
-d: This option activates the "d"ebug mode. When the -d option is used, the FUSE program will emit additional debugging messages, which can be helpful in understanding the behavior of the FUSE file system.
./[mountPoint]: specifies the mount point where the FUSE file system will be attached.
 
Then
	you will see a lot of logs (which are now in the log directory)
	on an other terminal, go to your mount point then try different commands ;}

---------- OTHER INFORMATIONS ----------
All the information of the filesystem (e.g. created directories and files) will reside in the memory and will not be stored persistently on the disk, once the filesystem is unmounted, all of its contents will go. For the sake of simplicity, the new files and directories can be only created in the root directory of the filesystem “/”, we will call this limitation “root-level-only objects” to ease our discussions later on. An object is either a directory or a file. However, the concept will be the same if you want to make it possible to create files and directories in a sub-directory, but more advanced data structure should be used instead.

The point above is subject to change

Point of root-level-only : 
	in is_dir
	path++ : when the user needs to deal with an object (file, directory) in the filesystem, the full path of this object will be sent to our filesystem by FUSE
	For example, when the use whishes to write some content to the file "bar" which resides in the directory "foo", FUSE sends a write request to our filesystem with the full path "/foo/bar" of the file.

