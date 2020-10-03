/*
	Marcelo Kiochi Hatanaka
	10295645
	Trabalho 2 de Algotitmos e Estruturas de Dados - Dicionario
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist.h"
#define ERRO 0

int main(){

	char let;	/*letra para imprimir palavras*/
	int erro =0;	/*indica erro*/

	char* op = (char*)malloc(15*sizeof(char));	/*operacao*/

	char* word = (char*)malloc(60*sizeof(char));	/*verbete*/

	char* def = (char*)malloc(150*sizeof(char));	/*definicao*/

	skiplist* list = cria_lista();

	while(!feof(stdin)){

		erro =0;

		scanf("%s", op);

		if(strcmp(op, "insercao")==0){
			scanf("%s", word);
			getchar();
			scanf("%[^\n]s", def);

			if(insere(list, word, def) == ERRO) erro=1;	/*retorna ERRO quando a palavra repete*/
		}

		else if(strcmp(op, "alteracao")==0){
			scanf("%s", word);
			getchar();
			scanf("%[^\n]s", def);

			if(altera(list, word, def) == ERRO) erro=1;	/*retorna ERRO quando nao existe palavra*/
		}

		else if(strcmp(op, "remocao")==0){
			scanf("%s", word);

			if(remover(list, word) == ERRO) erro=1;	/*retora ERRO quando nao existe palavra*/
		}

		else if(strcmp(op, "busca")==0){
			scanf("%s", word);
			strcpy(def, buscadef(list, word));

			if(strcmp(def, "\0")==0) erro=1;	/*buscadef retorna "\0" se palavra nao existir*/

			else printf("%s %s\n", word, def);
		}

		else if(strcmp(op, "impressao")==0){
			scanf(" %c\n", &let);

			if(impressao(list, let) == ERRO) erro=-1;	/*retorna ERRO quando nao ha palavras com a inicial*/
		}

		if(erro==1) printf("OPERACAO INVALIDA\n");

		else if(erro==-1) printf("NAO HA PALAVRAS INCIADAS POR %c\n", let);
	}

	free(op);
	free(word);
	free(def);
	fri(list);

	return 0;
}