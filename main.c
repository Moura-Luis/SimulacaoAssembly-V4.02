// Universidade Federal do Maranhão
// Projeto - Simulação Linguagem Assembly - Arquitetura de Computadores
// Made by: Stenio Fonseca and Luis Moura

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int is_string;  // 1 se armazenar string, 0 se armazenar número
    union {
        int valor;
        char instrucao[50];
    } data;
} Memoria;

Memoria memoria_instrucao[10];  // Apenas os primeiros 10 endereços armazenam strings ou valores
int memoria[256];  // Todos os 256 endereços armazenam valores numéricos
int R[4]; // Registradores R0, R1, R2, R3
int PC = 0; // Contador de programa
int contador_operacoes = 0; // Contador de operações realizadas

// Exibe os valores de uma faixa de memória
void estado_memoria(int inicio, int fim) {
    printf("Estado atual da memoria (de %d a %d):\n", inicio, fim);
    for (int i = inicio; i <= fim && i < 256; i++) {
        printf("Memoria[%d]: %d\n", i, memoria[i]);
    }
    printf("\n");
}

// Exibe os valores dos registradores
void estado_registradores() {
    printf("Estado atual dos registradores:\n");
    for (int i = 0; i < 4; i++) {
        printf("R%d: %d\n", i, R[i]);
    }
    printf("\n");
}

// Processa uma instrução
void executa_instrucao(char *instrucao) {
    char operacao[10];
    int reg_dest, reg_fonte1, reg_fonte2, endereco;

    if (sscanf(instrucao, "%s R%d, R%d, R%d", operacao, &reg_dest, &reg_fonte1, &reg_fonte2) == 4) {
        if (strcmp(operacao, "ADD") == 0) {
            R[reg_dest] = R[reg_fonte1] + R[reg_fonte2];
            printf("Resultado armazenado em R%d: %d\n", reg_dest, R[reg_dest]);
        } else if (strcmp(operacao, "SUB") == 0) {
            R[reg_dest] = R[reg_fonte1] - R[reg_fonte2];
            printf("Resultado armazenado em R%d: %d\n", reg_dest, R[reg_dest]);
        } else if (strcmp(operacao, "MUL") == 0) {
            R[reg_dest] = R[reg_fonte1] * R[reg_fonte2];
            printf("Resultado armazenado em R%d: %d\n", reg_dest, R[reg_dest]);
        } else if (strcmp(operacao, "DIV") == 0) {
            if (R[reg_fonte2] != 0) {
                R[reg_dest] = R[reg_fonte1] / R[reg_fonte2];
                printf("Resultado armazenado em R%d: %d\n", reg_dest, R[reg_dest]);
            } else {
                printf("Erro: Divisao por zero!\n");
            }
        } else {
            printf("Instrucao nao reconhecida: %s\n", operacao);
        }
    } else if (sscanf(instrucao, "%s %d", operacao, &endereco) == 2 && strcmp(operacao, "J") == 0) {
        if (endereco >= 0 && endereco < 10) {
            PC = endereco;
            printf("Salto incondicional para o endereco %d\n", endereco);
            if (memoria_instrucao[PC].is_string) {
                printf("Executando instrucao armazenada: %s\n", memoria_instrucao[PC].data.instrucao);
                executa_instrucao(memoria_instrucao[PC].data.instrucao);
            }
        } else {
            printf("Erro: Endereco de memoria invalido para salto!\n");
        }
    } else if (sscanf(instrucao, "STORE %d %49[^\n]", &endereco, memoria_instrucao[endereco].data.instrucao) == 2) {
        if (endereco >= 0 && endereco < 10) {
            memoria_instrucao[endereco].is_string = 1;
            printf("Instrucao armazenada na memoria_instrucao[%d]: %s\n", endereco, memoria_instrucao[endereco].data.instrucao);
        } else {
            printf("Erro: Apenas endereços de 0 a 9 podem armazenar instruções!\n");
        }
    }
    PC++;
    contador_operacoes++;
    if (contador_operacoes % 8 == 0) {
        printf("\n--- Estado apos %d operacoes ---\n", contador_operacoes);
        estado_registradores();
        estado_memoria(0, 9);
    }
}

int main() {
    char instrucao[50];
    R[0] = 0;
    R[1] = 10;
    R[2] = 20;
    R[3] = 30;
    for (int i = 0; i < 256; i++) {
        memoria[i] = i * 10;
    }

    printf("Simulador de Assembly - MIPS32\n");
    printf("Digite instrucoes no formato:\n");
    printf("  OP Rdest, Rsrc1, Rsrc2 (e.g., ADD R0, R1, R2)\n");
    printf("  STORE <endereco> <instrucao> (e.g., STORE 2 ADD R0, R1, R2)\n");
    printf("  J <endereco> (e.g., J 2)\n");
    printf("  Para sair, digite 'SAIR'\n\n");
    
    printf("Estado inicial dos registradores:\n");
    estado_registradores();
    printf("Estado inicial da memoria (primeiras 10 posicoes):\n");
    estado_memoria(0, 9);
    
    while (1) {
        printf("> ");
        fgets(instrucao, sizeof(instrucao), stdin);
        instrucao[strcspn(instrucao, "\n")] = '\0';
        if (strcmp(instrucao, "SAIR") == 0) {
            break;
        }
        executa_instrucao(instrucao);
    }
    printf("Encerrando o simulador...\n");
    return 0;
}


