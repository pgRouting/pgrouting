/*
 * Traveling Salesman Problem solution algorithm for PostgreSQL
 *
 * Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
                   
extern "C"
{
#include <gaul.h>
#include <postgres.h>
}

#include "tsp.h"

using namespace std;


// Maximal number of nodes in the path (to avoid infinite loops)
//#define MAX_TOWNS 40

float DISTANCE[MAX_TOWNS][MAX_TOWNS];
int ids[MAX_TOWNS];
int source_id;
int dest_id;
int cnum;

boolean tsp_score(population *pop, entity *entity)
{
  int           k;
  float         dist;
  int           cur_allele, prev_allele;
      
  entity->fitness = 0.0;
  dist = 0.0;

  // Loop over alleles in chromosome.
  for (k = 1; k < pop->len_chromosomes; k++)
    {
      cur_allele = ((int *)entity->chromosome[0])[k];
      prev_allele = ((int *)entity->chromosome[0])[k-1];
      dist += DISTANCE[cur_allele][prev_allele];
    }
                                  
  entity->fitness = 1/dist*100;
  if(ids[((int *)entity->chromosome[0])[0]] != source_id)
    entity->fitness /= 10;
      
  return TRUE;
}

boolean tspd_score(population *pop, entity *entity)
{
  int           k;
  float         dist;
  int           cur_allele, prev_allele;
      
  entity->fitness = 0.0;
  dist = 0.0;

  // Loop over alleles in chromosome.
  for (k = 1; k < pop->len_chromosomes; k++)
    {
      cur_allele = ((int *)entity->chromosome[0])[k];
      prev_allele = ((int *)entity->chromosome[0])[k-1];
      dist += DISTANCE[cur_allele][prev_allele];
    }
                                  
  entity->fitness = 1/dist*100; 
  if(ids[((int *)entity->chromosome[0])[0]] != source_id){
    entity->fitness /= 10;
  }  
  //ensure last node is dest  
  if(ids[((int *)entity->chromosome[0])[pop->len_chromosomes-1]] != dest_id){
    entity->fitness /= 10; 
  }   
      
  return TRUE;
}
                                            
boolean tsp_seed(population *pop, entity *adam)
{
  int           i,s,tmp;
  int           *data;
    
  data = (int *)adam->chromosome[0];
      
  for (i=0; i<pop->len_chromosomes; i++)
  {
    data[i] = i;
  }
		      
  for (i=0; i<pop->len_chromosomes; i++)
  {
    if(ids[data[i]] == source_id)
      s = i;
  }
					      
  tmp = data[0];
  data[0] = data[s];
  data[s] = tmp;
						    
  return TRUE;
}

boolean tspd_seed(population *pop, entity *adam)
{
  int           i,s,t,tmp;
  int           *data;
    
  data = (int *)adam->chromosome[0];
      
  for (i=0; i<pop->len_chromosomes; i++)
  {
    data[i] = i;
  }
		      
  for (i=0; i<pop->len_chromosomes; i++)
  {
    if(ids[data[i]] == source_id)
      s = i;
    if(ids[data[i]] == dest_id)
      t = i; 
  }
	
  //swap start to top				      
  tmp = data[0];
  data[0] = data[s];
  data[s] = tmp;
  
  //swap dest to end
  tmp = data[pop->len_chromosomes-1];
  data[pop->len_chromosomes-1] = data[t];
  data[t] = tmp;
						    
  return TRUE;
}
						      
void tsp_mutate_swap(population *pop, entity *mother, entity *son)
{
  int           i, j; 
  int           tmp;  
          
  // Copy chromosomes from parent to offspring.
  memcpy( son->chromosome[0],
          mother->chromosome[0],
          pop->len_chromosomes*sizeof(int) );
                        
  do
    {
      i = random_int(pop->len_chromosomes-1);
    } 
  while(i==0);

  do
    {
      j = random_int(pop->len_chromosomes-1);
    } 
  while(j==0);
                                    
  if (i==j)
    {
      if (j==9)
        j=1;
      else
        j++;
    }
                                                                          
  tmp = ((int *)son->chromosome[0])[i];
  ((int *)son->chromosome[0])[i] = ((int *)son->chromosome[0])[j];
  ((int *)son->chromosome[0])[j] = tmp;
                                                    
  return;
}

void tsp_mutate_shift(population *pop, entity *mother, entity *son)
{
  int i, j, k;        // Team members.
  int tmp;            // For swapping i and j.
          
  // Copy chromosomes from parent to offspring.
  memcpy( son->chromosome[0],
          mother->chromosome[0],
          pop->len_chromosomes*sizeof(int) );
                                
                  
  i = random_int(pop->len_chromosomes-1);

  do
    {
      j = random_int(pop->len_chromosomes-1);
    } 
  while(i==j);
                                                    
  if (i>j)
    {
      tmp = ((int *)son->chromosome[0])[j];
      for (k=j; k<i; k++)
        {
          ((int *)son->chromosome[0])[k] = ((int *)son->chromosome[0])[k+1];
        }
      ((int *)son->chromosome[0])[i] = tmp;
    }
  else
    {
      tmp = ((int *)son->chromosome[0])[j];
      for (k=j; k>i; k--)
        {
          ((int *)son->chromosome[0])[k] = ((int *)son->chromosome[0])[k-1];
        }
      ((int *)son->chromosome[0])[i] = tmp;
    }

  return;
}                                                     

void tsp_mutate(population *pop, entity *mother, entity *son)
{
  
  if (!mother || !son) die("Null pointer to entity structure passed");
          
  if (random_boolean_prob(0.2))
    tsp_mutate_swap(pop, mother, son);
  else
    tsp_mutate_shift(pop, mother, son);
                          
  return;
}


void tsp_crossover(population *pop, entity *mother, 
                   entity *father, entity *daughter, entity *son)
{
  int i, j; 
      
  for (i=0; i<pop->len_chromosomes; i++)
    {
      if (random_boolean())
        {
          ((int *)son->chromosome[0])[i] = 
            ((int *)father->chromosome[0])[i];
          ((int *)daughter->chromosome[0])[i] = 
            ((int *)mother->chromosome[0])[i];
        }
      else
        {
          ((int *)son->chromosome[0])[i] = 
            ((int *)father->chromosome[0])[i];

          ((int *)daughter->chromosome[0])[i] = 
            ((int *)mother->chromosome[0])[i];
        }
    }

  for (i=1; i<pop->len_chromosomes; i++)
    {
      for (j=0; j<i; j++)
        {
          if (((int *)son->chromosome[0])[j] == 
                ((int *)son->chromosome[0])[i])
            {
              if (((int *)son->chromosome[0])[i]==9)
                ((int *)son->chromosome[0])[i]=0;
              else
                ((int *)son->chromosome[0])[i]++;
              j=0;
            }
        }
      for (j=0; j<i; j++)
        {
          if (((int *)daughter->chromosome[0])[j] == 
                ((int *)daughter->chromosome[0])[i])
            {
              if (((int *)daughter->chromosome[0])[i]==9)
                ((int *)daughter->chromosome[0])[i]=0;
              else
                ((int *)daughter->chromosome[0])[i]++;
              j=0;
            }
        }
    }
  return;
}


int 
find_tsp_solution(int num, float dist[MAX_TOWNS][MAX_TOWNS], 
                  int p_ids[MAX_TOWNS], int source, 
                  float *fit, char* err_msg)
{
  int i,j;
  population    *pop=NULL;              /* Population of solutions. */
  float         score = 0.0;            /* Best score */

  source_id = source;
  cnum=num;
  
  for(i=0;i<cnum;i++)
    {
      ids[i] = p_ids[i];

      for(j=0;j<cnum;j++)
        {
          DISTANCE[i][j]=dist[i][j];
        }
    }
  
  random_init();

  for (int ss=0; ss<15; ss++) //use seed 15 times
    {
      if (pop) ga_extinction(pop);
      random_seed(ss);
      pop = ga_genesis_integer(
                num*4,  /* const int              population_size */
                1,      /* const int              num_chromo */
                cnum,   /* const int              len_chromo */
                NULL,   /* GAgeneration_hook      generation_hook */
                NULL,   /* GAiteration_hook       iteration_hook */
                NULL,   /* GAdata_destructor      data_destructor */
                NULL,   /* GAdata_ref_incrementor data_ref_incrementor */
                tsp_score,/* GAevaluate           evaluate */
                tsp_seed, /* GAseed               seed */
                NULL,     /* GAadapt              adapt */
                ga_select_one_randomrank,/* GAselect_one     select_one */
                ga_select_two_randomrank,/* GAselect_two     select_two */
                tsp_mutate,    /* GAmutate        mutate */
                tsp_crossover, /* GAcrossover     crossover */
                NULL,          /* GAreplace       replace */
                NULL           /* vpointer        User data */
                );

      ga_population_set_parameters(
               pop,                     /* population      *pop */
               GA_SCHEME_DARWIN,        /* const ga_scheme_type     scheme */
               GA_ELITISM_PARENTS_DIE,  /* const ga_elitism_type   elitism */
               0.5,                     /* optimal double  crossover */
               0.4,                     /* optimal double  mutation */
               0.0                      /* unused  double  migration */
               );

      ga_evolution(
              pop,              /* population  *pop */
              num*4             /* const int   max_generations */
              );

      
      if(score < ga_get_entity_from_rank(pop,0)->fitness)
        {
          score = ga_get_entity_from_rank(pop,0)->fitness;
          *fit = score;
      
          for(int l=0; l<cnum; l++)
            { 
              p_ids[l] = ids[
                             ((int *)ga_get_entity_from_rank(pop,0)->
                              chromosome[0])[l]];
            }
        }
      
    }  
  
  return EXIT_SUCCESS;
}

