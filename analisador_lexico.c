//Marcos Menghini Vitti
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void analiza (char *palavra, int *block, int *parentheses, int *aspasD, int *aspasS);
void reinicializa (char *palavra);
void tokens();

/**/char tokens_libs[1][3][10] = {"#", "<", ">"};
/**/char tokens_keyWords[1][3][10] = {"main", "return", "printf"};
/**/char tokens_in_looping[1][2][10] = {"continue", "break"};
/**/char tokens_operators_or_parameter[1][2][10] = {"(", ")"};
/**/char tokens_looping[1][4][10] = {"do", "while", "for", "goto"};
/**/char tokens_commander_delimitador[1][3][10] = {";","{", "}"};
/**/char tokens_messages[1][2][10] = {"\"", "'"};
/**/char tokens_operators[1][6][10] = {"+", "-", "=", "==", "!=", "/"};
/**/char tokens_messages_special[1][5][10] = {"\\d", "\\f", "\\n", "\\r", "\\t"};
/**/char tokens_conditionals[1][5][10] = {"if", "else", "switch", "case", "break"};
/**/char tokens_typing[1][12][10] = {"struct", "typedef", "void", "char", "int", "float", "double", "const", "signed", "unsigned", "long", "short"};

int main(void) {

	tokens ();
	printf("ANALIZADOR LEXICO\n\n");

    	char ch;
	char palavra[100];
	int block = 0;
	int parentheses = 0;
	int aspasD = 0;
	int aspasS = 0;

    	FILE *arq;

    	arq = fopen ("programa.txt","r");

    	if (arq == NULL) {

		printf("Falha ao abrir o arquivo !");

		return 0;

    	}

	int i = 0;
	int linha = 1;

	printf("linha %d : ", linha);

    	while( (ch=fgetc(arq)) != EOF ) {

		if (ch == '\n') {

			analiza(palavra, &block, &parentheses, &aspasD, &aspasS);
			i = 0;
			reinicializa(palavra);

			linha++;
			printf("\n\nlinha %d : ", linha);
			continue;
		}

		putchar(ch);
		palavra[i] = ch;

		i++;
	}

	fclose(arq);

	printf("\n");

	if (block != 0) {

		printf(" ======> ERRO: Desbalanceamento de '{' '}' ! %d\n", block);

	}

	if (parentheses != 0) {

		printf(" ======> ERRO: Desbalanceamento de '(' ')' ! %d\n", parentheses);

	}

	if (aspasD % 2 != 0) {

		printf(" ======> ERRO: Desbalanceamento de '\"' ! %d\n", aspasD);

	}

	if (aspasS % 2 != 0) {

		printf(" ======> ERRO: Desbalanceamento de aspas simples ! %d\n", aspasS);

	}
}

