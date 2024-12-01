#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <source_file> <dest_dir>\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "r");
    if (!src) {
        perror("fopen");
        return 1;
    }

    char destPath[1024];
    snprintf(destPath, sizeof(destPath), "%s/%s", argv[2], argv[1]);

    FILE *dest = fopen(destPath, "w");
    if (!dest) {
        perror("fopen");
        fclose(src);
        return 1;
    }

    char buff[1024];
    size_t bytes;
    while ((bytes = fread(buff, 1, sizeof(buff), src)) > 0) {
        fwrite(buff, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);

    printf("Successfully copied %s to %s\n", argv[1], argv[2]);
    return 0;
}
