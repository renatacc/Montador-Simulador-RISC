/* -------------------------------
 * File:   montador.c
 * Autora: Renata Caroline Cunha
 * Matrícula : 0022429
 * Data: 11/12/2016
---------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "montador.h"

//#define ADDRESS 17 /*Para compilar gcc -Wall *.c -DADDRESS="ADDRESS" -o exe */

/* --- Estrutura label --- */
struct label
{
    int endMemoria;
    char nome[50];
};
/*-------------------------*/

/* ------------- Declaração das funções ------------- */
void leInstrucao(char *arqEntrada, char *arqSaida);
void pegaOpcode(char *instrucao, char *opcode, int *fim);
void converteInstrucao(char *instrucao, char *arqSaida);
void itoa(int numero, char *numBinario, int numBits);
int verificaLabel(char *instrucao);
void geraSaida(char *arqSaida, char *instrucaoSaida);
void insereLabel(char *label) ;
/* -------------------------------------------------- */

/* ----- Declaração das variáveis globais ----*/
char memoria[65536][50];
int contMemoria=0,contLabel=0,PC=0;
struct label VetorLabel[65536];
/* ------------------------------------------ */

void montador(char *arqEntrada, char *arqSaida)
{
   leInstrucao(arqEntrada, arqSaida); /* Lê instruções do arquivo de entrada */

   return;
}

void itoa(int numero, char *numBinario, int numBits) /* Converte um número decimal para binário */
{
   int resultDeslocamento, i;

   numBinario[0] = '\0';
   numBits--;

   for(i=numBits; i>=0; i--)
   {
      resultDeslocamento = numero >> i;

      if(resultDeslocamento & 1)
      {
         strcat(numBinario,"1");
      }
      else
      {
        strcat(numBinario,"0");
      }
   }

   numBinario[i] = '\0';

   return;
}

void pegaOpcode(char *instrucao, char *opcode, int *fim) /* Pega apenas o opcode da instrução */
{
   int i;

   for(i=0;isalpha(instrucao[i]);i++)
   {
      opcode[i] = instrucao[i];
   }

   opcode[i]= '\0';
   *fim=i+1;

   return;
}

int verificaLabel(char *instrucao)  /* Verifica se a instrução é um label */
{
   if(instrucao[strlen(instrucao)-1] == ':')
   {
     return(1);
   }

   return(0);
}

void insereLabel(char *label) /* Insere o label no vetor de labels */
{
    strcat(VetorLabel[contLabel].nome,label);
    VetorLabel[contLabel].endMemoria=contMemoria;
    contLabel++;

    return;
}

int procuraMemoriaLabel(char *label) /* Procura o label e retorna a posição na memória */
{
    int i;

    for(i=0; i<contLabel; i++)
    {
       if(strcmp(label,VetorLabel[i].nome) == 0)
       {
           return(VetorLabel[i].endMemoria);
       }
    }

    return(-1);
}

void geraSaida(char *arqSaida, char *instrucaoSaida) /* Gera arquivo de saída com as instruções em binário */
{
    int i;
    FILE *arquivoSaida;
    char instrucao[500];

    arquivoSaida=fopen(arqSaida,"wt");

    strcat(instrucaoSaida, "address ");
    itoa(ADDRESS,instrucao, 16);
    strcat(instrucaoSaida, instrucao);

    fprintf(arquivoSaida,"%s\n",instrucaoSaida);

    for(i=0;i<contMemoria;i++)
    {
        converteInstrucao(memoria[i], instrucaoSaida); /* Converte a instrução para binário */
        fprintf(arquivoSaida,"%s\n",instrucaoSaida);
    }

    fclose(arquivoSaida);

    return;
}

void leInstrucao(char *arqEntrada, char *arqSaida) /* Lê as instruções do arquivo de entrada */
{
  	FILE *arquivoEntrada;
		char instrucao[500], instrucaoSaida[1000];

    instrucaoSaida[0]='\0';

		arquivoEntrada=fopen(arqEntrada,"r");

	  while(fgets(instrucao, sizeof(instrucao),arquivoEntrada)) /* Carrega instrucoes na memória */
	  {
         instrucao[strlen(instrucao)-1]='\0';

         if(verificaLabel(instrucao) == 0) /* Se não for um label */
         {
            strcat(memoria[contMemoria], instrucao); /* Guarda a instrução na memória */
            contMemoria++;
         }
         else /* Se for um label */
         {
            instrucao[strlen(instrucao)-1]='\0'; /* Tira o ":" */
            insereLabel(instrucao); /* Insere o label e a posição de memória no vetor de labels */
         }
	  }

    fclose(arquivoEntrada);

    geraSaida(arqSaida,instrucaoSaida); /* Gera arquivo de saída */

    return;
}

