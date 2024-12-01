#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_STATN 100000
#define TOTAL_STATN_NAMES 1000

typedef struct {
    char name[TOTAL_STATN_NAMES];
    int cnt;
    double min, max, total;
} Station;

Station station[TOTAL_STATN] = {0};

unsigned long custom_hash(const char *name) {
    unsigned long hash = 5381;
    for (int i = 0; name[i]; i++) {
        hash = ((hash << 5) + hash) + name[i];
    }
    return hash % TOTAL_STATN;
}

void process_line(char *line) {
    char *name = strtok(line, ";");
    char *temp = strtok(NULL, ";");

    if (!name || !temp)
        return;

    double temperature = atof(temp);
    unsigned long idx = custom_hash(name);

    Station *current_station = &station[idx];
    if (current_station->cnt == 0 || strcmp(current_station->name, name) != 0) {
        strcpy(current_station->name, name);
        current_station->min = temperature;
        current_station->max = temperature;
        current_station->total = temperature;
        current_station->cnt = 1;
    } else {
        if (temperature < current_station->min)
            current_station->min = temperature;
        if (temperature > current_station->max)
            current_station->max = temperature;
        current_station->total += temperature;
        current_station->cnt++;
    }
}

void soln_mmap(const char *filename) {
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        close(fd);
        return;
    }

    char *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("Error memory mapping file");
        close(fd);
        return;
    }

    madvise(mapped, sb.st_size, MADV_SEQUENTIAL);
    memset(station, 0, sizeof(station));

    char *line_start = mapped;
    char *file_end = mapped + sb.st_size;
    char line_buffer[256];

    while (line_start < file_end) {
        char *line_end = memchr(line_start, '\n', file_end - line_start);
        if (!line_end)
            break;

        size_t line_length = line_end - line_start;
        if (line_length >= sizeof(line_buffer))
            line_length = sizeof(line_buffer) - 1;
        memcpy(line_buffer, line_start, line_length);
        line_buffer[line_length] = '\0';

        process_line(line_buffer);

        line_start = line_end + 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_time =
        (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nMMAP Results:\n");
    for (int i = 0; i < TOTAL_STATN; i++) {
        if (station[i].cnt > 0) {
            printf("%s min=%.1f mean=%.1f max=%.1f\n", station[i].name,
                   station[i].min, station[i].total / station[i].cnt, station[i].max);
        }
    }

    printf("\nTime taken by mmap: %.3f seconds\n", elapsed_time);

    munmap(mapped, sb.st_size);
    close(fd);
}

void soln_fread(const char *filename) {
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    memset(station, 0, sizeof(station));

    char line_buffer[256];
    while (fgets(line_buffer, sizeof(line_buffer), file)) {
        process_line(line_buffer);
    }

    clock_gettime(CLOCK_MONOTONIC, &stop);
    double elapsed_time =
        (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nFREAD Results:\n");
    for (int i = 0; i < TOTAL_STATN; i++) {
        if (station[i].cnt > 0) {
            printf("%s min=%.1f mean=%.1f max=%.1f\n", station[i].name,
                   station[i].min, station[i].total / station[i].cnt, station[i].max);
        }
    }

    printf("\nTime taken by fread: %.3f seconds\n", elapsed_time);

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    soln_mmap(argv[1]);
    soln_fread(argv[1]);

    return 0;
}
