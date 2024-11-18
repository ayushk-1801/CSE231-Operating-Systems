#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int row, col;
  int m, n, p;
  int **matA;
  int **matB;
  int **result;
} ThreadData;

void *parallelMul(void *arg) {
  ThreadData *data = (ThreadData *)arg;
  int sum = 0;

  for (int k = 0; k < data->n; k++) {
    sum += data->matA[data->row][k] * data->matB[k][data->col];
  }

  data->result[data->row][data->col] = sum;
  free(arg);
  return NULL;
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

  pthread_t threads[m * p];
  int threadCount = 0;

  clock_gettime(CLOCK_MONOTONIC, &start);
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));
      data->row = i;
      data->col = j;
      data->m = m;
      data->n = n;
      data->p = p;
      data->matA = matA;
      data->matB = matB;
      data->result = parallelResult;

      pthread_create(&threads[threadCount++], NULL, parallelMul, data);
    }
  }

  for (int i = 0; i < threadCount; i++) {
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

  printf("\nParallel Result:\n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      printf("%d ", parallelResult[i][j]);
    }
    printf("\n");
  }

  printf("\nSequential Time: %f seconds\n", seqTime);
  printf("Parallel Time: %f seconds\n", parallelTime);
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
