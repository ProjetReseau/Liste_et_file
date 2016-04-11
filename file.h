//déclaration de file

#ifndef __FILECLIENT__
#define __FILECLIENT__

#include <pthread.h>


typedef struct fifo_elmt{
  
  char message[TAILLE_MAX_MESSAGE+32];
  struct fifo_elmt *next;
  
} fifo_elmt;

typedef struct fifo_head{
  
  int nb_elmt;
  int sock;
  char pseudo[TAILLE_PSEUDO];
  fifo_elmt *tete;
  fifo_elmt *fin;
  pthread_mutex_t *mutex_fifo;
  
} fifo;

typedef struct liste_elmt{
  
  fifo *file;
  struct liste_elmt *preced;
  struct liste_elmt *suiv;
  
} liste_elmt;

typedef struct liste_head{
  
  int taille;
  liste_elmt *premier;
  pthread_mutex_t *mutex_liste;
  
} liste;

int ajouter_liste(liste* list,fifo* file);

liste_elmt* chercher_par_pseudo(liste* list,char* pseudo);

int supprimer_par_pseudo(liste* list,char* pseudo);



#define estVide_fifo(file) (file->nb_elmt==0)

fifo* creer_fifo(void);

int enfiler_fifo(fifo* file, char* data);

int defiler_fifo(fifo* file, char *emplacement);

void supprimer_fifo(fifo* file);



#endif