void converteInstrucao(char *instrucao, char *instrucaoSaida) /* Converte a instrução em assembly para binário */
{
    char opcode[15], instrucaoBinario[1000], label[50];
    int numeroOpcode, fim, rc, rb, ra,const16, end;

    instrucaoSaida[0] ='\0';

    pegaOpcode(instrucao,opcode, &fim); /* Pega apenas o opcode da instrução */

    /* numeroOpcode recebe o número que correspondente ao opcode */
    if(!strcmp(opcode, "nop"))
    {
       numeroOpcode = 0;
    }
    if(!strcmp(opcode, "add"))
    {
       numeroOpcode = 1;
    }
    if(!strcmp(opcode, "sub"))
    {
       numeroOpcode = 2;
    }
    if(!strcmp(opcode, "zeros"))
    {
       numeroOpcode = 3;
    }
    if(!strcmp(opcode, "xor"))
    {
       numeroOpcode = 4;
    }
    if(!strcmp(opcode, "or"))
    {
       numeroOpcode = 5;
    }
    if(!strcmp(opcode, "not"))
    {
       numeroOpcode = 6;
    }
    if(!strcmp(opcode, "and"))
    {
       numeroOpcode = 7;
    }
    if(!strcmp(opcode, "asl"))
    {
       numeroOpcode = 8;
    }
    if(!strcmp(opcode, "asr"))
    {
       numeroOpcode = 9;
    }
    if(!strcmp(opcode, "lsl"))
    {
       numeroOpcode = 10;
    }
    if(!strcmp(opcode, "lsr"))
    {
       numeroOpcode = 11;
    }
    if(!strcmp(opcode, "passa"))
    {
       numeroOpcode = 12;
    }
    if(!strcmp(opcode, "lch"))
    {
       numeroOpcode = 14;
    }
    if(!strcmp(opcode, "lcl"))
    {
       numeroOpcode = 15;
    }
    if(!strcmp(opcode, "load"))
    {
       numeroOpcode = 16;
    }
    if(!strcmp(opcode, "store"))
    {
       numeroOpcode = 17;
    }
    if(!strcmp(opcode, "mult"))
    {
       numeroOpcode = 18;
    }
    if(!strcmp(opcode, "div"))
    {
       numeroOpcode = 19;
    }
    if(!strcmp(opcode, "pot"))
    {
       numeroOpcode = 20;
    }
    if(!strcmp(opcode, "mod"))
    {
       numeroOpcode = 21;
    }
    if(!strcmp(opcode, "addi"))
    {
       numeroOpcode = 22;
    }
    if(!strcmp(opcode, "inc"))
    {
       numeroOpcode = 23;
    }
    if(!strcmp(opcode, "dec"))
    {
       numeroOpcode = 24;
    }
    if(!strcmp(opcode, "li"))
    {
       numeroOpcode = 25;
    }
    if(!strcmp(opcode, "bgt"))
    {
       numeroOpcode = 26;
    }
    if(!strcmp(opcode, "jal"))
    {
       numeroOpcode = 32;
    }
    if(!strcmp(opcode, "jr"))
    {
       numeroOpcode = 33;
    }
    if(!strcmp(opcode, "beq"))
    {
       numeroOpcode = 34;
    }
    if(!strcmp(opcode, "bne"))
    {
       numeroOpcode = 35;
    }
    if(!strcmp(opcode, "j"))
    {
       numeroOpcode = 36;
    }
    if(!strcmp(opcode, "halt"))
    {
       numeroOpcode = 255;
    }

    /*  Converte número do opcode para binário */
    itoa(numeroOpcode, instrucaoBinario,8);
    strcat(instrucaoSaida, instrucaoBinario);

    /* Converte o restante da instrução para binário */
  	switch(numeroOpcode)
  	{
  					 case 0: //NOP
                itoa(numeroOpcode, instrucaoBinario,24);
                strcat(instrucaoSaida, instrucaoBinario);
  					 break;
  					 case 1: //ADD
                sscanf(instrucao, "add r%d,r%d,r%d", &rc,&ra,&rb);
                itoa(ra, instrucaoBinario,8);
                strcat(instrucaoSaida, instrucaoBinario);
                itoa(rb, instrucaoBinario,8);
                strcat(instrucaoSaida, instrucaoBinario);
                itoa(rc, instrucaoBinario,8);
                strcat(instrucaoSaida, instrucaoBinario);
  					 break;
  					 case 2: //SUB
                 sscanf(instrucao, "sub r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra, instrucaoBinario,8);
                 strcat(instrucaoSaida, instrucaoBinario);
                 itoa(rb, instrucaoBinario,8);
                 strcat(instrucaoSaida, instrucaoBinario);
                 itoa(rc, instrucaoBinario,8);
                 strcat(instrucaoSaida, instrucaoBinario);
  					 break;
  					 case 3: //ZERA
                 sscanf(instrucao, "zeros r%d", &rc);
                 itoa(rc,instrucaoBinario,24);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 4: //XOR
                 sscanf(instrucao, "xor r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 5: //OR
                 sscanf(instrucao, "or r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 6: //NOT
                 sscanf(instrucao, "or r%d,r%d", &rc,&ra);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,16);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 7: //AND
                 sscanf(instrucao, "and r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 8: //SHIFT ARIT ESQUERDA
                 sscanf(instrucao, "asl r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 9: //SHIFT ARIT DIREITA
                 sscanf(instrucao, "asr r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 10: //SHIFT LOGICO ESQUERDA
               sscanf(instrucao, "lsl r%d,r%d,r%d", &rc,&ra,&rb);
               itoa(ra,instrucaoBinario,8);
               strcat(instrucaoSaida,instrucaoBinario);
               itoa(rb,instrucaoBinario,8);
               strcat(instrucaoSaida,instrucaoBinario);
               itoa(rc,instrucaoBinario,8);
               strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 11: //SHIFT LOGICO DIREITA
               sscanf(instrucao, "lsr r%d,r%d,r%d", &rc,&ra,&rb);
               itoa(ra,instrucaoBinario,8);
               strcat(instrucaoSaida,instrucaoBinario);
               itoa(rb,instrucaoBinario,8);
               strcat(instrucaoSaida,instrucaoBinario);
               itoa(rc,instrucaoBinario,8);
               strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 12: //PASSA
                 sscanf(instrucao, "passa r%d,r%d", &rc,&ra);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,16);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 14: //LOAD CONSTANTE MAIS SIGNIFICATIVOS
                 sscanf(instrucao, "lch r%d,%d", &rc,&const16);
                 itoa(const16,instrucaoBinario,16);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 15: //LOAD CONSTANTE MENOS SIGNIFICATIVOS
                 sscanf(instrucao, "lcl r%d,%d", &rc,&const16);
                 itoa(const16,instrucaoBinario,16);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 16: //LOAD
                 sscanf(instrucao, "load r%d,%d", &rc,&ra);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,16);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 17: //STORE
                 sscanf(instrucao, "store r%d,r%d", &rc,&ra);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,16);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 18: //MULT
                 sscanf(instrucao, "mult r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 19: //DIV
                 sscanf(instrucao, "div r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
             case 20: //pot
                 sscanf(instrucao, "pot r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
             case 21: //mod
                 sscanf(instrucao, "mod r%d,r%d,r%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
             case 22: //addi
                 sscanf(instrucao, "addi r%d,r%d,%d", &rc,&ra,&rb);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rb,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
             case 23: //inc
                 sscanf(instrucao, "inc r%d,r%d", &rc,&ra);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,16);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
             case 24: //dec
                 sscanf(instrucao, "dec r%d,r%d", &rc,&ra);
                 itoa(ra,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,16);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
             case 25: //li
                 sscanf(instrucao, "li r%d,%d", &rc,&const16);
                 itoa(const16,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(0,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
                 itoa(rc,instrucaoBinario,8);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
             case 26: //bgt
                 sscanf(instrucao, "bgt r%d,r%d,%s", &ra,&rb,label);
                 end = procuraMemoriaLabel(label);

                 if(end == -1)
                 {
                    exit(0);
                 }
                 else
                 {
                   itoa(ra,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                   itoa(rb,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                   itoa(end,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                 }
  					 break;
  					 case 32: //JUMP AND LINK
                 sscanf(instrucao, "jal %s", label);
                 end = procuraMemoriaLabel(label);

                 if(end == -1)
                 {
                    exit(0); //Label nao encontrado
                 }
                 else
                 {
                   itoa(end,instrucaoBinario,24);
                   strcat(instrucaoSaida,instrucaoBinario);
                 }
  					 break;
  					 case 33: //JUMP REGISTER
               sscanf(instrucao, "jr r%d", &rc);
               itoa(rc,instrucaoBinario,24);
               strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  					 case 34: //JUMP SE IGUAL
                 sscanf(instrucao, "beq r%d,r%d,%s", &ra,&rb,label);
                 end = procuraMemoriaLabel(label);

                 if(end == -1)
                 {
                    exit(0);
                 }
                 else
                 {
                   itoa(ra,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                   itoa(rb,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                   itoa(end,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                 }
  					 break;
  					 case 35: //JUMP SE DIFERENTE
                 sscanf(instrucao, "bne r%d,r%d,%s", &ra,&rb,label);
                 end = procuraMemoriaLabel(label);

                 if(end == -1)
                 {
                    exit(0);
                 }
                 else
                 {
                   itoa(ra,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                   itoa(rb,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                   itoa(end,instrucaoBinario,8);
                   strcat(instrucaoSaida,instrucaoBinario);
                 }
  					 break;
  					 case 36: //JUMP INCONDICIONAL
                 sscanf(instrucao, "j %s", label);
                 end = procuraMemoriaLabel(label);

                 if(end == -1)
                 {
                    exit(0);
                 }
                 else
                 {
                   itoa(end,instrucaoBinario,24);
                   strcat(instrucaoSaida,instrucaoBinario);
                 }
  					 break;
  					 case 255:
                 strcat(instrucaoSaida,instrucaoBinario);
                 strcat(instrucaoSaida,instrucaoBinario);
                 strcat(instrucaoSaida,instrucaoBinario);
  					 break;
  		}

    return;
}
