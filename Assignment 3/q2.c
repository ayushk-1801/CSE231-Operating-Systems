#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t channels[5];
int server_itr[5] = {0};

void *f(void *arg) {
  int id = *((int *)arg);
  int l = id;
  int r = (id + 1) % 5;

  while (server_itr[id] < 3) {
    printf("Server %d is waiting\n", id + 1);
    sleep(1);

    if (id == 4) {
      sem_wait(&channels[r]);
      printf("Server %d acquired Channel %d\n", id + 1, r + 1);
      sem_wait(&channels[l]);
      printf("Server %d acquired Channel %d\n", id + 1, l + 1);
    } else {
      sem_wait(&channels[l]);
      printf("Server %d acquired Channel %d\n", id + 1, l + 1);
      sem_wait(&channels[r]);
      printf("Server %d acquired Channel %d\n", id + 1, r + 1);
    }

    printf("Server %d is processing (ITR %d)\n", id + 1,
           server_itr[id] + 1);
    sleep(1);

    server_itr[id]++;

    printf("Server %d releasing Channel %d\n", id + 1, r + 1);
    sem_post(&channels[r]);
    printf("Server %d releasing Channel %d\n", id + 1, l + 1);
    sem_post(&channels[l]);

    sleep(1);
  }
  return NULL;
}

int main() {
  pthread_t threads[5];
  int id[5];

  for (int i = 0; i < 5; i++) {
    sem_init(&channels[i], 0, 1);
  }

  for (int i = 0; i < 5; i++) {
    id[i] = i;
    pthread_create(&threads[i], NULL, f, &id[i]);
  }

  for (int i = 0; i < 5; i++) {
    pthread_join(threads[i], NULL);
  }

  for (int i = 0; i < 5; i++) {
    sem_destroy(&channels[i]);
  }

  return 0;
}
