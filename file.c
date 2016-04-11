//implémentation de file.

#include "protocole.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define estVide_fifo(file) (file->nb_elmt==0)

fifo* creer_fifo(void){
  
  fifo *tete=malloc(sizeof(fifo));
  tete->mutex_fifo=malloc(sizeof(pthread_mutex_t));
  tete->nb_elmt=0;
  pthread_mutex_init(tete->mutex_fifo,NULL);
  sprintf(tete->pseudo,"?");
  
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

int ajouter_liste(liste* list,fifo* file){
  
  pthread_mutex_lock(list->mutex_liste);
  
  liste_elmt* element=malloc(sizeof(liste_elmt));
  element->file=file;
  element->preced=NULL;
  element->suiv=list->premier;
  if(list->premier!=NULL)
    list->premier->preced=element;
  list->premier=element;
  list->taille++;
  
  pthread_mutex_unlock(list->mutex_liste);

}



int rechercher_par_pseudo(liste* list,char* pseudo, fifo** resultat){
  
    int i;
  liste_elmt* result=list->premier;
  
  for(i=0;i<list->taille;i++){
    if(0==strcmp(pseudo,result->file->pseudo)){
      
      *resultat=result->file;
      
      return 1;
    }
    result=result->suiv;
  }
  return 0;

  
}



int supprimer_par_pseudo(liste* list,char* pseudo){
  
  int i;
  liste_elmt* result=list->premier;
  
  for(i=0;i<list->taille;i++){
    if(0==strcmp(pseudo,result->file->pseudo)){
      
        pthread_mutex_lock(list->mutex_liste);
      
      if(result->suiv!=NULL)
	result->suiv->preced=result->preced;
      if(result->preced!=NULL)
	result->preced->suiv=result->suiv;
      else
	list->premier=result->suiv;
      list->taille--;
      free(result);
      
      pthread_mutex_unlock(list->mutex_liste);
      
      return 1;
    }
    result=result->suiv;
  }
  return 0;
    
}


