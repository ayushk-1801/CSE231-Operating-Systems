#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

struct Student {
    int id;
    char name[20];
};

int main() {
    int id;
    printf("Enter student id (1-5): ");
    scanf("%d", &id);

    if (id < 1 || id > 5) {
        printf("Invalid ID\n");
        return 1;
    }

    int f = open("students.rec", O_RDONLY);
    if (f < 0) {
        perror("Error opening file");
        return 1;
    }

    off_t offset = (id - 1) * sizeof(struct Student);
    if (lseek(f, offset, SEEK_SET) == (off_t)-1) {
        perror("Error seeking file");
        close(f);
        return 1;
    }

    struct Student student;
    if (read(f, &student, sizeof(struct Student)) != sizeof(struct Student)) {
        perror("Error reading file");
        close(f);
        return 1;
    }

    close(f);

    printf("Student name: %s\n", student.name);

    return 0;
}
