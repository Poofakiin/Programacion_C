#include <string.h>
#include <stdlib.h>

#include "desplazar.h"

void desplazar(char *str, int from, int nbytes, int by){
    char *aux=str;
    int len=strlen(str)-1;
    if(by>0){
        aux+=from-1+nbytes;
        str+=from+nbytes+by-1;
        for(int i=0;i<nbytes;i++){
            if(from+nbytes+by-1>len){
                by--;
            }
            else{
                *str=*aux;  
            }   
            str--;
            aux--;
        }
    }
    if(by<0){
        aux+=from;
        str+=from+by;
        for(int i=0;i<nbytes;i++){
            if(from+by<0){
                by++;
            }
            else{
                *str=*aux;
            }
            str++;
            aux++;
        }

    }   
}
char *desplazamiento(char *str, int from, int nbytes, int by){
    char *copy = malloc(strlen(str)+1);
    strcpy(copy,str);
    desplazar(copy, from, nbytes, by);
    return copy;
}
