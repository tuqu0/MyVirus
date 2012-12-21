#include "../include/utils.h"

int addDirectory(Directory *list, Directory *elt) {
	Directory *it = NULL;

	if (list == NULL || elt == NULL)
		return -1;

	it = list;
	while (it->next != NULL)
		it=it->next;
	it->next = elt;
	return 0;	
}

int freeList(Directory *list) {
	Directory *it = NULL;
	Directory *del = NULL;

	if (list == NULL)
		return 0;

	it = list;
	while (it->next != NULL) {
		del = it;
		it = it->next;
		free(del);
	}
	return 0;
}

int listDirectories(Directory *list, char *path) {
	DIR *d;
	struct dirent *dir;
	Directory *elt;

	if (!path)
		return -1;

	d = opendir(path);
	if (d) {
		while ((dir = readdir(d)) != NULL) {
			if (dir->d_type == DT_DIR && strcmp(dir->d_name, "..") && strcmp(dir->d_name, ".")) {
				if (list->directory == NULL)
					list->directory = dir->d_name;
				else {
					elt = (Directory *) malloc(sizeof(Directory));
					elt->directory = dir->d_name;
					addDirectory(list, elt);
				}
			}
		}
	}
	return 0;
}

int copy(const char *from, const char *to) {
	int fd_to, fd_from;
	char buf[4096];
	ssize_t nread;
	int saved_errno;

	if (from == NULL || to == NULL)
		return -1;

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
	if (file == NULL)
		return -1;

	chmod(file, S_IRWXU);
	return 0;
}

int checkMarker(char *dir) {
	int ret = -1;
	char *marker_path = NULL;

	marker_path = (char *) malloc(strlen(dir) + strlen(MARKER) + 2);
	strcpy(marker_path, dir);
	strcat(marker_path, "/");
	strcat(marker_path, MARKER);

	if (dir == NULL)
		ret = -1;
	else if(access(marker_path, F_OK) == 0)
		ret = 0;
	printf("MARKER : %s\n", MARKER);
	printf("marker path : %s\n", marker_path);
	printf("ret : %d\n", ret);
	
	free(marker_path);
	return ret;
}