void analiza (char *palavra, int *block, int *parentheses, int *aspasD, int *aspasS) {

	int isSet = 0;

	for (int count = 0, i = 0; i <= 2; i++) {

		if ( strstr(palavra, tokens_libs[0][i]) ) count++;

		if  (i == 2) {

			switch (count) {

				case 3:
					printf("\n ==> Token %s, %s, %s indicam inclusão de biblioteca!", tokens_libs[0][0], tokens_libs[0][1], tokens_libs[0][2]);
					isSet = 1;
				break;

				case 2:
				case 1:
					printf("\n ==> Erro na inclusão de biblioteca!");
				break;

				default:
				break;
			}

			count = 0;
		}
	}

	for (int i = 0; i <= 2; i++) {

		if ( strstr(palavra, tokens_keyWords[0][i]) && !isSet)  {

			printf("\n ==> Token %s, indicam palavra reservada!", tokens_keyWords[0][i]);
		}
	}

	for (int count = 0, i = 0; i <= 1; i++) {

		if ( strstr(palavra, tokens_messages[0][i]) )  {

			int nextEnter = 0;

			for (int j = 0; palavra[j] != '\0'; j++) {

				if (palavra[j] == '"') {

					*aspasD = *aspasD + 1;
					if (*aspasD == 1) {
						nextEnter = j + 1;
						printf("\n ==> Token %s indica abertura de impressão string!", tokens_messages[0][i]);
					}

					if (*aspasD == 2 && palavra[j-1] != '\\') {
						printf("\n ==> Token %s indica fechamento de impressão string!", tokens_messages[0][i]);
					}
				} else if (palavra[j] == '\'') {
					
					*aspasS = *aspasS + 1;
					if (*aspasS == 1) {
						nextEnter = j + 1;
						printf("\n ==> Token %s indica abertura de impressão carater!", tokens_messages[0][i]);
					}

					if (*aspasS == 2 && palavra[j-1] != '\\') {
						printf("\n ==> Token %s indica fechamento de impressão carater!", tokens_messages[0][i]);
					}
				} else if (palavra[j] == '%') {

					if (palavra[j+1] == 'c' || palavra[j+1] == 's' || palavra[j+1] == 'd' || palavra[j+1] == 'i' || palavra[j+1] == 'f') {

						printf("\n ==> Token %c%c indica impressão de variável!", palavra[j], palavra[j+1]);

						j++;

						continue;	
					}
				} else {

					if ((*aspasD == 1 || *aspasS == 1) && nextEnter == j) {

						printf("\n ==>  O identificador ");

						for (int k = j; k < strlen(palavra); k++) {

							if (palavra[k] == '"' || palavra[k] == '\'') {

								nextEnter = k + 1;
								break;
							}

							else if (palavra[k] != '%' && palavra[k] != '\\') {

								switch (palavra[k]) {

									case 'd':

										switch (palavra[k-1]) {

											case '%':

												if (palavra[k-2] == '%') {

													printf("%");
													printf("%c", palavra[k]);
												}
											break;

											case '\\':

												if (palavra[k-2] != '\\') {

													printf("%c", palavra[k]);
												}
											break;
										}
									break;

									case 'f':

										switch (palavra[k-1]) {

											case '%':

												if (palavra[k-2] == '%') {

													printf("%");
													printf("%c", palavra[k]);
												}
											break;

											case '\\':

												if (palavra[k-2] != '\\') {

													printf("%c", palavra[k]);
												}
											break;
										}
									break;

									case 'c':
										if (palavra[k-1] != '%' && palavra[k-2] != '%') {

											printf("%c", palavra[k]);
										}
									break;

									case 's':
										if (palavra[k-1] != '%' && palavra[k-2] != '%') {

											printf("%c", palavra[k]);
										}
									break;

									case 'n':
										if (palavra[k-1] != '\\' && palavra[k-2] != '\\') {

											printf("%c", palavra[k]);
										}
									break;

									case 'r':
										if (palavra[k-1] != '\\' && palavra[k-2] != '\\') {

											printf("%c", palavra[k]);
										}
									break;

									case 't':
										if (palavra[k-1] != '\\' && palavra[k-2] != '\\') {

											printf("%c", palavra[k]);
										}
									break;

									default:

										printf("%c", palavra[k]);

									break;
								}
							}
						}

						printf(" indica texto a ser imprimido na tela!");
					}
				}
			}
		}
	}

	for (int i = 0; i <= 11; i++) {

		if ( strstr(palavra, tokens_typing[0][i]) )  {

			printf("\n ==> Token %s, indicam tipagem!", tokens_typing[0][i]);

		}
	}

	for (int i = 0; i <= 1; i++) {

		if ( strstr(palavra, tokens_in_looping[0][i]) )  {

			printf("\n ==> Token %s, indica palavra reservada dentro de loops!", tokens_in_looping[0][i]);
		}
	}

	for (int i = 0; i <= 1; i++) {

		if ( strstr(palavra, tokens_operators_or_parameter[0][i]) )  {

			if (strstr(tokens_operators_or_parameter[0][i], "(")) {
				*parentheses = *parentheses + 1;
				printf("\n ==> Token %s indica inicio de paramento ou operador de prioridade!", tokens_operators_or_parameter[0][i]);
			}

			if (strstr(tokens_operators_or_parameter[0][i], ")")) {
				*parentheses = *parentheses - 1;
				printf("\n ==> Token %s indica fim de paramento ou operador de prioridade!", tokens_operators_or_parameter[0][i]);
			}
		}
	}

	for (int count = 0, i = 0; i <= 3; i++) {

		if ( strstr(palavra, tokens_looping[0][i]) )  {

			printf("\n ==> Token %s indica palavra reservada para loops!", tokens_looping[0][1]);

		}
	}

	for (int i = 0; i <= 4; i++) {

		if ( strstr(palavra, tokens_commander_delimitador[0][i]) ) {

			if ( strstr(tokens_commander_delimitador[0][i], ";") ) {
				printf("\n ==> Token %s indica final de instrução!", tokens_commander_delimitador[0][i]);
			}

			if ( strstr(tokens_commander_delimitador[0][i], "{") ) {
				*block = *block + 1;
				printf("\n ==> Token %s indica inicio de bloco!", tokens_commander_delimitador[0][1]);
			}

			if ( strstr(tokens_commander_delimitador[0][i], "}") ) {
				*block = *block - 1;
				printf("\n ==> Token %s indica fim do bloco!", tokens_commander_delimitador[0][2]);
			}
		}
	}

	for (int i = 0; i <= 5; i++) {

		if ( strstr(palavra, tokens_operators[0][i]) )  {

			printf("\n ==> Token %s indicam operadores!", tokens_operators[0][i]);

		}
	}

	for (int i = 0; i <= 4; i++) {

		if ( strstr(palavra, tokens_conditionals[0][i]) )  {

			printf("\n ==> Token %s indicam condicionais!", tokens_conditionals[0][i]);

		}
	}
}

