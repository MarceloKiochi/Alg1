#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist.h"
#include <assert.h>
#define ERRO 0
#define OK 1

struct pala{	/*elementos da lista*/ 
	char* nome;			/*verbete*/
	char* def;			/*definicao*/
	palavra* p_hor;		/*proximo elemento na horizontal*/
	palavra* p_vert;
	palavra* a_hor;											
	palavra* a_vert;	/*elemento anterior na vertical*/
};

struct skip{	/*lista*/
	int niveis;			/*numero de niveis da lista*/
	palavra* inicio;	/*elemento inicial auxiliar*/
};

skiplist* cria_lista(){

	skiplist* aux = (skiplist*)malloc(sizeof(skiplist));

	aux->niveis=0;

	aux->inicio = (palavra*)malloc(sizeof(palavra));

	aux->inicio->nome = (char*)malloc(3*sizeof(char));

	aux->inicio->def=NULL;

	strcpy(aux->inicio->nome, "A");	/*define como "A" para ser sempre o primeiro elemento da lista*/

	aux->inicio->p_hor=NULL;
	aux->inicio->p_vert=NULL;
	aux->inicio->a_hor=NULL;
	aux->inicio->a_vert=NULL;

	return aux;
}

void cria_novo_nivel(skiplist* l, palavra* novo, palavra* nivel[], int n){	/*copia um novo elemento no nivel acima do nivel passado (n)*/

	palavra* p;
	palavra* aux;

	if(l==NULL || novo==NULL || nivel==NULL) return;

	/*cria o elemento e copia as informacoes*/
	aux = (palavra*)malloc(sizeof(palavra));		
	aux->nome = (char*)malloc(60*sizeof(char));

	strcpy(aux->nome, novo->nome);
	
	aux->def = (char*)malloc(150*sizeof(char));
	strcpy(aux->def, novo->def); 

	aux->a_vert = NULL;

	aux->p_vert = novo;

	novo->a_vert = aux;
	

	if(l->niveis > n){	/*se ja existir esse nivel, so adiciona o elemento*/

		if((nivel[n+1])->p_hor==NULL){
			(nivel[n+1])->p_hor = aux;
			aux->p_hor = NULL;
			aux->a_hor = nivel[n+1];
		}

		else{
			aux->p_hor = (nivel[n+1])->p_hor;
			aux->p_hor->a_hor = aux;
			(nivel[n+1])->p_hor = aux;
			aux->a_hor = nivel[n+1];
		}
	}

	else{	/*se nao existir, cria novo nivel e insere o elemento*/
		p = (palavra*)malloc(sizeof(palavra));

		p->nome = (char*)malloc(3*sizeof(char));
		strcpy(p->nome, l->inicio->nome);

		p->def=NULL;
		p->a_hor = NULL;
		p->a_vert = NULL;

		p->p_vert = l->inicio;

		l->inicio = p;

		p->p_hor = aux;

		aux->a_hor = p;

		l->niveis++;
	}

	if(rand()%2) cria_novo_nivel(l, aux, nivel, n+1); /*sorteia se vai adicionar o novo elemento ao outro nivel */
}

int insere(skiplist* l, char* word, char* def){ /*insere novo elemento no ultimo nivel (mais baixo)*/

	int i;
	int t;
	
	palavra* aux;
	
	palavra** nivel;	/*vetor que guardara posicoes a serem ligadas ao novo elemento*/

	palavra* novo;

	if(l==NULL || word==NULL || def==NULL) return ERRO;

	novo = (palavra*)malloc(sizeof(palavra));
	
	t = l->niveis;

	aux = l->inicio;

	nivel = (palavra**)malloc((t+1)*sizeof(palavra*));

	/*inicia o elemento*/
	novo->nome = (char*)malloc(60*sizeof(char));
	strcpy(novo->nome, word);
	
	novo->def = (char*)malloc(150*sizeof(char));
	strcpy(novo->def, def);

	novo->p_hor=NULL;
	novo->p_vert=NULL;
	novo->a_hor=NULL;
	novo->a_vert=NULL;	
	
	while(aux->p_vert!=NULL || (aux->p_hor!=NULL && strcmp(word, aux->p_hor->nome)>=0)){ /*percorre a lista para ser inserido ordenado*/

		if(aux->p_hor!=NULL && strcmp(word, aux->p_hor->nome)>=0){	/*se o novo for igual ou depois da proxima posicao horizontal*/
			aux = aux->p_hor;										/*passa para essa posicao*/
		}

		else if(aux->p_vert!=NULL){	/*se nao, desce para um nivel com mais elementos*/
			nivel[t] = aux;			/* <-essa posicao sera linkada ao novo elemento se ele subir ate o nivel dela*/
			t--;
			aux = aux->p_vert;
		}
	}

	if(strcmp(aux->nome, word)==0){
		for(i=0;i<t;i++){
			free(nivel[i]);
		}
		free(nivel);

		free(novo->nome);
		free(novo->def);
		free(novo);

		return ERRO;
	}

	/*arruma os ponteiros da lista*/
	if(aux->p_hor == NULL){
		novo->p_hor = NULL;
	}

	else{
		novo->p_hor = aux->p_hor;

		aux->p_hor->a_hor = novo;
	}

	aux->p_hor = novo;

	novo->a_hor = aux;	

	if(rand()%2) cria_novo_nivel(l, novo, nivel, 0); /*sorteia se ira subir para outros niveis*/

	for(i=0;i<t;i++){
		free(nivel[i]);
	}
	free(nivel);

	return OK;
	
}

