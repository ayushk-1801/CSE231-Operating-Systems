#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

const int NUM_THREADS = 4;

typedef struct {
  int row;
  int col;
  int m, n, p;
  int **matA;
  int **matB;
  int **result;
} Task;

typedef struct {
  Task **tasks;
  int taskCount;
  int currentTask;
  pthread_mutex_t mutex;
  pthread_cond_t condition;
  int threadsWorking;
  int isShutdown;
} ThreadPool;

ThreadPool *initThreadPool() {
  ThreadPool *pool = (ThreadPool *)malloc(sizeof(ThreadPool));
  pool->tasks = (Task **)malloc(sizeof(Task *) * 1000);
  pool->taskCount = 0;
  pool->currentTask = 0;
  pool->threadsWorking = 0;
  pool->isShutdown = 0;
  pthread_mutex_init(&pool->mutex, NULL);
  pthread_cond_init(&pool->condition, NULL);
  return pool;
}

void submitTask(ThreadPool *pool, Task *task) {
  pthread_mutex_lock(&pool->mutex);
  pool->tasks[pool->taskCount++] = task;
  pthread_cond_signal(&pool->condition);
  pthread_mutex_unlock(&pool->mutex);
}

void *workerThread(void *arg) {
  ThreadPool *pool = (ThreadPool *)arg;
  while (1) {
    pthread_mutex_lock(&pool->mutex);

    while (pool->currentTask >= pool->taskCount && !pool->isShutdown) {
      pthread_cond_wait(&pool->condition, &pool->mutex);
    }

    if (pool->isShutdown && pool->currentTask >= pool->taskCount) {
      pthread_mutex_unlock(&pool->mutex);
      break;
    }

    Task *task = pool->tasks[pool->currentTask++];
    pool->threadsWorking++;
    pthread_mutex_unlock(&pool->mutex);

    int sum = 0;
    for (int k = 0; k < task->n; k++) {
      sum += task->matA[task->row][k] * task->matB[k][task->col];
    }
    task->result[task->row][task->col] = sum;
    free(task);

    pthread_mutex_lock(&pool->mutex);
    pool->threadsWorking--;
    pthread_cond_signal(&pool->condition);
    pthread_mutex_unlock(&pool->mutex);
  }
  return NULL;
}

void destroyThreadPool(ThreadPool *pool) {
  pthread_mutex_lock(&pool->mutex);
  pool->isShutdown = 1;
  pthread_cond_broadcast(&pool->condition);
  pthread_mutex_unlock(&pool->mutex);

  pthread_mutex_destroy(&pool->mutex);
  pthread_cond_destroy(&pool->condition);
  free(pool->tasks);
  free(pool);
}

int main() {
  int m, n, p;
  printf("Enter dimensions (m n p): ");
  scanf("%d %d %d", &m, &n, &p);

  int **matA = (int **)malloc(m * sizeof(int *));
  int **matB = (int **)malloc(n * sizeof(int *));
  int **seqResult = (int **)malloc(m * sizeof(int *));
  int **parallelResult = (int **)malloc(m * sizeof(int *));

  for (int i = 0; i < m; i++) {
    matA[i] = (int *)malloc(n * sizeof(int));
    seqResult[i] = (int *)malloc(p * sizeof(int));
    parallelResult[i] = (int *)malloc(p * sizeof(int));
  }
  for (int i = 0; i < n; i++) {
    matB[i] = (int *)malloc(p * sizeof(int));
  }

  printf("Enter matrix A (%dx%d):\n", m, n);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      scanf("%d", &matA[i][j]);

  printf("Enter matrix B (%dx%d):\n", n, p);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < p; j++)
      scanf("%d", &matB[i][j]);

  struct timespec start, end;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      seqResult[i][j] = 0;
      for (int k = 0; k < n; k++) {
        seqResult[i][j] += matA[i][k] * matB[k][j];
      }
    }
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  double seqTime =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

  ThreadPool *pool = initThreadPool();
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, workerThread, pool);
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      Task *task = (Task *)malloc(sizeof(Task));
      task->row = i;
      task->col = j;
      task->m = m;
      task->n = n;
      task->p = p;
      task->matA = matA;
      task->matB = matB;
      task->result = parallelResult;
      submitTask(pool, task);
    }
  }

  pthread_mutex_lock(&pool->mutex);
  while (pool->currentTask < pool->taskCount || pool->threadsWorking > 0) {
    pthread_cond_wait(&pool->condition, &pool->mutex);
  }
  pthread_mutex_unlock(&pool->mutex);

  destroyThreadPool(pool);

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  double parallelTime =
      (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

  printf("\nSequential Result:\n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      printf("%d ", seqResult[i][j]);
    }
    printf("\n");
  }

  printf("\nParallel Result (Thread Pool):\n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      printf("%d ", parallelResult[i][j]);
    }
    printf("\n");
  }

  printf("\nSequential Time: %f seconds\n", seqTime);
  printf("Parallel Time (Thread Pool): %f seconds\n", parallelTime);
  printf("Speedup: %f\n", seqTime / parallelTime);

  for (int i = 0; i < m; i++) {
    free(matA[i]);
    free(seqResult[i]);
    free(parallelResult[i]);
  }
  for (int i = 0; i < n; i++) {
    free(matB[i]);
  }
  free(matA);
  free(matB);
  free(seqResult);
  free(parallelResult);

  return 0;
}
