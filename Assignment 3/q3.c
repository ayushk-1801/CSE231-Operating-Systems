#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  int items[69];
  int in, out, count;
  pthread_mutex_t mutex;
  sem_t empty, filled;
} CircularBuffer;

CircularBuffer warehouse = {
    .in = 0, .out = 0, .count = 0, .mutex = PTHREAD_MUTEX_INITIALIZER};

bool flag = 1;

void *manager(void *arg) {
  int manager_id = *(int *)arg;
  while (flag) {
    sem_wait(&warehouse.filled);
    pthread_mutex_lock(&warehouse.mutex);

    int products = warehouse.items[warehouse.out];
    warehouse.out = (warehouse.out + 1) % 69;
    warehouse.count -= products;

    printf("Manager %d stored %d products [Inventory: %d]\n", manager_id,
           products, warehouse.count);

    pthread_mutex_unlock(&warehouse.mutex);
    sem_post(&warehouse.empty);

    sleep(1);
  }
  free(arg);
  return NULL;
}

void *truck(void *arg) {
  int truck_id = *(int *)arg;
  while (flag) {
    int products = (rand() % 5) + 1;

    sem_wait(&warehouse.empty);
    pthread_mutex_lock(&warehouse.mutex);

    warehouse.items[warehouse.in] = products;
    warehouse.in = (warehouse.in + 1) % 69;
    warehouse.count += products;

    printf("Truck %d delivered %d products [Inventory: %d]\n", truck_id,
           products, warehouse.count);

    pthread_mutex_unlock(&warehouse.mutex);
    sem_post(&warehouse.filled);

    sleep(1);
  }
  free(arg);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <num_trucks> <num_managers>\n", argv[0]);
    return 1;
  }

  int num_trucks = atoi(argv[1]);
  int num_managers = atoi(argv[2]);

  sem_init(&warehouse.empty, 0, 69);
  sem_init(&warehouse.filled, 0, 0);

  srand(time(NULL));

  pthread_t *truck_threads = malloc(num_trucks * sizeof(pthread_t));
  pthread_t *manager_threads = malloc(num_managers * sizeof(pthread_t));

  for (int i = 0; i < num_trucks; i++) {
    int *id = malloc(sizeof(int));
    *id = i + 1;
    pthread_create(&truck_threads[i], NULL, truck, id);
  }

  for (int i = 0; i < num_managers; i++) {
    int *id = malloc(sizeof(int));
    *id = i + 1;
    pthread_create(&manager_threads[i], NULL, manager, id);
  }

  sleep(10);
  flag = 0;

  for (int i = 0; i < num_trucks; i++) {
    pthread_join(truck_threads[i], NULL);
  }
  for (int i = 0; i < num_managers; i++) {
    pthread_join(manager_threads[i], NULL);
  }

  printf("\nFinal warehouse inventory size: %d products\n", warehouse.count);
  return 0;
}