int 
find_tspd_solution(int num, float dist[MAX_TOWNS][MAX_TOWNS], 
                  int p_ids[MAX_TOWNS], int source, int dest,
                  float *fit, char* err_msg)
{
  int i,j;
  population    *pop=NULL;              /* Population of solutions. */
  float         score = 0.0;            /* Best score */

  source_id = source;
  dest_id = dest;
  cnum=num;
  
  for(i=0;i<cnum;i++)
    {
      ids[i] = p_ids[i];

      for(j=0;j<cnum;j++)
        {
          DISTANCE[i][j]=dist[i][j];
        }
    }
  
  random_init();

  for (int ss=0; ss<15; ss++) //use seed 15 times
    {
      if (pop) ga_extinction(pop);
      random_seed(ss);
      pop = ga_genesis_integer(
                num*4,  /* const int              population_size */
                1,      /* const int              num_chromo */
                cnum,   /* const int              len_chromo */
                NULL,   /* GAgeneration_hook      generation_hook */
                NULL,   /* GAiteration_hook       iteration_hook */
                NULL,   /* GAdata_destructor      data_destructor */
                NULL,   /* GAdata_ref_incrementor data_ref_incrementor */
                tspd_score,/* GAevaluate           evaluate */
                tspd_seed, /* GAseed               seed */
                NULL,     /* GAadapt              adapt */
                ga_select_one_randomrank,/* GAselect_one     select_one */
                ga_select_two_randomrank,/* GAselect_two     select_two */
                tsp_mutate,    /* GAmutate        mutate */
                tsp_crossover, /* GAcrossover     crossover */
                NULL,          /* GAreplace       replace */
                NULL           /* vpointer        User data */
                );

      ga_population_set_parameters(
               pop,                     /* population      *pop */
               GA_SCHEME_DARWIN,        /* const ga_scheme_type     scheme */
               GA_ELITISM_PARENTS_DIE,  /* const ga_elitism_type   elitism */
               0.5,                     /* optimal double  crossover */
               0.4,                     /* optimal double  mutation */
               0.0                      /* unused  double  migration */
               );

      ga_evolution(
              pop,              /* population  *pop */
              num*4             /* const int   max_generations */
              );

      
      if(score < ga_get_entity_from_rank(pop,0)->fitness)
        {
          score = ga_get_entity_from_rank(pop,0)->fitness;
          *fit = score;
      
          for(int l=0; l<cnum; l++)
            { 
              p_ids[l] = ids[
                             ((int *)ga_get_entity_from_rank(pop,0)->
                              chromosome[0])[l]];
            }
        }
      
    }  
  
  return EXIT_SUCCESS;
}
