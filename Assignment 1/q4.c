#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

struct Process {
    int pid, AT, BT, ResT, TaT, ST, CT, WT, RemT;
};

int min(int a, int b) { return (a < b) ? a : b; }
int max(int a, int b) { return (a > b) ? a : b; }

int cmpBT(const void *a, const void *b) {
    struct Process *p = (struct Process *)a;
    struct Process *q = (struct Process *)b;
    return p->BT - q->BT;
}

int cmpAT(const void *a, const void *b) {
    struct Process *p = (struct Process *)a;
    struct Process *q = (struct Process *)b;
    return p->AT - q->AT;
}

float avgResponseTime(struct Process process[], int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += process[i].ResT;
    }
    return res / n;
}

float avgTurnaroundTime(struct Process process[], int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += process[i].TaT;
    }
    return res / n;
}

void fifo(struct Process process[], int n) {
    printf("First In First Out (FIFO)\n");
    qsort(process, n, sizeof(struct Process), cmpAT);

    int time = process[0].AT;
    printf("Order of execution: ");
    for (int i = 0; i < n; i++) {
        time = max(time, process[i].AT);

        process[i].ST = time;
        process[i].CT = time + process[i].BT;
        process[i].TaT = process[i].CT - process[i].AT;
        process[i].ResT = process[i].ST - process[i].AT;
        process[i].WT = process[i].TaT - process[i].BT;
        time += process[i].BT;

        printf("%d", process[i].pid);
        if (i != n - 1) {
            printf(" -> ");
        } else {
            printf("\n");
        }
    }

    float avgResT = avgResponseTime(process, n);
    float avgTaT = avgTurnaroundTime(process, n);

    printf("Average response time: %.2f\n", avgResT);
    printf("Average turnaround time: %.2f\n", avgTaT);
    printf("\n");
}

void sjf(struct Process process[], int n) {
    printf("Shortest Job First (SJF)\n");

    qsort(process, n, sizeof(struct Process), cmpAT);
    int time = 0;
    bool done[n];
    for (int i = 0; i < n; i++) {
        done[i] = false;
    }

    printf("Order of execution: ");
    for (int i = 0; i < n; i++) {
        int shortest = -1;
        for (int j = 0; j < n; j++) {
            if (!done[j] && process[j].AT <= time) {
                if (shortest == -1 || process[j].BT < process[shortest].BT) {
                    shortest = j;
                }
            }
        }

        if (shortest == -1) {
            time++;
            i--;
            continue;
        }

        done[shortest] = true;
        process[shortest].ST = time;
        process[shortest].CT = time + process[shortest].BT;
        process[shortest].TaT = process[shortest].CT - process[shortest].AT;
        process[shortest].ResT = process[shortest].ST - process[shortest].AT;
        process[shortest].WT = process[shortest].TaT - process[shortest].BT;
        time = process[shortest].CT;

        printf("%d", process[shortest].pid);
        if (i != n - 1) {
            printf(" -> ");
        } else {
            printf("\n");
        }
    }

    float avgResT = avgResponseTime(process, n);
    float avgTaT = avgTurnaroundTime(process, n);

    printf("Average response time: %.2f\n", avgResT);
    printf("Average turnaround time: %.2f\n", avgTaT);
    printf("\n");
}

void srtf(struct Process process[], int n) {
    int time = 0, completed = 0;
    int shortest = -1;
    int finish_time;
    int order[n];
    int idx = 0;
    bool done[n];

    printf("Shortest Remaining Time First (SRTF)\n");

    for (int i = 0; i < n; i++) {
        process[i].RemT = process[i].BT;
        done[i] = false;
    }

    while (completed != n) {
        shortest = -1;
        int min_burst = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (process[i].AT <= time && !done[i]) {
                if (process[i].RemT < min_burst) {
                    min_burst = process[i].RemT;
                    shortest = i;
                }
                if (process[i].RemT == min_burst) {
                    if (process[i].AT < process[shortest].AT) {
                        shortest = i;
                    }
                }
            }
        }

        if (shortest == -1) {
            time++;
        } else {
            if (process[shortest].RemT == process[shortest].BT) {
                process[shortest].ST = time;
                order[idx++] = process[shortest].pid;
            }
            process[shortest].RemT--;
            time++;

            if (process[shortest].RemT == 0) {
                process[shortest].CT = time;
                process[shortest].TaT =
                    process[shortest].CT - process[shortest].AT;
                process[shortest].WT =
                    process[shortest].TaT - process[shortest].BT;
                process[shortest].ResT =
                    process[shortest].ST - process[shortest].AT;

                done[shortest] = true;
                completed++;
            }
        }
    }

    printf("Order of execution: ");
    for (int i = 0; i < idx; i++) {
        printf("%d", order[i]);
        if (i != idx - 1) {
            printf(" -> ");
        } else {
            printf("\n");
        }
    }

    float avgResT = avgResponseTime(process, n);
    float avgTaT = avgTurnaroundTime(process, n);

    printf("Average response time: %.2f\n", avgResT);
    printf("Average turnaround time: %.2f\n", avgTaT);
    printf("\n");
}

void rr(struct Process process[], int n, int time_quantum) {
    qsort(process, n, sizeof(struct Process), cmpAT);

    printf("Round Robin (RR) with quantum = %d\n", time_quantum);
    printf("Order of execution: ");

    for (int i = 0; i < n; i++) {
        process[i].RemT = process[i].BT;
        process[i].ResT = -1;
    }

    int time = 0;
    int completed = 0;
    while (completed < n) {
        bool f = false;
        for (int i = 0; i < n; i++) {
            if (process[i].RemT > 0 && process[i].AT <= time) {
                f = true;
                if (process[i].ResT == -1) {
                    process[i].ResT = time - process[i].AT;
                }

                if (process[i].RemT > time_quantum) {
                    time += time_quantum;
                    process[i].RemT -= time_quantum;
                } else {
                    time += process[i].RemT;
                    process[i].CT = time;
                    process[i].TaT = process[i].CT - process[i].AT;
                    process[i].WT = process[i].TaT - process[i].BT;
                    process[i].RemT = 0;
                    completed++;
                }

                printf("%d", process[i].pid);
                if (completed < n) {
                    printf(" -> ");
                }
            }
        }
        if (!f) {
            time++;
        }
    }
    printf("\n");

    float avgResT = avgResponseTime(process, n);
    float avgTaT = avgTurnaroundTime(process, n);

    printf("Average response time: %.2f\n", avgResT);
    printf("Average turnaround time: %.2f\n", avgTaT);
    printf("\n");
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process process[n];
    printf("Enter PID, arrival time, and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        if (scanf("%d %d %d", &process[i].pid, &process[i].AT, &process[i].BT) != 3) {
            printf("Invalid input for process %d.\n", i + 1);
            exit(1);
        }
        process[i].RemT = process[i].BT;
    }

    int time_quantum;
    printf("Enter time quantum for round robin: ");
    if (scanf("%d", &time_quantum) != 1 || time_quantum <= 0) {
        printf("Invalid time quantum.\n");
        exit(1);
    }

    int rc = fork();

    if (rc < 0) {
        printf("Fork failed.\n");
        exit(1);
    } else if (rc == 0) {
        fifo(process, n);
        sjf(process, n);
        srtf(process, n);
        rr(process, n, time_quantum);
        exit(0);
    } else {
        wait(NULL);
    }

    return 0;
}