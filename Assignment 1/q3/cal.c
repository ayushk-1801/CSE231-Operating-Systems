#include <stdio.h>
#include <stdlib.h>

struct Month {
    const char *name;
    int days;
};

int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int zellerCongruence(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year--;
    }

    int k = year % 100;
    int j = year / 100;

    return ((day + (13 * (month + 1)) / 5 + k + k / 4 + j / 4 - 2 * j) % 7 + 6) % 7;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Invalid command!\n");
        return 1;
    }

    int month = atoi(argv[1]);
    int year = atoi(argv[2]);

    if (month < 1 || month > 12 || year < 1) {
        printf("Invalid month or year.\n");
        return 1;
    }

    struct Month months[] = {{"January", 31}, {"February", 28}, {"March", 31},
        {"April", 30},   {"May", 31},      {"June", 30},
        {"July", 31},    {"August", 31},   {"September", 30},
        {"October", 31}, {"November", 30}, {"December", 31}};

    if (isLeapYear(year) && month == 2) {
        months[1].days = 29;
    }

    printf("    %s %d\n", months[month - 1].name, year);
    printf("Su Mo Tu We Th Fr Sa\n");

    int dayOne = zellerCongruence(1, month, year);

    for (int i = 0; i < dayOne; i++) {
        printf("   ");
    }

    for (int i = 1; i <= months[month - 1].days; i++) {
        printf("%2d ", i);
        if ((i + dayOne) % 7 == 0) {
            printf("\n");
        }
    }
    printf("\n");

    return 0;
}
