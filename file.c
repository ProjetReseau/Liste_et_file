//implémentation de file.

#include "protocole.h"
#include "file.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define estVide_fifo(file) (file->nb_elmt==0)

fifo* creer_fifo(void){
  
  fifo *tete=malloc(sizeof(fifo));
  tete->mutex_fifo=malloc(sizeof(pthread_mutex_t));
  tete->nb_elmt=0;
  pthread_mutex_init(tete->mutex_fifo,NULL);
  
  return tete;
  
}



int enfiler_fifo(fifo* file, char* data){
  fifo_elmt *element=malloc(sizeof(fifo_elmt));
  strcpy(element->message,data);
  element->next=NULL;
  
  pthread_mutex_lock(file->mutex_fifo);
  
  if(estVide_fifo(file)){
    file->tete=element;
    file->fin=element;
    file->nb_elmt=1;
  }else{
    file->fin->next=element;
    file->fin=element;
    (file->nb_elmt)++;
  }
  
  pthread_mutex_unlock(file->mutex_fifo);
  
  return file->nb_elmt;
  
}

int defiler_fifo(fifo* file, char *emplacement){
  
  pthread_mutex_lock(file->mutex_fifo);
  
  if(estVide_fifo(file))
    return -1;
  
  fifo_elmt *element=file->tete;
  file->tete=element->next;
  (file->nb_elmt)--;
  if(estVide_fifo(file)){
    file->tete=NULL;
    file->fin=NULL;
  }
  
  strcpy(emplacement,element->message);
  free(element);
  
  pthread_mutex_unlock(file->mutex_fifo);
  
  
  return file->nb_elmt;
  
}


void supprimer_fifo(fifo* file){
  
  fifo_elmt *element;
  
  while(!(estVide_fifo(file))){
    element=file->tete;
    file->tete=element->next;
    (file->nb_elmt)--;
    free(element);
  }
  
  while(0!=pthread_mutex_destroy(file->mutex_fifo))
    sleep(1);
  
  free(file);
  
}


