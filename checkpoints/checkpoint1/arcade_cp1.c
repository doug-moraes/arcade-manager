/*
 * Projeto Integrador (P2) - Sistema de Gestao de Fliperama (CRUD em C)
 * Disciplina: Algoritmos e Pensamento Computacional
 *
 * CHECKPOINT 1: arquitetura inicial (structs, menu principal e submenus).
 * As rotinas marcadas como [Em construcao] serao implementadas nos
 * proximos checkpoints (a versao completa esta no arquivo arcade.c).
 *
 * Integrante A: Douglas Luan   - Modulo: Insercao e Cadastro
 * Integrante B: Leonardo Messias - Modulo: Busca e Modificacao
 * Integrante C: Douglas Luan   - Modulo: Remocao e Matriz
 * Integrante D: Eduardo Pan    - Modulo: Relatorios e Ordenacao
 */

#include <stdio.h>

#define MAX_MAQUINAS 50
#define TAM_NOME 50
#define TAM_RECORDISTA 30
#define DIAS_SEMANA 7

/* ============================================================
 * PARTE 1: MODELAGEM DE DADOS (ESCOPO GLOBAL)
 * ============================================================ */

struct Maquina
{
    int id;
    char nome[TAM_NOME];
    int fichasInseridas;
    int highScore;
    char recordista[TAM_RECORDISTA];
    int ativa; /* 1 = Ativa, 0 = Manutencao */
    float receita;
};

struct Maquina maquinas[MAX_MAQUINAS];
int totalMaquinas = 0;

struct Caixa
{
    float precoFicha;
    float totalArrecadado;
    int fichasVendidas;
    int fichasEmMaos;
};

struct Caixa caixa;

/* Fichas inseridas por maquina em cada dia da semana (Dom a Sab) */
int usoSemanal[MAX_MAQUINAS][DIAS_SEMANA];

/* ============================================================
 * PARTE 2: MODULOS (UM SUBMENU POR INTEGRANTE)
 * ============================================================ */

// Autor: Douglas Luan
void moduloA(void)
{
    int opcao;

    do
    {
        printf("\n--- [MODULO A: CADASTRO E CAIXA (Douglas)] ---\n");
        printf("1. Cadastrar nova maquina\n");
        printf("2. Listar maquinas\n");
        printf("3. Comprar fichas no caixa\n");
        printf("4. Jogar em uma maquina\n");
        printf("5. Consultar caixa\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\n[Em construcao] Cadastro de maquina (nome do jogo, id...)\n");
            break;
        case 2:
            printf("\n[Em construcao] Listagem de maquinas...\n");
            break;
        case 3:
            printf("\n[Em construcao] Venda de fichas no caixa...\n");
            break;
        case 4:
            printf("\n[Em construcao] Jogada em maquina e atualizacao de recorde...\n");
            break;
        case 5:
            printf("\n[Em construcao] Consulta do caixa...\n");
            break;
        case 0:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo A!\n");
        }
    } while (opcao != 0);
}

// Autor: Leonardo Messias
void moduloB(void)
{
    int opcao;

    do
    {
        printf("\n--- [MODULO B: BUSCA E MODIFICACAO (Leonardo)] ---\n");
        printf("1. Buscar maquina por ID\n");
        printf("2. Buscar maquina por nome\n");
        printf("3. Modificar nome ou recorde\n");
        printf("4. Alterar status (Ativa/Manutencao)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\n[Em construcao] Busca de maquina por ID...\n");
            break;
        case 2:
            printf("\n[Em construcao] Busca de maquina por nome...\n");
            break;
        case 3:
            printf("\n[Em construcao] Edicao de nome ou recorde...\n");
            break;
        case 4:
            printf("\n[Em construcao] Troca de status Ativa/Manutencao...\n");
            break;
        case 0:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo B!\n");
        }
    } while (opcao != 0);
}

// Autor: Douglas Luan
void moduloC(void)
{
    int opcao;

    do
    {
        printf("\n--- [MODULO C: REMOCAO E MATRIZ DE USO (Douglas)] ---\n");
        printf("1. Remover maquina\n");
        printf("2. Matriz de uso semanal\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\n[Em construcao] Remocao de maquina por ID...\n");
            break;
        case 2:
            printf("\n[Em construcao] Matriz de uso (maquinas x dias da semana)...\n");
            break;
        case 0:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo C!\n");
        }
    } while (opcao != 0);
}

// Autor: Eduardo Pan
void moduloD(void)
{
    int opcao;

    do
    {
        printf("\n--- [MODULO D: RELATORIOS E ORDENACAO (Eduardo)] ---\n");
        printf("1. Relatorio geral\n");
        printf("2. Ranking por high score\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\n[Em construcao] Relatorio geral (totais, media, extremos)...\n");
            break;
        case 2:
            printf("\n[Em construcao] Ranking com Bubble Sort...\n");
            break;
        case 0:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo D!\n");
        }
    } while (opcao != 0);
}

/* ============================================================
 * PARTE 3: FUNCAO PRINCIPAL (MENU PRINCIPAL)
 * ============================================================ */

// Autor: Douglas Luan
int main(void)
{
    int opcao;

    do
    {
        printf("\n======================================\n");
        printf("   FLIPERAMA - SISTEMA DE GESTAO\n");
        printf("======================================\n");
        printf("1. Modulo A: Cadastro e Caixa (Douglas)\n");
        printf("2. Modulo B: Busca e Modificacao (Leonardo)\n");
        printf("3. Modulo C: Remocao e Matriz (Douglas)\n");
        printf("4. Modulo D: Relatorios e Ordenacao (Eduardo)\n");
        printf("0. Sair\n");
        printf("--------------------------------------\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            moduloA();
            break;
        case 2:
            moduloB();
            break;
        case 3:
            moduloC();
            break;
        case 4:
            moduloD();
            break;
        case 0:
            printf("Ate a proxima!\n");
            break;
        default:
            printf("Opcao invalida. Digite um numero de 0 a 4.\n");
        }
    } while (opcao != 0);

    return 0;
}
