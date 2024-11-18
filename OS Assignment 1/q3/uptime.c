#include <stdio.h>
#include <sys/sysinfo.h>

int main() {
    struct sysinfo temp;

    if (sysinfo(&temp) == 0) {
        long uptime = temp.uptime;

        long hours = uptime / 3600;
        long minutes = (uptime % 3600) / 60;
        long seconds = uptime % 60;

        printf("Uptime: %ld hours, %ld minutes, %ld seconds\n", hours, minutes, seconds);
    } else {
        printf("Error!");
    }

    return 0;
}
