#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
  int row, col;
  int n;
  int **A;
  int **B;
  int **C;
} Thread;

void *f(void *arg) {
  Thread *data = (Thread *)arg;
  int sum = 0;
  for (int k = 0; k < data->n; k++) {
    sum += data->A[data->row][k] * data->B[k][data->col];
  }
  data->C[data->row][data->col] = sum;
  free(arg);
  return NULL;
}

int main() {
  int m, n, p;
  printf("Enter dimensions (m n p): ");
  scanf("%d %d %d", &m, &n, &p);

  int **A = malloc(m * sizeof(int *));
  int **B = malloc(n * sizeof(int *));
  int **C = malloc(m * sizeof(int *));
  for (int i = 0; i < m; i++)
    A[i] = malloc(n * sizeof(int));
  for (int i = 0; i < n; i++)
    B[i] = malloc(p * sizeof(int));
  for (int i = 0; i < m; i++)
    C[i] = malloc(p * sizeof(int));

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
      C[i][j] = 0;
      for (int k = 0; k < n; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  clock_t end = clock();
  double seqTime = (double)(end - start) / CLOCKS_PER_SEC;

  pthread_t threads[m * p];
  int threadCount = 0;

  start = clock();
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      Thread *data = malloc(sizeof(Thread));
      data->row = i;
      data->col = j;
      data->n = n;
      data->A = A;
      data->B = B;
      data->C = C;
      pthread_create(&threads[threadCount++], NULL, f, data);
    }
  }

  for (int i = 0; i < threadCount; i++) {
    pthread_join(threads[i], NULL);
  }
  end = clock();
  double parallelTime = (double)(end - start) / CLOCKS_PER_SEC;

  printf("\nResultant Matrix C:\n");
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < p; j++) {
      printf("%d ", C[i][j]);
    }
    printf("\n");
  }

  printf("\nSequential Time = %f secs\n", seqTime);
  printf("Parallel Time = %f secs\n", parallelTime);
  printf("Speedup = %f\n", seqTime / parallelTime);

  for (int i = 0; i < m; i++) {
    free(A[i]);
    free(C[i]);
  }
  for (int i = 0; i < n; i++) {
    free(B[i]);
  }
  free(A);
  free(B);
  free(C);

  return 0;
}
