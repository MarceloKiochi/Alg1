#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "calculadora.h"
#include "listaestranha.h"

void reseta(char* aux, int n){ //reseta uma string
	int j;

	for(j=0;j<n;j++){
		aux[j]='\0';
	}
}

double atod(char* string){ //ASCII To Double - transforma uma string em um numero

	assert(string!=NULL);

	int i, t, j;

	double aux1;
	double res;

	char* aux = (char*)calloc(50, sizeof(char));

	t = strlen(string);

	for(i=0;i<t;i++){

		if((string[i]<48 || string[i]>57) && string[i]!='.'){ //se nao for numero nem ponto, erro
			free(aux);
			return 0;
		}

		if(string[i]!='.') aux[i]=string[i]; //pega a parte inteira (ate o ponto)

		else{
			aux[i]='\0';
			break;
		}
	}

	i++;

	res = (double)atoi(aux);

	reseta(aux, i);
		
	for(j=i;j<t;j++){
		if(string[j]<48 || string[j]>57){ //se tiver algo que nao e' numero, erro.
			free(aux);
			return 0;
		}	
		aux[j-i]=string[j];	
	}
	
	aux1 = (double)atoi(aux) / (double)pow(10, strlen(aux));
	
	res = res + aux1;  //soma a parte inteira e a fracionaria

	free(aux);

	return res;	
}

void transforma(expr** conta, char* linha){ //percorre uma string e monta uma lista de elementos baseada nela

	assert(conta!=NULL);

	int i=0; 
	int abrep=0; //contador de parenteses
	int j=0;

	int npontos=0; //conta pontos em numeros

	int t = strlen(linha);

	char* aux = (char*)calloc(50, sizeof(char));

	for(i=0;i<t;i++){ //percorre os chars da string passada

		if((linha[i]>47 && linha[i]<58) || linha[i]=='.'){ //se for numero ou ponto, insere numa string aux

			if(linha[i]=='.') npontos++;

			if(npontos > 1){
				(*conta)->erro = 1;//se um numero tiver mais de um ponto, erro.		
			}	

			aux[j] = linha[i];

			if(i==t-1){
				adiciona_elem(conta, 'n', atod(aux)); //se for o ultimo char da string, adiciona o numero a lista
			}										  //sinal e' 'n' e o valor e' conversao da string aux para double

			j++;			
		}	
		
		else if(linha[i]=='+' || linha[i]=='-' || linha[i]=='/' || linha[i]=='*' || linha[i]==')' || linha[i]=='('){	
			//se for um sinal

			if(linha[i-1]>47 && linha[i-1]<58){	//se o anterior for numero

				if(linha[i]=='('){		//nao pode ter ( depois de um numero
					(*conta)->erro=1;									
				}

				adiciona_elem(conta, 'n', atod(aux)); //quando entra um sinal, quer dizer que a leitura do numero acabou

				reseta(aux, 50);

				npontos = 0;

				j=0;
				
			}		
														//se o atual for um sinal,
			else if(linha[i]!='(' && linha[i-1]!=')'){ //se o anterior nao for numero, o atual precisa ser (
				(*conta)->erro=1;						//ou o anterior precisa ser )
			}

			if(linha[i]==')') abrep--;
												//verifica se os pares de parenteses estao corretos
			else if(linha[i]=='(') abrep++;

			adiciona_elem(conta, linha[i], 0); //adiciona o sinal
		}
	}

	linha[i]='\0';

	if(abrep!=0){	
		(*conta)->erro=1; //se abrep nao zerou, pares de parenteses errados
	}

	free(aux);
}



