#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define ENTER 13
#define ESCAPE 27
#define MAX_AMIGOS 10

struct data {
	int dia;
	int mes;
	int ano;
};

struct amigo {
	char nome[50];
	char telefone[15];
	struct data dataNascimento;
};

void menu();
int voltar();
void mostraFicha(struct amigo ficha);
void insereFicha(struct amigo *ficha);
int validarAno(int dia, int mes, int ano);
int procurarFicha(struct amigo agenda[], char nome[], int contador);
void mudarDados(struct amigo agenda[], int contador);
int anoBisexto(int ano, int mes);
void listarAmigosOrdenados(struct amigo agenda[], int contador);
void eliminarAmigo(struct amigo agenda[], int *contador, int posicao);

int main() {
	struct amigo agenda[MAX_AMIGOS];
	char opcao;
	int contador = 0;
	int i;  // Moved declaration outside loop
	
	// Inicializa a agenda
	for(i = 0; i < MAX_AMIGOS; i++) {
		strcpy(agenda[i].nome, "");
	}
	
	setlocale(LC_ALL, "Portuguese");
	
	FILE *f;
	if((f = fopen("agenda.bin", "rb")) != NULL) {
		fread(agenda, sizeof(agenda), 1, f);
		fclose(f);
		
		// Conta quantos amigos já existem
		for(i = 0; i < MAX_AMIGOS; i++) {
			if(strlen(agenda[i].nome) > 0) {
				contador++;
			}
		}
	}

	do {
		menu();
		opcao = getch();
		
		switch(opcao) {
			case '1': // Novo amigo
				if(contador < MAX_AMIGOS) {
					system("cls");
					insereFicha(&agenda[contador]);
					if(strlen(agenda[contador].nome) > 0) {
						contador++;
					}
				} else {
					printf("\nAgenda cheia!\n");
					getch();
				}
				break;
				
			case '2': // Listar todos
				system("cls");
				printf("\n*************** LISTA DE AMIGOS ***************\n");
				for(i = 0; i < contador; i++) {
					mostraFicha(agenda[i]);
				}
				printf("\nFim da listagem....\n");
				getch();
				break;
				
			case '3': { // Alterar dados
				char subOpcao;
				char nome[50];
				int posicao;
				
				do {
					system("cls");
					printf("\n*************** ALTERAR DADOS ***************\n");
					printf("\n1 - Lista ordenada\n2 - Editar\n3 - Eliminar\n4 - Voltar\n");
					subOpcao = getch();
					
					switch(subOpcao) {
						case '1':
							listarAmigosOrdenados(agenda, contador);
							getch();
							break;
							
						case '2':
							system("cls");
							printf("Nome do amigo que deseja editar: ");
							fflush(stdin);
							gets(nome);
							posicao = procurarFicha(agenda, nome, contador);
							if(posicao == -1) {
								printf("Amigo não encontrado!\n");
							} else {
								mudarDados(agenda, contador);
							}
							getch();
							break;
						
						case '3':
							system("cls");
							printf("Nome do amigo que deseja eliminar: ");
							fflush(stdin);
							gets(nome);
							posicao = procurarFicha(agenda, nome, contador);
							if(posicao == -1) {
								printf("Amigo não encontrado!\n");
							} else {
								eliminarAmigo(agenda, &contador, posicao);
								printf("Amigo eliminado com sucesso!\n");
							}
							getch();
							break;
					}
				} while(subOpcao != '4');
				break;
			}
			
			case '4': // Lista ordenada
				listarAmigosOrdenados(agenda, contador);
				getch();
				break;
		}
	} while(opcao != '5');

	// Salva a agenda no arquivo
	if((f = fopen("agenda.bin", "wb")) != NULL) {
		fwrite(agenda, sizeof(agenda), 1, f);
		fclose(f);
	}
	
	system("cls");
	printf("\n*************** PROGRAMA ENCERRADO ***************\n");
	return 0;
}

