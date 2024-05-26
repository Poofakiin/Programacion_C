#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "groups.h"

int groups(unsigned int x) {
  int total=0;
  int mask=0;
  int size = (sizeof(x)*8);
  while(size>0){
    if(((x<<(size-1))>>((sizeof(x)*8)-1)) ^ mask){
      mask ^=1;
      total+=mask;
    }
    size--;
  }
  return total;
}