void resolve_parenteses(expr* conta){ //resolve os parenteses da expressao

	assert(conta!=NULL);
	assert(conta->elem!=NULL);

	int i, j, k;

	for(i=0;i<conta->nelem;i++){ //percorre a expressao

		if(conta->elem[i].sinal == ')'){ //ate achar um )

			for(j=i; j>=0; j--){
				if(conta->elem[j].sinal == '(') break; //volta ate achar o ( correspondente
			}

			for(k=j+1; k<i-2;k++){ //percorre o pedaco dentro dos parenteses

				if(conta->elem[k+1].sinal=='*'){ //se tiver *, multiplica

					conta->elem[k].valor = conta->elem[k].valor * conta->elem[k+2].valor;

					retira_elem(k+2, conta); //3 elementos viram 1, retira 2
					retira_elem(k+1, conta);

					k--;
					i-=2;
				}
				else if(conta->elem[k+1].sinal=='/'){ //divisao

					if(conta->elem[k+2].valor==0){	//nao pode ser divisao por 0					
						conta->erro = 1;
						return;
					}

					conta->elem[k].valor = conta->elem[k].valor / conta->elem[k+2].valor;

					retira_elem(k+2, conta);
					retira_elem(k+1, conta);

					k--;
					i-=2;
				}
			}			
			

			for(k=j+1; k<i-2;k++){

				if(conta->elem[k+1].sinal=='+'){ //soma

					conta->elem[k].valor = conta->elem[k].valor + conta->elem[k+2].valor;

					retira_elem(k+2, conta);
					retira_elem(k+1, conta);

					k--;					
					i-=2;

				}

				else if(conta->elem[k+1].sinal=='-'){ //subtracao

					conta->elem[k].valor = conta->elem[k].valor - conta->elem[k+2].valor;

					retira_elem(k+2, conta);
					retira_elem(k+1, conta);

					k--;				
					i-=2;
				}
			}			
			
			retira_elem(j, conta);		//tira os parenteses da expressao
			retira_elem(j+1, conta);	

			i-=2;			
			
		}
	}
	
}



void resolve_mult_div(expr* conta){ //percorre a expressao e faz as multiplicacoes e divisoes

	assert(conta!=NULL);
	assert(conta->elem!=NULL);

	int k;

	for(k=0;k<conta->nelem-2;k++){

		if(conta->elem[k+1].sinal=='*'){

			conta->elem[k].valor = conta->elem[k].valor * conta->elem[k+2].valor;

			retira_elem(k+2, conta);
			retira_elem(k+1, conta);

			k--;		
		}

		else if(conta->elem[k+1].sinal=='/'){	

			if(conta->elem[k+2].valor==0){
				conta->erro=1;
				return;
			}

			conta->elem[k].valor = conta->elem[k].valor / conta->elem[k+2].valor;

			retira_elem(k+2, conta);
			retira_elem(k+1, conta);

			k--;			
		}
	}		
}

void resolve_som_sub(expr* conta){ //resolve as somas e subtracoes da expressao

	assert(conta!=NULL);
	assert(conta->elem!=NULL);

	int k;

	for(k=0;k<conta->nelem-2;k++){

		if(conta->elem[k+1].sinal=='+'){

			conta->elem[k].valor = conta->elem[k].valor + conta->elem[k+2].valor;

			retira_elem(k+2, conta);
			retira_elem(k+1, conta);

			k--;
		}
		else if(conta->elem[k+1].sinal=='-'){

			conta->elem[k].valor = conta->elem[k].valor - conta->elem[k+2].valor;

			retira_elem(k+2, conta);
			retira_elem(k+1, conta);

			k--;
		}
	}		
}

void tira_espaco(char* linha){ //muda a string para retirar os espacos vazios (para facilitar)

	assert(linha!=NULL);

	int t, i, j;

	t = strlen(linha);

	j=0;

	for(i=0;i<t;i++){

		if(linha[i] != ' ' && linha[i]!='\t' && linha[i]!='\n'){
			linha[j]=linha[i];
			j++;
		}
	}

	linha[j]='\0';	
}

double resolve_conta(char* linha, double* res){

	int i;

	if(linha==NULL) return 0; //erro

	tira_espaco(linha);

	if(strlen(linha)==0) return 0; //erro

	expr* conta = cria_expressao();		

	transforma(&conta, linha);		

	resolve_parenteses(conta);	//deixa a expressao sem parenteses

	resolve_mult_div(conta); //deixa sem * e sem /
	
	resolve_som_sub(conta); //resolve o resto

	*res = conta->elem[0].valor; //resultado sera o numero que sobrar

	if(conta->erro==1) *res=0; //se der erro, deixa zerado
	
	i = !conta->erro;

	its_been_a_long_day(conta);

	return i; //retorna 1 para ok ou 0 para erro
}