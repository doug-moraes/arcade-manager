/*
 * Arquivo: jogadores.h
 * Perspectiva do Integrante B: JOGADORES do fliperama.
 * Struct, vetor global, leitura/escrita em jogadores.csv, CRUD completo
 * e compra de fichas no caixa.
 */
#ifndef JOGADORES_H
#define JOGADORES_H

#include <stdio.h>
#include <string.h>
#include "utilitarios.h"

/* Tamanhos fixos (o pre-processador troca cada nome pelo numero) */
#define MAX_JOGADORES 50
#define TAM_TELEFONE 20

// Autor: Leonardo Messias
struct Jogador
{
    int idJogador;
    char nome[TAM_NOME_PESSOA];
    char telefone[TAM_TELEFONE];
    char nivel;       /* B-Bronze, P-Prata, O-Ouro (pelo total gasto) */
    int fichas;       /* saldo de fichas compradas e ainda nao usadas */
    float totalGasto; /* total em reais gasto na compra de fichas */
};

struct Jogador jogadores[MAX_JOGADORES];
int totalJogadores = 0;

/*
 * PERSISTENCIA: regrava o arquivo inteiro a partir do vetor. O modo "w"
 * zera o arquivo, entao qualquer cadastro, edicao ou remocao e refletido.
 */
// Autor: Leonardo Messias
void salvarJogadoresCSV()
{
    FILE *arq;
    int i;

    arq = fopen("jogadores.csv", "w");
    if (arq == NULL)
    {
        printf("[ERRO] Nao foi possivel gravar jogadores.csv!\n");
        return;
    }

    for (i = 0; i < totalJogadores; i++)
    {
        fprintf(arq, "%d;%s;%s;%c;%d;%.2f\n",
                jogadores[i].idJogador, jogadores[i].nome,
                jogadores[i].telefone, jogadores[i].nivel,
                jogadores[i].fichas, jogadores[i].totalGasto);
    }
    fclose(arq);
}

/*
 * PERSISTENCIA: carrega jogadores.csv para o vetor ao abrir o programa.
 * O fscanf devolve quantos campos leu (6). Numeros e o char levam &;
 * os textos nao, pois o nome do vetor de char ja e o endereco dele.
 */
// Autor: Leonardo Messias
void carregarJogadoresCSV()
{
    FILE *arq;

    arq = fopen("jogadores.csv", "r");
    if (arq == NULL)
    {
        return;
    }

    totalJogadores = 0;
    while (totalJogadores < MAX_JOGADORES &&
           fscanf(arq, "%d;%49[^;];%19[^;]; %c;%d;%f\n",
                  &jogadores[totalJogadores].idJogador,
                  jogadores[totalJogadores].nome,
                  jogadores[totalJogadores].telefone,
                  &jogadores[totalJogadores].nivel,
                  &jogadores[totalJogadores].fichas,
                  &jogadores[totalJogadores].totalGasto) == 6)
    {
        totalJogadores++;
    }
    fclose(arq);
}

/* Devolve a POSICAO do id no vetor, ou -1 se nao existir */
// Autor: Leonardo Messias
int buscarIndiceJogador(int idProcurado)
{
    int i;

    for (i = 0; i < totalJogadores; i++)
    {
        if (jogadores[i].idJogador == idProcurado)
        {
            return i;
        }
    }
    return -1;
}

/*
 * Define o nivel pelo total gasto: ate R$ 24,99 Bronze, de R$ 25,00 a
 * R$ 59,99 Prata e a partir de R$ 60,00 Ouro.
 */
// Autor: Leonardo Messias
void atualizarNivel(int pos)
{
    if (jogadores[pos].totalGasto >= 60.0)
    {
        jogadores[pos].nivel = 'O';
    }
    else if (jogadores[pos].totalGasto >= 25.0)
    {
        jogadores[pos].nivel = 'P';
    }
    else
    {
        jogadores[pos].nivel = 'B';
    }
}

/* Cabecalho da tabela de jogadores */
// Autor: Leonardo Messias
void imprimirCabecalhoJogadores()
{
    printf("\n%-4s %-24s %-13s %-6s %-7s %s\n",
           "ID", "Nome", "Telefone", "Nivel", "Fichas", "Total gasto");
}

/* Imprime UM jogador (pela posicao no vetor) */
// Autor: Leonardo Messias
void imprimirJogador(int pos)
{
    printf("%-4d %-24s %-13s %-6c %-7d R$ %.2f\n",
           jogadores[pos].idJogador, jogadores[pos].nome,
           jogadores[pos].telefone, jogadores[pos].nivel,
           jogadores[pos].fichas, jogadores[pos].totalGasto);
}

