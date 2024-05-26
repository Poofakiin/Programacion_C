#define _XOPEN_SOURCE 500

#include "nthread-impl.h"

#include "rwlock.h"

struct rwlock{
  int lectores;
  int escritores;
  NthQueue *escritoresQueue;
  NthQueue *lectoresQueue;
};

nRWLock *nMakeRWLock() {
  nRWLock *newLock = (nRWLock *)nMalloc(sizeof(nRWLock));
  newLock->lectores = 0;
  newLock->escritores = 0;
  newLock->escritoresQueue = nth_makeQueue();
  newLock->lectoresQueue = nth_makeQueue();
  return newLock;
}

void nDestroyRWLock(nRWLock *rwl) {
  nth_destroyQueue(rwl->escritoresQueue);
  nth_destroyQueue(rwl->lectoresQueue);
  free(rwl);
}

int nEnterRead(nRWLock *rwl, int timeout) {
  START_CRITICAL
  
  //Caso de espera
  if(rwl->escritores == 0 && nth_emptyQueue(rwl->escritoresQueue)){
    rwl->lectores+=1;
  }
  else{
    nThread this_th = nSelf();
    nth_putBack(rwl->lectoresQueue, this_th);
    suspend(WAIT_RWLOCK);
    schedule();
  }
  
  END_CRITICAL
  return 1;
}

int nEnterWrite(nRWLock *rwl, int timeout) {
  START_CRITICAL
  
  //Caso de espera
  if(rwl->lectores==0 && rwl->escritores==0){
    rwl->escritores+=1;
  }
  else{
    nThread this_th = nSelf();
    nth_putBack(rwl->escritoresQueue, this_th);
    suspend(WAIT_RWLOCK);
    schedule();
  }
  END_CRITICAL
  return 1;
}

void nExitRead(nRWLock *rwl) {
  START_CRITICAL
  rwl->lectores--;
  if(rwl->lectores==0 && !nth_emptyQueue(rwl->escritoresQueue)){
    //se saca de la cola al escritor y se le manda a trabajar
    nThread first_writer = nth_getFront(rwl->escritoresQueue);
    rwl->escritores+=1;
    setReady(first_writer);
    schedule();
  }
  END_CRITICAL
}

void nExitWrite(nRWLock *rwl) {
  START_CRITICAL
  rwl->escritores--;
  if(!nth_emptyQueue(rwl->lectoresQueue)){
    while(!nth_emptyQueue(rwl->lectoresQueue)){
      nThread first_reader = nth_getFront(rwl->lectoresQueue);
      rwl->lectores+=1;
      setReady(first_reader);
      schedule();
    }
  }
  else if(!nth_emptyQueue(rwl->escritoresQueue)){
    //se acepta al escritor mas antiguo
    nThread first_writer = nth_getFront(rwl->escritoresQueue);
    rwl->escritores+=1;
    setReady(first_writer);
    schedule();
  }
  END_CRITICAL
}
