#include "../include/utils.h"

int addDirectory(Directory *list, Directory *elt) {
	Directory *iterator = NULL;

	iterator = list;
	while (iterator->next != NULL)
		iterator = iterator->next;
	iterator->next = elt;

	return 0;	
}

int freeList(Directory *list) {
	Directory *iterator = NULL;
	Directory *delete = NULL;

	iterator = list;
	while (iterator->next != NULL) {
		delete = iterator;
		iterator = iterator->next;
		free(delete->directory);
		free(delete);
	}
	free(iterator->directory);
	free(iterator);

	return 0;
}

int buildList(Directory *list, char *path) {
	int ret = -1;
	char directory_path[PATH_MAX];
	struct dirent *dir = NULL;
	Directory *elt = NULL;
	DIR *root_directory = NULL;

	root_directory = opendir(path);
	if (root_directory) {
		while ((dir = readdir(root_directory)) != NULL) {
			if (dir->d_type == DT_DIR && strcmp(dir->d_name, "..") && strcmp(dir->d_name, ".")) {
				if (list->directory == NULL) {
					realpath(dir->d_name, directory_path);
					list->directory = (char *) malloc(strlen(directory_path) + 1);
					strcpy(list->directory, directory_path);
				}
				else {
					elt = (Directory *) malloc(sizeof(Directory));
					realpath(dir->d_name, directory_path);
					elt->directory = (char *) malloc(strlen(directory_path) + 1);
					strcpy(elt->directory, directory_path);
					addDirectory(list, elt);
				}
				memset(directory_path, 0, PATH_MAX);
			}
		}
		closedir(root_directory);
		ret = 0;
	}

	return ret;
}

int copy(const char *from, const char *to) {
	int fd_to;
	int fd_from;
	int saved_errno;
	char buf[PATH_MAX];
	ssize_t nread;

	fd_from = open(from, O_RDONLY);
	if (fd_from < 0)
		return -1;

	fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fd_to < 0)
		goto out_error;

	while (nread = read(fd_from, buf, sizeof buf), nread > 0) {
		char *out_ptr = buf;
		ssize_t nwritten;

		do {
			nwritten = write(fd_to, out_ptr, nread);

			if (nwritten >= 0) {
				nread -= nwritten;
				out_ptr += nwritten;
			}
			else if (errno != EINTR)
				goto out_error;
		} while (nread > 0);
	}

	if (nread == 0) {
		if (close(fd_to) < 0) {
			fd_to = -1;
			goto out_error;
		}
		close(fd_from);

		return 0;
	}

out_error:
	saved_errno = errno;

	close(fd_from);
	if (fd_to >= 0)
		close(fd_to);

	errno = saved_errno;

	return -1;
}

int setExecutable(char *file) {
	chmod(file, S_IRUSR | S_IXUSR | S_IWUSR);

	return 0;
}

int markerExists(char *dir) {
	int ret = -1;
	char *marker_path = NULL;

	marker_path = (char *) malloc(strlen(dir) + strlen(MARKER) + 2);
	strcpy(marker_path, dir);
	strcat(marker_path, "/");
	strcat(marker_path, MARKER);

	if(access(marker_path, R_OK) == 0)
		ret = 0;

	free(marker_path);

	return ret;
}