void reinicializa (char *palavra) {

	memset(palavra, 0, 100 * sizeof(char));

}

void tokens () {
	printf("=== === DEFINE === ===\n");
	for (int i = 0; i <= 2; i++) {

		printf("%d - %s \n",i,tokens_libs[0][i]);
	}
	printf("=== === === === ===\n");

	printf("=== === OPERDOR OU PARAMETRO === ===\n");
	for (int i = 0; i <= 2; i++) {

		printf("%d - %s \n",i,tokens_operators_or_parameter[0][i]);
	}
	printf("=== === === === ===\n");


	printf("\n=== === Key Words === ===\n");
	for (int i = 0; i <= 2; i++) {

		printf("%d - %s \n",i,tokens_keyWords[0][i]);
	}
	printf("=== === === === ===\n");


	printf("\n=== === IN LOOPING === ===\n");

	for (int i = 0; i <= 1; i++) {

		printf("%d - %s \n",i,tokens_in_looping[0][i]);
	}
	printf("=== === === === ===\n");


	printf("\n=== === LOOPING === ===\n");
	for (int i = 0; i <= 3; i++) {

		printf("%d - %s \n",i,tokens_looping[0][i]);
	}
	printf("=== === === === ===\n");

	printf("\n=== === MENSAGENS === ===\n");
	for (int i = 0; i <= 4; i++) {

		printf("%d - %s \n",i,tokens_messages_special[0][i]);
	}
	printf("=== === === === ===\n");

	printf("\n=== === DELIMITADOR === ===\n");
	for (int i = 0; i <= 2; i++) {

		printf("%d - %s \n",i,tokens_commander_delimitador[0][i]);
	}
	printf("=== === === === ===\n");

	printf("\n=== === OPERADOR === ===\n");
	for (int i = 0; i <= 5; i++) {

		printf("%d - %s \n",i,tokens_operators[0][i]);
	}
	printf("=== === === === ===\n");

	printf("\n=== === CONDICIONAL === ===\n");
	for (int i = 0; i <= 4; i++) {

		printf("%d - %s \n",i,tokens_conditionals[0][i]);
	}
	printf("=== === === === ===\n");

	printf("\n=== === MENSAGENS === ===\n");
	for (int i = 0; i <= 1; i++) {

		printf("%d - %s \n",i,tokens_messages[0][i]);
	}
	printf("=== === === === ===\n");

	printf("\n=== === TIPOS === ===\n");
	for (int i = 0; i <= 11; i++) {

		printf("%d - %s \n",i,tokens_typing[0][i]);
	}
	printf("=== === === === ===\n");
}
