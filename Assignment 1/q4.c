#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, arrival_time, burst_time, response_time, turnaround_time, start_time,
    completion_time;
};

int min(int a, int b) { return (a < b) ? a : b; }

int cmpBT(const void *a, const void *b) {
    struct Process *p = (struct Process *)a;
    struct Process *q = (struct Process *)b;
    return p->burst_time - q->burst_time;
}

int cmpAT(const void *a, const void *b) {
    struct Process *p = (struct Process *)a;
    struct Process *q = (struct Process *)b;
    return p->arrival_time - q->arrival_time;
}

float avgResponseTime(struct Process process[], int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += process[i].response_time;
    }
    return res / n;
}

float avgTurnaroundtime(struct Process process[], int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += process[i].turnaround_time;
    }
    return res / n;
}

void scheduling(struct Process process[], int n){
    int time = process[0].arrival_time;
    printf("Order of execution: ");
    for (int i = 0; i < n; i++) {
        if (time < process[i].arrival_time) {
            time = process[i].arrival_time;
        }
        process[i].start_time = time;
        process[i].completion_time = time + process[i].burst_time;
        process[i].turnaround_time =
            process[i].completion_time - process[i].arrival_time;
        process[i].response_time = process[i].start_time - process[i].arrival_time;
        time += process[i].burst_time;

        printf("%d", process[i].pid);
        if (i != n - 1) {
            printf(" -> ");
        } else {
            printf("\n");
        }
    }

    float avg_response_time = avgResponseTime(process, n);
    float avg_turnaround_time = avgTurnaroundtime(process, n);

    printf("Average response time: %.2f\n", avg_response_time);
    printf("Average turnaround time: %.2f\n", avg_turnaround_time);
    printf("\n");
}

void fifo(struct Process process[], int n) {
    printf("First In First Out (FIFO)\n");
    scheduling(process, n);
}

void sjf(struct Process process[], int n) {
    qsort(process, n, sizeof(struct Process), cmpBT);

    printf("Shortest Job First (SJF)\n");
    scheduling(process, n);
}

void srtf() {}

void rr() { return; }

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process process[n];

    printf("Enter PID, arrival time, and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &process[i].pid, &process[i].arrival_time,
              &process[i].burst_time);
    }

    int time_quantum;
    printf("Enter time quantum for round robin: ");
    scanf("%d", &time_quantum);

    fifo(process, n);
    sjf(process, n);

    return 0;
}