/*
 * CADASTRO: valida o limite do vetor e bloqueia ID repetido. O jogador
 * novo comeca sem fichas, com nivel Bronze e sem gastos.
 */
// Autor: Leonardo Messias
void cadastrarJogador()
{
    int id;

    if (totalJogadores >= MAX_JOGADORES)
    {
        printf("[ERRO] Limite de %d jogadores atingido!\n", MAX_JOGADORES);
        return;
    }

    printf("\nID do novo jogador: ");
    id = lerInteiro();
    if (id <= 0)
    {
        printf("[ERRO] O ID deve ser maior que zero!\n");
        return;
    }
    if (buscarIndiceJogador(id) != -1)
    {
        printf("[ERRO] Ja existe um jogador com o ID %d!\n", id);
        return;
    }

    /* O espaco antes do % descarta o ENTER que sobrou da leitura anterior;
       [^\n] aceita espacos; 49 e 19 evitam estourar nome[50] e telefone[20] */
    printf("Nome do jogador: ");
    scanf(" %49[^\n]", jogadores[totalJogadores].nome);
    limparSeparador(jogadores[totalJogadores].nome);

    printf("Telefone (somente numeros): ");
    scanf(" %19[^\n]", jogadores[totalJogadores].telefone);
    limparSeparador(jogadores[totalJogadores].telefone);

    jogadores[totalJogadores].idJogador = id;
    jogadores[totalJogadores].nivel = 'B';
    jogadores[totalJogadores].fichas = 0;
    jogadores[totalJogadores].totalGasto = 0;
    totalJogadores++;

    salvarJogadoresCSV();
    printf("[OK] Jogador cadastrado e salvo em 'jogadores.csv'!\n");
}

/* LISTAGEM formatada de todos os jogadores */
// Autor: Leonardo Messias
void listarJogadores()
{
    int i;

    printf("\n--- JOGADORES (%d cadastrados) ---", totalJogadores);
    if (totalJogadores == 0)
    {
        printf("\nNenhum jogador cadastrado.\n");
        return;
    }

    imprimirCabecalhoJogadores();
    for (i = 0; i < totalJogadores; i++)
    {
        imprimirJogador(i);
    }
    printf("Niveis: B-Bronze (ate R$ 24,99) | P-Prata (R$ 25,00 a 59,99) | "
           "O-Ouro (R$ 60,00 ou mais)\n");
}

/*
 * BUSCA TEXTUAL: strstr procura o texto digitado dentro do nome e devolve
 * o endereco onde ele comecou, ou NULL se nao achou.
 */
// Autor: Leonardo Messias
void buscarJogadorPorNome()
{
    char termo[30];
    int i;
    int encontrados = 0;

    printf("\nDigite parte do nome do jogador: ");
    scanf(" %29[^\n]", termo);

    for (i = 0; i < totalJogadores; i++)
    {
        if (strstr(jogadores[i].nome, termo) != NULL)
        {
            if (encontrados == 0)
            {
                imprimirCabecalhoJogadores();
            }
            imprimirJogador(i);
            encontrados++;
        }
    }

    if (encontrados == 0)
    {
        printf("Nenhum jogador encontrado com \"%s\".\n", termo);
    }
    else
    {
        printf("%d jogador(es) encontrado(s).\n", encontrados);
    }
}

/*
 * EDICAO: altera nome ou telefone na memoria e depois regrava o CSV
 * inteiro. Fichas e nivel mudam so pela compra de fichas e pelas partidas.
 */
// Autor: Leonardo Messias
void editarJogador()
{
    int id, pos, escolha;

    printf("\nID do jogador a EDITAR: ");
    id = lerInteiro();
    pos = buscarIndiceJogador(id);
    if (pos == -1)
    {
        printf("[ERRO] Jogador nao encontrado!\n");
        return;
    }

    imprimirCabecalhoJogadores();
    imprimirJogador(pos);

    printf("\nO que deseja alterar?\n");
    printf("1 - Nome\n");
    printf("2 - Telefone\n");
    printf("0 - Cancelar\n");
    printf("Escolha: ");
    escolha = lerInteiro();

    switch (escolha)
    {
    case 1:
        printf("Novo nome: ");
        scanf(" %49[^\n]", jogadores[pos].nome);
        limparSeparador(jogadores[pos].nome);
        break;
    case 2:
        printf("Novo telefone: ");
        scanf(" %19[^\n]", jogadores[pos].telefone);
        limparSeparador(jogadores[pos].telefone);
        break;
    case 0:
        printf("Edicao cancelada.\n");
        return;
    default:
        printf("[ERRO] Opcao invalida! Nada foi alterado.\n");
        return;
    }

    salvarJogadoresCSV();
    printf("[OK] Jogador atualizado na memoria e no arquivo CSV!\n");
}

