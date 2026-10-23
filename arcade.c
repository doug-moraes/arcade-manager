/*
 * Projeto Integrador (P2) - Sistema de Gestao de Fliperama (CRUD em C)
 * Disciplina: Algoritmos e Pensamento Computacional
 *
 * Integrante A: Douglas Luan
 * Modulo: Insercao e Cadastro (validacao de limites na RAM)
 *
 * Neste arquivo, a funcao cadastrarMaquina() e a parte do integrante A.
 * A funcao listarMaquinas() e o main() com o menu sao provisorios e
 * servem apenas para testar o cadastro.
 */

#include <stdio.h>
#include <stdbool.h>

/* Limite de maquinas que cabem no vetor (capacidade da RAM) */
#define MAX_MAQUINAS 20
#define TAM_NOME 50
#define TAM_RECORDISTA 30

typedef struct {
    int id;
    char nome[TAM_NOME];
    int fichasInseridas;
    int highScore;
    char recordista[TAM_RECORDISTA];
    bool ativa;
} Maquina;

typedef struct {
    int precoFicha;
    int saldo;
    int fichasEmMaos;
} Caixa;

Maquina maquinas[MAX_MAQUINAS] = {
    {.id = 1, .nome = "Street Fighter II", .fichasInseridas = 12,
     .highScore = 15000, .recordista = "Ana", .ativa = true},
    {.id = 2, .nome = "Street Fighter II", .fichasInseridas = 9,
     .highScore = 11800, .recordista = "Diego", .ativa = true},
    {.id = 3, .nome = "Pac-Man", .fichasInseridas = 7,
     .highScore = 8200, .recordista = "Bruno", .ativa = true},
    {.id = 4, .nome = "Pac-Man", .fichasInseridas = 15,
     .highScore = 9600, .recordista = "Elisa", .ativa = true},
    {.id = 5, .nome = "Pinball", .fichasInseridas = 20,
     .highScore = 23400, .recordista = "Carla", .ativa = true},
    {.id = 6, .nome = "Pinball", .fichasInseridas = 4,
     .highScore = 17500, .recordista = "Felipe", .ativa = true},
    {.id = 7, .nome = "Street Fighter II", .fichasInseridas = 18,
     .highScore = 13200, .recordista = "Gabi", .ativa = false},
    {.id = 8, .nome = "Pac-Man", .fichasInseridas = 11,
     .highScore = 7400, .recordista = "Hugo", .ativa = false},
    {.id = 9, .nome = "Pinball", .fichasInseridas = 6,
     .highScore = 20100, .recordista = "Iara", .ativa = false},
    {.id = 10, .nome = "Street Fighter II", .fichasInseridas = 3,
     .highScore = 5900, .recordista = "Joao", .ativa = false}
};

/* Quantas posicoes do vetor estao realmente em uso */
int totalMaquinas = 10;

/* Proximo id a ser usado no cadastro. So aumenta, assim nunca repete
   um id, mesmo depois de o modulo C remover uma maquina */
int proximoId = 11;

Caixa caixa = {1, 0, 0};

/*
 * Modulo A (Douglas Luan): cadastra uma nova maquina no vetor.
 * Antes de gravar, valida o limite do vetor para nao escrever fora
 * da memoria reservada.
 */
void cadastrarMaquina(void) {
    /* Validacao de limite: se o vetor esta cheio, nao cadastra */
    if (totalMaquinas >= MAX_MAQUINAS) {
        printf("Limite de maquinas atingido (%d).\n", MAX_MAQUINAS);
        return;
    }

    /* A maquina e montada numa variavel local e so depois copiada
       para o vetor */
    Maquina nova;

    nova.id = proximoId;

    /* O espaco antes do % descarta o Enter que sobrou da leitura
       anterior; [^\n] aceita nomes com espaco; 49 evita estourar
       o tamanho de nome[50] */
    printf("Nome do jogo: ");
    scanf(" %49[^\n]", nova.nome);

    /* Valores iniciais de uma maquina nova */
    nova.fichasInseridas = 0;
    nova.highScore = 0;
    nova.recordista[0] = '\0';
    nova.ativa = true;

    /* Grava na primeira posicao livre e atualiza os contadores */
    maquinas[totalMaquinas] = nova;
    totalMaquinas++;
    proximoId++;

    printf("Maquina cadastrada com ID %d.\n", nova.id);
}

void listarMaquinas(void) {
    printf("\nID   Jogo                 Fichas  Recorde  Recordista  Status\n");
    for (int i = 0; i < totalMaquinas; i++) {
        printf("%-4d %-20s %-7d %-8d ",
               maquinas[i].id, maquinas[i].nome,
               maquinas[i].fichasInseridas, maquinas[i].highScore);

        if (maquinas[i].recordista[0] == '\0') {
            printf("%-11s ", "---");
        } else {
            printf("%-11s ", maquinas[i].recordista);
        }

        if (maquinas[i].ativa) {
            printf("Ativa\n");
        } else {
            printf("Manutencao\n");
        }
    }
}

int main(void) {
    int opcao;

    do {
        printf("\n=== FLIPERAMA ===\n");
        printf("1 - Cadastrar maquina\n");
        printf("2 - Listar maquinas\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                cadastrarMaquina();
                break;
            case 2:
                listarMaquinas();
                break;
            case 0:
                printf("Ate a proxima!\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}