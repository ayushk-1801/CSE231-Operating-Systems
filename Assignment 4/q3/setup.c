#include <stdio.h>
#include <string.h>

struct Student {
    int id;
    char name[20];
};

int main() {
    struct Student students[5] = {
        {1, "Arhan Jain"},
        {2, "Arnesh Batra"},
        {3, "Anushk Kumar"},
        {4, "Anant Gyan Singhal"},
        {5, "Ayush Kitnawat"}
    };

    FILE *file = fopen("students.rec", "wb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        fwrite(&students[i], sizeof(struct Student), 1, file);
    }

    fclose(file);
    printf("students.rec file created successfully.\n");
    return 0;
}
