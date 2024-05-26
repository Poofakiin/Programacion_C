
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <pthread.h>
#include "maleta.h"

typedef struct {
  double *w;
  double *v;
  int *z;
  int n;
  double maxW;
  int k;
  double res;
} Args;



void *thread(void *p){
  Args *args = (Args*) p;
  double *w = args->w;
  double *v = args->v;
  int *z = args->z;
  int n = args->n;
  double maxW = args->maxW;
  int k = args->k;
  args->res = llenarMaletaSec(w, v, z, n, maxW, k);
  return NULL;
}

double llenarMaletaPar(double w[], double v[], int z[], int n,
                       double maxW, int k) {
  pthread_t pid[8];
  Args args[8];
  for(int j=0;j<8;j++){
    args[j].w = w;
    args[j].v = v;
    args[j].n = n;
    args[j].maxW = maxW;
    args[j].k = k/8;
    args[j].z = (int*) malloc(n * sizeof(int));
    pthread_create(&pid[j], NULL, thread, &args[j]);
  }
  double best = -1;
  int best_index = 0;
  for(int j=0; j<8; j++){
    pthread_join(pid[j],NULL);
    if (args[j].res >= best){
      best = args[j].res;
      best_index = j;
    }
  }
  for(int i=0; i<n; i++) {
        z[i]= args[best_index].z[i];
  }
  for(int j=0; j<8; j++){
    free(args[j].z);
  }
  return best;
}
