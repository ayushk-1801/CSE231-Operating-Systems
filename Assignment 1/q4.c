#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct Process {
    int pid, arrival_time, burst_time, response_time, turnaround_time, start_time,
    completion_time, remaining_time, waiting_time;
};

int min(int a, int b) { return (a < b) ? a : b; }
int max(int a, int b) { return (a > b) ? a : b; }

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

float avgTurnaroundTime(struct Process process[], int n) {
    float res = 0;
    for (int i = 0; i < n; i++) {
        res += process[i].turnaround_time;
    }
    return res / n;
}

void fifo(struct Process process[], int n) {
    printf("First In First Out (FIFO)\n");
    qsort(process, n, sizeof(struct Process), cmpAT);
    
    int time = process[0].arrival_time;
    printf("Order of execution: ");
    for (int i = 0; i < n; i++) {
        time = max(time, process[i].arrival_time);

        process[i].start_time = time;
        process[i].completion_time = time + process[i].burst_time;
        process[i].turnaround_time =
            process[i].completion_time - process[i].arrival_time;
        process[i].response_time = process[i].start_time - process[i].arrival_time;
        process[i].waiting_time = process[i].turnaround_time - process[i].burst_time;
        time += process[i].burst_time;

        printf("%d", process[i].pid);
        if (i != n - 1) {
            printf(" -> ");
        } else {
            printf("\n");
        }
    }

    float avg_response_time = avgResponseTime(process, n);
    float avg_turnaround_time = avgTurnaroundTime(process, n);

    printf("Average response time: %.2f\n", avg_response_time);
    printf("Average turnaround time: %.2f\n", avg_turnaround_time);
    printf("\n");
}

void sjf(struct Process process[], int n) {
    printf("Shortest Job First (SJF)\n");
    qsort(process, n, sizeof(struct Process), cmpAT);
    int time = 0;
    bool executed[n];
    for (int i = 0; i < n; i++) {
        executed[i] = false;
    }
    
    printf("Order of execution: ");
    for (int i = 0; i < n; i++) {
        int shortest = -1;
        for (int j = 0; j < n; j++) {
            if (!executed[j] && process[j].arrival_time <= time) {
                if (shortest == -1 || process[j].burst_time < process[shortest].burst_time) {
                    shortest = j;
                }
            }
        }
        
        if (shortest == -1) {
            time++;
            i--;
            continue;
        }
        
        executed[shortest] = true;
        process[shortest].start_time = time;
        process[shortest].completion_time = time + process[shortest].burst_time;
        process[shortest].turnaround_time = process[shortest].completion_time - process[shortest].arrival_time;
        process[shortest].response_time = process[shortest].start_time - process[shortest].arrival_time;
        process[shortest].waiting_time = process[shortest].turnaround_time - process[shortest].burst_time;
        time = process[shortest].completion_time;
        
        printf("%d", process[shortest].pid);
        if (i != n - 1) {
            printf(" -> ");
        } else {
            printf("\n");
        }
    }
    
    float avg_response_time = avgResponseTime(process, n);
    float avg_turnaround_time = avgTurnaroundTime(process, n);

    printf("Average response time: %.2f\n", avg_response_time);
    printf("Average turnaround time: %.2f\n", avg_turnaround_time);
    printf("\n");
}

void srtf(struct Process process[], int n) {
    int time = 0, completed = 0;
    int shortest = -1;
    int finish_time;
    int order[n];
    int order_index = 0;
    bool is_completed[n];
    int remaining_time[n];

    printf("Shortest Remaining Time First (SRTF)\n");

    for(int i = 0; i < n; i++) {
        remaining_time[i] = process[i].burst_time;
        is_completed[i] = false;
    }

    while(completed != n) {
        shortest = -1;
        int min_burst = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(process[i].arrival_time <= time && !is_completed[i]) {
                if(remaining_time[i] < min_burst) {
                    min_burst = remaining_time[i];
                    shortest = i;
                }
                if(remaining_time[i] == min_burst) {
                    if(process[i].arrival_time < process[shortest].arrival_time) {
                        shortest = i;
                    }
                }
            }
        }

        if(shortest == -1) {
            time++;
        }
        else {
            if(remaining_time[shortest] == process[shortest].burst_time) {
                process[shortest].start_time = time;
                order[order_index++] = process[shortest].pid;
            }
            remaining_time[shortest]--;
            time++;

            if(remaining_time[shortest] == 0) {
                process[shortest].completion_time = time;
                process[shortest].turnaround_time = process[shortest].completion_time - process[shortest].arrival_time;
                process[shortest].waiting_time = process[shortest].turnaround_time - process[shortest].burst_time;
                process[shortest].response_time = process[shortest].start_time - process[shortest].arrival_time;

                is_completed[shortest] = true;
                completed++;
            }
        }
    }

    printf("Order of execution: ");
    for (int i = 0; i < order_index; i++) {
        printf("%d", order[i]);
        if (i != order_index - 1) {
            printf(" -> ");
        } else {
            printf("\n");
        }
    }

    float avg_response_time = avgResponseTime(process, n);
    float avg_turnaround_time = avgTurnaroundTime(process, n);

    printf("Average response time: %.2f\n", avg_response_time);
    printf("Average turnaround time: %.2f\n", avg_turnaround_time);
    printf("\n");
}

void rr(struct Process process[], int n, int time_quantum) {
    qsort(process, n, sizeof(struct Process), cmpAT);

    printf("Round Robin (RR) with quantum = %d\n", time_quantum);
    printf("Order of execution: ");

    int remaining_time[n];
    for (int i = 0; i < n; i++) {
        remaining_time[i] = process[i].burst_time;
        process[i].response_time = -1;
    }

    int time = 0;
    int completed = 0;
    while (completed < n) {
        bool flag = false;
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && process[i].arrival_time <= time) {
                flag = true;
                if (process[i].response_time == -1) {
                    process[i].response_time = time - process[i].arrival_time;
                }

                if (remaining_time[i] > time_quantum) {
                    time += time_quantum;
                    remaining_time[i] -= time_quantum;
                } else {
                    time += remaining_time[i];
                    process[i].completion_time = time;
                    process[i].turnaround_time = process[i].completion_time - process[i].arrival_time;
                    process[i].waiting_time = process[i].turnaround_time - process[i].burst_time;
                    remaining_time[i] = 0;
                    completed++;
                }

                printf("%d", process[i].pid);
                if (completed < n) {
                    printf(" -> ");
                }
            }
        }
        if (!flag) {
            time++;
        }
    }
    printf("\n");

    float avg_response_time = avgResponseTime(process, n);
    float avg_turnaround_time = avgTurnaroundTime(process, n);

    printf("Average response time: %.2f\n", avg_response_time);
    printf("Average turnaround time: %.2f\n", avg_turnaround_time);
    printf("\n");
}

int main() {
    int n;
    printf("Enter the number of process: ");
    scanf("%d", &n);

    struct Process process[n];

    printf("Enter PID, arrival time, and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &process[i].pid, &process[i].arrival_time,
              &process[i].burst_time);
        process[i].remaining_time = process[i].burst_time;
    }

    int time_quantum;
    printf("Enter time quantum for round robin: ");
    scanf("%d", &time_quantum);

    fifo(process, n);
    sjf(process, n);
    srtf(process, n);
    rr(process, n, time_quantum);

    return 0;
}