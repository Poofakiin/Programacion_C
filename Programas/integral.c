#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "integral.h"

double integral_par(Funcion f, void *ptr, double xi, double xf, int n, int p) {
  int pids[p];
  int fds[p][2];
  double block_len = (xf-xi)/p;
  
  for(int i=0;i<p;i++){
    pipe(fds[i]);
    pids[i] = fork();
    if(pids[i]==0){
      close(fds[i][0]);
      double left=xi+(block_len*i);
      double right=xi+(block_len*(i+1));
      double mini_integral=integral(f,ptr, left, right,n/p);
      write(fds[i][1],&mini_integral,sizeof(double));
      exit(0);
    }
    else{
      close(fds[i][1]);
    }
  }
  double final_answer=0;
  for(int i=0;i<p;i++){
    double child_answer;
    read(fds[i][0],&child_answer,sizeof(double));
    close(fds[i][0]);
    waitpid(pids[i], NULL, 0);
    final_answer+=child_answer;
  }
  return final_answer;
}

