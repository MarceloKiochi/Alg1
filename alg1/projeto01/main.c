/*
	Marcelo Kiochi Hatanaka 10295645
	Trabalho 1 de Algotitimos e Estruturas de Dados - Calculadora
	Programa que le expressoes matematicas em cada linha e imprime o resultado.
	(Codigo com 2 niveis de TAD)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calculadora.h"

int le_linha(char** linha){ //le a expressao de uma linha
	int i=0;
	do{
		*linha = (char*)realloc(*linha, (i+1)*sizeof(char));
		scanf("%c", &(*linha)[i]);	
		
		i++;

	}while((*linha)[i-1]!=';' && (*linha)[i-1]!=',');

	(*linha)[i]='\0';


	if((*linha)[i-1]==';') return 1; //continua
	return 0; //para de ler
}

int main(){
	double res; //resultado
	int t;
	char* linha = NULL;
	int fim=0;	
	
	do{

		if(le_linha(&linha)==0) fim=1;

		t = strlen(linha);

		linha[t-1]='\0'; //tira o ; 

		getchar();					

		if(resolve_conta(linha, &res)==1) printf("%.2lf\n", res);
		//a funcao pega a expressao em uma string e muda a variavel do resultado
		//retorna 1 se concluiu com sucesso, ou 0 se houve erro

		else printf("Expressao incorreta\n");	
		

	}while(!fim);

	free(linha);
	
	return 0;
}