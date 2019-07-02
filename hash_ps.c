#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_BUCKETS 1000
#define MULTIPLIER 31
#define MAX_LEN    100

typedef struct wordrec
{
  char* word;
  unsigned long count;
  struct wordrec* next;
}wordRecord;

/*
  @function walloc
  @desc     produces a new node
*/
wordRecord* walloc(const char* str)
{
  wordRecord* p=(wordRecord*)malloc(sizeof(wordRecord));
  if(p!=NULL)
  {
    p->count=0;
    p->word=strdup(str); /*creates a duplicate*/
    p->next=NULL;
  }
  return p;
}

/*hash bucket*/
wordRecord* table[MAX_BUCKETS]; /* changed from MAX_LEN -- dsw */

/*
  @function hashstring
  @desc     produces hash code for a string
            multipliers 31,35 have been found to work well
*/
unsigned long hashstring(const char* str)
{
  unsigned long hash=0;
  while(*str)
    {
      hash= hash*MULTIPLIER+*str;
      str++;
    }
  return hash%MAX_BUCKETS;
}

/*
  @function lookup
  @desc     returns a pointer to the word or creates
            it if required
*/
wordRecord* lookup(const char* str, int create)
{
  unsigned long hash = hashstring(str);/*starting point*/
  wordRecord* wp = table[hash];
  wordRecord* curr = NULL;

  // Check if the word is in the table
  for (curr = wp; curr != NULL; curr = curr->next)
  {
    if (strcmp(curr->word, str) == 0)
    return curr;
  }

  // If the word is not in the table, then add it
  // to the table. It is basically inserting a new node
  // in a linked list
  if (create)
  {
    wordRecord* newNode = walloc(str);
    if (wp == NULL)
    {
      curr = newNode;
      table[hash] = curr;
    }
    else
    {
      while (wp != NULL)
      {
        curr = wp;
        wp = wp->next;
      }
      curr->next = newNode;
      curr = curr->next;
    }
  }
  return curr;
}

/*
  @function cleartable()
  @desc     reclaims memory
*/
void cleartable()
{
  wordRecord* wp = NULL,*p = NULL;
  for (int i = 0; i < MAX_BUCKETS; i++)
  {
    wp = table[i];
    p = wp;
    while (wp != NULL)
    {
      wp = wp->next;
      free(p->word); //(WHY?)
      free(p);
      p = wp;
    }
  }
}

int main(int argc,char* argv[])
{
  FILE* fp = fopen("book.txt","r");
  char  word[1024]; /*big enough*/
  wordRecord* wp = NULL;
  int i = 0;

  memset(table,0,sizeof(table));
  /*read from input*/
  while(1)
  {
    if(fscanf(fp,"%s",word)!=1)
      break;
    wp = lookup(word, 1); /*create if doesn't exist*/
    wp->count++;
  }
  fclose(fp);

  /*
    print all words have frequency>100
   */
  for(i=0;i<MAX_BUCKETS;i++)
  {
    for(wp=table[i];wp!=NULL;wp=wp->next)
    {
      if(wp->count>1000)
      {
        printf("%s-->%ld\n",wp->word,wp->count);
      }
    }
  }
  cleartable();
  return 0;
}
