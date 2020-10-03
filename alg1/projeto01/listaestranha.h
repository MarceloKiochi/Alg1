typedef struct elementos{ //elementos da expressao
	char sinal; // 'n' se for um numero, ou o proprio sinal se for um sinal ou operacao
	double valor;//valor do numero, ou 0 se for um sinal
}elementos;

typedef struct expr{ //lista da expressao
	elementos* elem; //elementos (lista sequencial)
	int nelem; //numero de elementos
	int erro; //1 - erro  0 - ok
}expr;

expr* cria_expressao();

void adiciona_elem(expr** conta, char sinal, double valor);

void retira_elem(int posicao, expr* conta);

void its_been_a_long_day(expr* conta);
