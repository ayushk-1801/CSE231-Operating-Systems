#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_B = PTHREAD_MUTEX_INITIALIZER;

int thread_itr[3] = {0, 0, 0};

void *f(void *arg) {
  int thread_id = *((int *)arg);

  while (thread_itr[thread_id - 1] < 3) {
    printf("T%d try to acquire Lock A\n", thread_id);
    pthread_mutex_lock(&lock_A);
    printf("T%d acquired Lock A\n", thread_id);

    sleep(1);

    printf("T%d try to acquire Lock B\n", thread_id);
    pthread_mutex_lock(&lock_B);
    printf("T%d acquired Lock B\n", thread_id);

    printf("T%d in critical section (ITR %d)\n", thread_id,
           thread_itr[thread_id - 1] + 1);
    sleep(2);

    thread_itr[thread_id - 1]++;

    printf("T%d releasing Lock B\n", thread_id);
    pthread_mutex_unlock(&lock_B);
    printf("T%d releasing Lock A\n", thread_id);
    pthread_mutex_unlock(&lock_A);

    sleep(1);
  }
  return NULL;
}

int main() {
  int ids[3];
  pthread_t threads[3];

  for (int i = 0; i < 3; i++) {
    ids[i] = i + 1;
    pthread_create(&threads[i], NULL, f, &ids[i]);
  }

  for (int i = 0; i < 3; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
