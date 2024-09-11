#include <stdio.h>
#include <stdlib.h>

struct Process{
    int pid, arrival_time, burst_time, response_time, turnaround_time;
};

int cmp(const void *p, const void *q){
    const struct Process *proc1 = (const struct Process*)p;
    const struct Process *proc2 = (const struct Process*)q;

    return proc1->arrival_time - proc2->arrival_time;
}

void fifo(struct Process process[], int n){
    qsort(process, n, sizeof(struct Process), cmp);

    for(int i=0; i<n; i++){
        printf("%d\n", process[i].pid);
    }
}

void sjf(){
    return;
}

void srtf(){
    return;
}

void rr(){
    return;
}

int main(){
    int n;
    scanf("%d", &n);

    struct Process process[n];

    for(int i=0; i<n; i++){
        struct Process p = process[i];
        scanf("%d %d %d", &p.pid, &p.arrival_time, &p.burst_time);
    }

    int time_quantum;
    scanf("%d", &time_quantum);

    fifo(process, n);
    return 0;
}
