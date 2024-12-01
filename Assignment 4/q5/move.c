#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <source_dir> <dest_dir>\n", argv[0]);
        return 1;
    }

    DIR *dir = opendir(argv[1]);
    if (!dir) {
        perror("Invalid source directory");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;

        char srcPath[1024];
        char destPath[1024];
        snprintf(srcPath, sizeof(srcPath), "%s/%s", argv[1], entry->d_name);
        snprintf(destPath, sizeof(destPath), "%s/%s", argv[2], entry->d_name);

        if (rename(srcPath, destPath) != 0) {
            perror("rename");
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);
    return 0;
}