/*
 * CAIXA: vende fichas a um jogador. O valor a pagar e a quantidade vezes
 * PRECO_FICHA. Soma as fichas no saldo, soma o valor no total gasto,
 * recalcula o nivel e regrava o CSV.
 */
// Autor: Leonardo Messias
void comprarFichas()
{
    int id, pos, quantidade;
    float valor;

    printf("\nID do jogador que esta comprando: ");
    id = lerInteiro();
    pos = buscarIndiceJogador(id);
    if (pos == -1)
    {
        printf("[ERRO] Jogador nao encontrado!\n");
        return;
    }

    printf("Preco da ficha: R$ %.2f\n", PRECO_FICHA);
    printf("Quantidade de fichas (1 a 100): ");
    quantidade = lerInteiro();
    if (quantidade < 1 || quantidade > 100)
    {
        printf("[ERRO] Quantidade invalida! Nada foi vendido.\n");
        return;
    }

    valor = quantidade * PRECO_FICHA;
    jogadores[pos].fichas += quantidade;
    jogadores[pos].totalGasto += valor;
    atualizarNivel(pos);

    salvarJogadoresCSV();
    printf("[OK] Venda de %d ficha(s): R$ %.2f.\n", quantidade, valor);
    printf("%s agora tem %d ficha(s) e nivel %c.\n",
           jogadores[pos].nome, jogadores[pos].fichas, jogadores[pos].nivel);
}

/*
 * REMOCAO: os jogadores seguintes sobem uma posicao para cobrir o buraco,
 * o contador diminui e o CSV e regravado sem o jogador removido.
 */
// Autor: Leonardo Messias
void removerJogador()
{
    int id, pos, j, confirma;

    printf("\nID do jogador a REMOVER: ");
    id = lerInteiro();
    pos = buscarIndiceJogador(id);
    if (pos == -1)
    {
        printf("[ERRO] Jogador nao encontrado!\n");
        return;
    }

    imprimirCabecalhoJogadores();
    imprimirJogador(pos);
    if (jogadores[pos].fichas > 0)
    {
        printf("Atencao: o jogador ainda tem %d ficha(s) que serao perdidas.\n",
               jogadores[pos].fichas);
    }
    printf("\nConfirma a remocao? (1 - Sim, 0 - Nao): ");
    confirma = lerInteiro();
    if (confirma != 1)
    {
        printf("Remocao cancelada.\n");
        return;
    }

    for (j = pos; j < totalJogadores - 1; j++)
    {
        jogadores[j] = jogadores[j + 1];
    }
    totalJogadores--;

    salvarJogadoresCSV();
    printf("[OK] Jogador removido do sistema e do CSV!\n");
}

/*
 * SUBMENU de Jogadores. Cada case chama uma funcao propria. A tela e
 * limpa a cada volta e o 0 devolve o controle ao menu principal.
 */
// Autor: Leonardo Messias
void moduloJogadores()
{
    int opcao;

    do
    {
        limparTela();
        printf("--- [MODULO 2: JOGADORES (Leonardo)] ---\n");
        printf("1. Cadastrar novo jogador\n");
        printf("2. Listar jogadores\n");
        printf("3. Buscar jogador por nome\n");
        printf("4. Editar dados do jogador\n");
        printf("5. Comprar fichas no caixa\n");
        printf("6. Remover jogador\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        opcao = lerInteiro();

        switch (opcao)
        {
        case 1:
            cadastrarJogador();
            pausar();
            break;
        case 2:
            listarJogadores();
            pausar();
            break;
        case 3:
            buscarJogadorPorNome();
            pausar();
            break;
        case 4:
            editarJogador();
            pausar();
            break;
        case 5:
            comprarFichas();
            pausar();
            break;
        case 6:
            removerJogador();
            pausar();
            break;
        case 0:
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo de Jogadores!\n");
            pausar();
        }
    } while (opcao != 0);
}

#endif
