#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

#include "pss.h"

Queue *queue;

typedef struct {
  int date;
  char* name;
} pairDir;

void listDir(char *nom, Queue *queue) {
  struct stat st_nom;
  int rc;
  rc= stat(nom, &st_nom);

  if (rc!=0) {
    exit(0);
  }
  if (S_ISREG(st_nom.st_mode)) {
    
    pairDir * pairs=malloc(sizeof(pairDir));
    pairs -> date = st_nom.st_mtime;
    pairs -> name = strdup(nom);
    put(queue, pairs);
  }
  else if (S_ISDIR(st_nom.st_mode)) {
    // Es un directorio
    DIR *dir = opendir(nom);
    if (dir == NULL) {
      perror(nom);
      exit(1);
    }
    for (struct dirent *entry = readdir(dir);
         entry != NULL;
         entry = readdir(dir)) {
      if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) {
        continue;
      }
      char *nom_arch= malloc(strlen(nom)+strlen(entry->d_name)+2);
      strcpy(nom_arch, nom);
      strcat(nom_arch, "/");
      strcat(nom_arch, entry->d_name);
      listDir(nom_arch, queue);
      free(nom_arch);
    }
    closedir(dir);
  }
  else {
    exit(1);
  }
}

int mtimeDirComparator(void *ptr, int i, int j) {
  pairDir **a = ptr;
  return a[j]->date - a[i]->date;
}

void mtimeDirSorted(Queue *queue, int n){
  int queue_length=queueLength(queue);
  pairDir * direcciones[queue_length];
  for(int i=0; i<queue_length;i++){
    direcciones[i]=get(queue);
  }
  destroyQueue(queue);
  sortPtrArray(direcciones,0,queue_length-1,mtimeDirComparator);
  for(int i=0; i<queue_length;i++){
    if(i<n){
      printf("%s\n", direcciones[i]->name);
    }
    free(direcciones[i]->name);
    free(direcciones[i]);
  } 
}


int main(int argc, char *argv[]) {
  queue=makeQueue();
  listDir(argv[1],queue);
  mtimeDirSorted(queue,atoi(argv[2]));
}





