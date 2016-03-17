//déclaration de file

#include <pthread.h>


typedef struct fifo_elmt{
  
  char message[TAILLE_MAX_MESSAGE+32];
  struct fifo_elmt *next;
  
} fifo_elmt;

typedef struct fifo_head{
  
  int nb_elmt;
  int sock;
  fifo_elmt *tete;
  fifo_elmt *fin;
  pthread_mutex_t *mutex_fifo;
  
} fifo;

#define estVide_fifo(file) (file->nb_elmt==0)

fifo* creer_fifo(void);

int enfiler_fifo(fifo* file, char* data);

int defiler_fifo(fifo* file, char *emplacement);

void supprimer_fifo(fifo* file);