int impressao(skiplist* l, char let){	/*imprime as palavras comecadas pela letra passada*/

	palavra* aux;

	char p[2];
	int sim = 0;

	if(l==NULL) return ERRO;

	aux = l->inicio;

	p[0] = let;
	p[1] = '\0'; /*transforma em string para facilitar*/
				/*a string da letra ficara uma posicao anterior a primeira palavra comecada por ela*/

	while(aux->p_vert!=NULL || (aux->p_hor!=NULL && strcmp(p, aux->p_hor->nome)>0)){

		if(aux->p_hor!=NULL && strcmp(p, aux->p_hor->nome)>0){
			aux = aux->p_hor;
		}

		else if(aux->p_vert!=NULL){			
			aux = aux->p_vert;
		}
	}

	while(aux->p_hor!=NULL && aux->p_hor->nome[0] == p[0]){ /*imprime as palavras*/

		aux = aux->p_hor;

		printf("%s %s\n", aux->nome, aux->def);

		sim =1; /*indica se imprimiu alguma*/
	}

	return sim;
}

void fri(skiplist* l){	/*destroi a lista*/

	palavra* aux;
	palavra* p;	

	if(l==NULL) return;	
		
	while(l->inicio != NULL){	/*esse loop passa pelos elementos auxiliares*/

		aux = l->inicio;
		l->inicio = l->inicio->p_vert;

		while(aux!=NULL){		/*da free em um nivel*/		

			if(aux->nome != NULL) free(aux->nome);

			if(aux->def != NULL) free(aux->def);

			p = aux;
			aux = aux->p_hor;

			free(p);

			p=NULL;
		}
	}

	l->inicio = NULL;
	free(l);
	l = NULL;
}

palavra* busca(skiplist* l, char* word){	/*verifica se um elemento esta presente na lista*/

	palavra* aux;

	if(l==NULL || word==NULL) return NULL;

	aux = l->inicio;

	while(aux->p_vert!=NULL || (aux->p_hor!=NULL && strcmp(word, aux->p_hor->nome)>=0)){

		if(aux->p_hor!=NULL && strcmp(word, aux->p_hor->nome)==0) return aux->p_hor;	/*se achar, retorna o endereco do maior nivel*/
		
		else if(aux->p_hor!=NULL && strcmp(word, aux->p_hor->nome)>0){
			aux = aux->p_hor;
		}

		else if(aux->p_vert!=NULL){			
			aux = aux->p_vert;
		}
	}

	return NULL;
}

char* buscadef(skiplist* l, char* word){	/*retorna a definicao de um elemento*/

	palavra* aux;

	if(l==NULL || word==NULL) return "\0";

	aux = busca(l, word);

	if(aux!=NULL) return aux->def;
	
	return "\0";

}

int altera(skiplist* l, char* word, char* def){	/*altera a definicao*/

	palavra* aux;

	if(l==NULL || word==NULL || def==NULL) return ERRO;

	aux = busca(l, word);	/*pega o endereco do maior nivel*/
	
	if(aux!=NULL){

		while(aux!=NULL){	/*altera de todos os niveis*/

			strcpy(aux->def, def);

			aux = aux->p_vert;
		}	

		return OK;
	}

	return ERRO;
}

int remover(skiplist* l, char* word){	/*remove um elemento*/

	palavra* aux;

	palavra* p;

	if(l==NULL || word==NULL) return ERRO;

	aux = busca(l, word);	/*pega o endereco do maior nivel*/

	if(aux==NULL) return ERRO;

	while(aux!=NULL){

		free(aux->nome);

		free(aux->def);

		if(aux->p_hor!=NULL){	/*arruma os ponteiros*/

			aux->a_hor->p_hor = aux->p_hor;
			aux->p_hor->a_hor = aux->a_hor;			
		}

		else{
			aux->a_hor->p_hor = NULL;
		}

		aux->a_hor = NULL;

		p = aux;

		aux = aux->p_vert;	/*passa para o prox*/

		free(p);	/*libera*/

		p=NULL;
	}

	return OK;
}





