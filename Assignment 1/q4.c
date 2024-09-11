#include <stdio.h>

struct Process{
    int pid, arrival_time, burst_time, response_time, turnaround_time;
};

void fifo(struct Process process[]){
    return;
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

    fifo(process);
    return 0;
}
