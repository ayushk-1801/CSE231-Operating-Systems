#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int row, col;
  int n;
  int **A;
  int **B;
  int **C;
} Task;

typedef struct {
  Task **tasks;
  int cnt;
  int currTask;
  int threadsWorking;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  bool flag;
} ThreadPool;

void *workerThread(void *arg) {
  ThreadPool *pool = (ThreadPool *)arg;
  while (1) {
    pthread_mutex_lock(&pool->mutex);

    while (pool->currTask >= pool->cnt && !pool->flag) {
      pthread_cond_wait(&pool->cond, &pool->mutex);
    }

    if (pool->flag && pool->currTask >= pool->cnt) {
      pthread_mutex_unlock(&pool->mutex);
      break;
    }

    Task *task = pool->tasks[pool->currTask++];
    pool->threadsWorking++;
    pthread_mutex_unlock(&pool->mutex);

    int sum = 0;
    for (int k = 0; k < task->n; k++) {
      sum += task->A[task->row][k] * task->B[k][task->col];
    }
    task->C[task->row][task->col] = sum;

    pthread_mutex_lock(&pool->mutex);
    pool->threadsWorking--;
    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);

    free(task);
  }
  return NULL;
}

int main() {
  int m, n, p;
  printf("Enter dimensions (m n p): ");
  scanf("%d %d %d", &m, &n, &p);

  int **A = malloc(m * sizeof(int *));
  int **B = malloc(n * sizeof(int *));
  int **C1 = malloc(m * sizeof(int *));
  int **C2 = malloc(m * sizeof(int *));
  for (int i = 0; i < m; i++) {
    A[i] = malloc(n * sizeof(int));
    C1[i] = malloc(p * sizeof(int));
    C2[i] = malloc(p * sizeof(int));
  }
  for (int i = 0; i < n; i++) {
    B[i] = malloc(p * sizeof(int));
  }

  printf("Enter matrix A (%dx%d):\n", m, n);
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      scanf("%d", &A[i][j]);

  printf("Enter matrix B (%dx%d):\n", n, p);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < p; j++)
      scanf("%d", &B[i][j]);

  clock_t start = clock();
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      C1[i][j] = 0;
      for (int k = 0; k < n; k++) {
        C1[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  clock_t end = clock();
  double time1 = (double)(end - start) / CLOCKS_PER_SEC;

  ThreadPool pool = {
      malloc(1000 * sizeof(Task *)), 0,    0, 0, PTHREAD_MUTEX_INITIALIZER,
      PTHREAD_COND_INITIALIZER,      false};
  pthread_t threads[4];
  for (int i = 0; i < 4; i++) {
    pthread_create(&threads[i], NULL, workerThread, &pool);
  }

  start = clock();
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      Task *task = malloc(sizeof(Task));
      task->row = i;
      task->col = j;
      task->n = n;
      task->A = A;
      task->B = B;
      task->C = C2;

      pthread_mutex_lock(&pool.mutex);
      pool.tasks[pool.cnt++] = task;
      pthread_cond_signal(&pool.cond);
      pthread_mutex_unlock(&pool.mutex);
    }
  }

  pthread_mutex_lock(&pool.mutex);
  while (pool.currTask < pool.cnt || pool.threadsWorking > 0) {
    pthread_cond_wait(&pool.cond, &pool.mutex);
  }
  pthread_mutex_unlock(&pool.mutex);

  pool.flag = true;
  pthread_cond_broadcast(&pool.cond);

  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }

  end = clock();
  double time2 = (double)(end - start) / CLOCKS_PER_SEC;

  printf("\nSequential Result:\n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      printf("%d ", C1[i][j]);
    }
    printf("\n");
  }

  printf("\nParallel Result (Thread Pool):\n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      printf("%d ", C2[i][j]);
    }
    printf("\n");
  }

  printf("\nSequential Time = %f secs\n", time1);
  printf("Parallel Time (Thread Pool) = %f secs\n", time2);
  printf("Speedup = %f\n", time1 / time2);

  for (int i = 0; i < m; i++) {
    free(A[i]);
    free(C1[i]);
    free(C2[i]);
  }
  for (int i = 0; i < n; i++) {
    free(B[i]);
  }
  free(A);
  free(B);
  free(C1);
  free(C2);
  free(pool.tasks);

  return 0;
}
