#include <stdio.h>
#include <string.h>
#include <time.h>

void date() {
    struct tm *ptr;
    time_t t;
    t = time(NULL);
    ptr = localtime(&t);
    printf("IST: %s", asctime(ptr));
}

void dateUTC() {
    struct tm *ptr;
    time_t t;
    t = time(NULL);
    ptr = gmtime(&t);
    printf("UTC: %s", asctime(ptr));
}

void dateRFC() {
    struct tm *ptr;
    time_t t;
    t = time(NULL);
    ptr = gmtime(&t);

    char days[][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char months[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    printf("GMT: %s %02d %s %d %02d:%02d:%02d\n", days[ptr->tm_wday], ptr->tm_mday, months[ptr->tm_mon], ptr->tm_year + 1900, ptr->tm_hour, ptr->tm_min, ptr->tm_sec);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        date();
    } else if (argc == 2) {
        if (strcmp(argv[1], "-u") == 0) {
            dateUTC();
        } else if (strcmp(argv[1], "-r") == 0) {
            dateRFC();
        } else {
            printf("Incorrect flags!\n");
        }
    } else {
        printf("Invalid command!\n");
    }
    return 0;
}
