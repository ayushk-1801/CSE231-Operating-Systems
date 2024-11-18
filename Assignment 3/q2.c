#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int total_servers = 5;

sem_t channels[5];
int server_itr[5] = {0};

void *f(void *arg) {
  int server_id = *((int *)arg);
  int l = server_id;
  int r = (server_id + 1) % total_servers;

  while (server_itr[server_id] < 3) {
    printf("Server %d is waiting\n", server_id + 1);
    sleep(1);

    if (server_id == total_servers - 1) {
      sem_wait(&channels[r]);
      printf("Server %d acquired Channel %d\n", server_id + 1, r + 1);
      sem_wait(&channels[l]);
      printf("Server %d acquired Channel %d\n", server_id + 1, l + 1);
    } else {
      sem_wait(&channels[l]);
      printf("Server %d acquired Channel %d\n", server_id + 1, l + 1);
      sem_wait(&channels[r]);
      printf("Server %d acquired Channel %d\n", server_id + 1, r + 1);
    }

    printf("Server %d is processing (ITR %d)\n", server_id + 1,
           server_itr[server_id] + 1);
    sleep(1);

    server_itr[server_id]++;

    printf("Server %d releasing Channel %d\n", server_id + 1, r + 1);
    sem_post(&channels[r]);
    printf("Server %d releasing Channel %d\n", server_id + 1, l + 1);
    sem_post(&channels[l]);

    sleep(1);
  }
  return NULL;
}

int main() {
  pthread_t threads[total_servers];
  int server_ids[total_servers];

  for (int i = 0; i < total_servers; i++) {
    sem_init(&channels[i], 0, 1);
  }

  for (int i = 0; i < total_servers; i++) {
    server_ids[i] = i;
    pthread_create(&threads[i], NULL, f, &server_ids[i]);
  }

  for (int i = 0; i < total_servers; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < total_servers; i++) {
    sem_destroy(&channels[i]);
  }

  return 0;
}
