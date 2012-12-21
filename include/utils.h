#ifndef UTILS_H
#define UTILS_H

 #include <dirent.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <errno.h>
 #include <string.h>
 #include <sys/stat.h>

 #define MARKER "MyVirus.out"

// An element of a list of directories
 typedef struct Directory_ {
 	struct Directory_ *next;
	char *directory;

 } Directory;

 // Add an Directory Element in a list
 int addDirectory(Directory *list, Directory *elt);

 // Free each Directory element in a list
 int freeList(Directory *list);

 /* 
 * List all directories in the given path.
 * subdirectories are not listed.
 */
 int listDirectories(Directory *list, char *path);

 /*
 * Copy a source file at a given destination
 */
 int copy(const char *from, const char *to);

 /*
 * Set full access on a file for the owner
 */
 int setExecutable(char *file);

 /*
 * Check is the defined marker is present in the given directory
 */
 int checkMarker(char *dir);
#endif /* UTILIS_H */
