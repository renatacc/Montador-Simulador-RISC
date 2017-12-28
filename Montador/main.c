/* -------------------------------
 * File:   main.c
 * Autora: Renata Caroline Cunha
 * Matrícula : 0022429
 * Data: 11/12/2016
---------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include "montador.h"

int main(int argc, char ** argv)
{
  if(argc == 3)
  {
    montador(argv[1],argv[2]);
    return 0;
  }
  else
  {
      printf("Erro!");
  }

  return(-1);
}