void insereFicha(struct amigo *ficha) {
	printf("\n*************** N O V O  A M I G O ***************\n");
	printf("\nNome: ");
	gets(ficha->nome);
	printf("\nTelefone: ");
	gets(ficha->telefone);
	printf("\n");
	fflush(stdin);
	do {
		printf("Data de nascimento - ");
		printf("Dia:  ");
		scanf("%d", &ficha->dataNascimento.dia);
		fflush(stdin);
		printf("                   - Ms: ");
		scanf("%d", &ficha->dataNascimento.mes);
		fflush(stdin);
		printf("                   - Ano: ");
		scanf("%d", &ficha->dataNascimento.ano);
		fflush(stdin);
	} while(validarAno(ficha->dataNascimento.dia, ficha->dataNascimento.mes, ficha->dataNascimento.ano) == 0);
}

void mostraFicha(struct amigo ficha) {
	printf("------------------------------------------\n");
	printf("Nome:  %s\n", ficha.nome);
	printf("Telefone:  %s\n", ficha.telefone);
	printf("Data de nascimento: %d/%d/%d \n\n", ficha.dataNascimento.dia, ficha.dataNascimento.mes, ficha.dataNascimento.ano);
}

int anoBisexto(int ano, int mes) {
	if(mes != 2) {
		return(0);
	}
	if(ano % 4 == 0) {
		return(1);
	}
}

int validarAno(int dia, int mes, int ano) {
	int data[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if(dia <= 0 || dia > 31 || mes <= 0 || mes > 12 || ano < 1927 || ano > 2024) {
		printf("Erro! Data incorreta!\n");
		return(0);
	} else {
		;
	}
	if(data[mes - 1] - anoBisexto(ano, mes) < dia) {
		return(0); // a diferença entre getch() e return(0), é que o return(0) é considerado o fim de uma função e não ocupa espaço na memória.
	}
	return(1);
}

void menu() {
	system("cls");
	printf("\n******************* M E N U *******************\n");
	printf("\n1 - Novo amigo\n");
	printf("2 - Lista todos os amigos\n");
	printf("3 - Altera dados de um amigo\n");
	printf("4 - Lista de amigos ordenada\n");
	printf("5 - Sair\n");
}

int voltar() {
	printf("\nEscape..........................para Voltar\n");
	printf("Precione qualquer tecla...........para Continuar\n\n");
	return(getch());
}

int procurarFicha(struct amigo agenda[], char nome[], int contador) {
	int i;
	for(i = 0; i < contador; i++) {
		if(strcmp(agenda[i].nome, nome) == 0) {
			return(i);
		}
	}
	return -1;
}

void mudarDados(struct amigo agenda[], int contador) {
	char nome[50];
	int op1;
	int i;
	int dia, mes, ano;
	
	for(i = 0; i < contador; i++) {
		printf("\n--------------------------------------\n");
		printf("1 - Mudar nome\n2 - Mudar telefone\n3 - Mudar data nascimento\n4 - Voltar\n");
		op1 = getch();
		if(op1 == '1') {
			system("cls");
			printf("Nome: ");
			gets(agenda[i].nome);
			getch();
		}
		if(op1 == '2') {
			system("cls");
			printf("Telefone: ");
			gets(agenda[i].telefone);
			getch();
		}
		if(op1 == '3') {
			system("cls");
			printf("Data de nascimento  ");
			printf("Dia: ");
			scanf("%d", &agenda[i].dataNascimento.dia);
			printf("Mês: ");
			scanf("%d", &agenda[i].dataNascimento.mes);
			printf("Ano: ");
			scanf("%d", &agenda[i].dataNascimento.ano);
			fflush(stdin);
			getch();
		}
	}
}

void listarAmigosOrdenados(struct amigo agenda[], int contador) {
	int i;
	printf("\n*************** L I S T A  A M I G O S  O R D E N A D A ***************\n");
	for(i = 0; i < contador; i++) {
		mostraFicha(agenda[i]);
	}
	printf("\nFim da listagem....\n");
}

void eliminarAmigo(struct amigo agenda[], int *contador, int posicao) {
	int i;
	for(i = posicao; i < *contador - 1; i++) {
		strcpy(agenda[i].nome, agenda[i + 1].nome);
		strcpy(agenda[i].telefone, agenda[i + 1].telefone);
		agenda[i].dataNascimento = agenda[i + 1].dataNascimento;
	}
	strcpy(agenda[*contador - 1].nome, "");
	strcpy(agenda[*contador - 1].telefone, "");
	agenda[*contador - 1].dataNascimento.dia = 0;
	agenda[*contador - 1].dataNascimento.mes = 0;
	agenda[*contador - 1].dataNascimento.ano = 0;
	(*contador)--;
}
