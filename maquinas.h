/*
 * Arquivo: maquinas.h
 * Perspectiva do Integrante A: MAQUINAS do fliperama.
 * Struct, vetor global, leitura/escrita em maquinas.csv e CRUD completo.
 */
#ifndef MAQUINAS_H
#define MAQUINAS_H

#include <stdio.h>
#include <string.h>
#include "utilitarios.h"

/* Tamanhos fixos (o pre-processador troca cada nome pelo numero) */
#define MAX_MAQUINAS 50
#define TAM_NOME_JOGO 40

// Autor: Douglas Luan
struct Maquina
{
    int idMaquina;
    char nome[TAM_NOME_JOGO];
    char categoria; /* L-Luta, T-Tiro, C-Corrida, P-Pinball, A-Labirinto */
    int ativa;      /* 1 = Ativa, 0 = Manutencao */
    int highScore;
    char recordista[TAM_NOME_PESSOA]; /* "---" enquanto ninguem jogou */
    float receita;                    /* total arrecadado pela maquina */
};

struct Maquina maquinas[MAX_MAQUINAS];
int totalMaquinas = 0;

/*
 * PERSISTENCIA: regrava o arquivo inteiro a partir do vetor. O modo "w"
 * zera o arquivo, entao qualquer cadastro, edicao ou remocao e refletido.
 */
// Autor: Douglas Luan
void salvarMaquinasCSV()
{
    FILE *arq;
    int i;

    arq = fopen("maquinas.csv", "w");
    if (arq == NULL)
    {
        printf("[ERRO] Nao foi possivel gravar maquinas.csv!\n");
        return;
    }

    for (i = 0; i < totalMaquinas; i++)
    {
        fprintf(arq, "%d;%s;%c;%d;%d;%s;%.2f\n",
                maquinas[i].idMaquina, maquinas[i].nome,
                maquinas[i].categoria, maquinas[i].ativa,
                maquinas[i].highScore, maquinas[i].recordista,
                maquinas[i].receita);
    }
    fclose(arq);
}

/*
 * PERSISTENCIA: carrega maquinas.csv para o vetor ao abrir o programa.
 * O fscanf devolve quantos campos leu (7); se for diferente, a linha
 * acabou ou esta incompleta e o laco para. As mascaras %39[^;] e %49[^;]
 * leem texto ate o proximo ';' e limitam o tamanho (sobra 1 para o \0).
 * Texto nao leva &, pois o nome do vetor ja e o endereco dele.
 */
// Autor: Douglas Luan
void carregarMaquinasCSV()
{
    FILE *arq;

    arq = fopen("maquinas.csv", "r");
    if (arq == NULL)
    {
        return;
    }

    totalMaquinas = 0;
    while (totalMaquinas < MAX_MAQUINAS &&
           fscanf(arq, "%d;%39[^;]; %c;%d;%d;%49[^;];%f\n",
                  &maquinas[totalMaquinas].idMaquina,
                  maquinas[totalMaquinas].nome,
                  &maquinas[totalMaquinas].categoria,
                  &maquinas[totalMaquinas].ativa,
                  &maquinas[totalMaquinas].highScore,
                  maquinas[totalMaquinas].recordista,
                  &maquinas[totalMaquinas].receita) == 7)
    {
        totalMaquinas++;
    }
    fclose(arq);
}

/* Devolve a POSICAO do id no vetor, ou -1 se nao existir */
// Autor: Douglas Luan
int buscarIndiceMaquina(int idProcurado)
{
    int i;

    for (i = 0; i < totalMaquinas; i++)
    {
        if (maquinas[i].idMaquina == idProcurado)
        {
            return i;
        }
    }
    return -1;
}

/* Cabecalho da tabela de maquinas */
// Autor: Douglas Luan
void imprimirCabecalhoMaquinas()
{
    printf("\n%-4s %-24s %-4s %-11s %-9s %-20s %s\n",
           "ID", "Jogo", "Cat", "Status", "Recorde", "Recordista", "Receita");
}

/* Imprime UMA maquina (pela posicao no vetor) */
// Autor: Douglas Luan
void imprimirMaquina(int pos)
{
    printf("%-4d %-24s %-4c %-11s %-9d %-20s R$ %.2f\n",
           maquinas[pos].idMaquina, maquinas[pos].nome,
           maquinas[pos].categoria,
           maquinas[pos].ativa == 1 ? "Ativa" : "Manutencao",
           maquinas[pos].highScore, maquinas[pos].recordista,
           maquinas[pos].receita);
}

/*
 * Pergunta a categoria e devolve a letra em maiuscula.
 * Devolve '?' se a letra digitada nao for uma categoria valida.
 */
// Autor: Douglas Luan
char lerCategoria()
{
    char c;

    printf("Categoria (L-Luta, T-Tiro, C-Corrida, P-Pinball, A-Labirinto): ");
    scanf(" %c", &c);

    /* Letras minusculas ficam maiusculas pela distancia na tabela ASCII */
    if (c >= 'a' && c <= 'z')
    {
        c = c - 'a' + 'A';
    }
    if (c != 'L' && c != 'T' && c != 'C' && c != 'P' && c != 'A')
    {
        return '?';
    }
    return c;
}

/*
 * CADASTRO: valida o limite do vetor e bloqueia ID repetido. A maquina
 * nova comeca ativa, sem recorde e sem receita.
 */
// Autor: Douglas Luan
void cadastrarMaquina()
{
    int id;
    char categoria;

    if (totalMaquinas >= MAX_MAQUINAS)
    {
        printf("[ERRO] Limite de %d maquinas atingido!\n", MAX_MAQUINAS);
        return;
    }

    printf("\nID da nova maquina: ");
    id = lerInteiro();
    if (id <= 0)
    {
        printf("[ERRO] O ID deve ser maior que zero!\n");
        return;
    }
    if (buscarIndiceMaquina(id) != -1)
    {
        printf("[ERRO] Ja existe uma maquina com o ID %d!\n", id);
        return;
    }

    /* O espaco antes do % descarta o ENTER que sobrou da leitura anterior;
       [^\n] aceita nomes com espaco; 39 evita estourar nome[40] */
    printf("Nome do jogo: ");
    scanf(" %39[^\n]", maquinas[totalMaquinas].nome);
    limparSeparador(maquinas[totalMaquinas].nome);

    categoria = lerCategoria();
    if (categoria == '?')
    {
        printf("[ERRO] Categoria invalida! Maquina nao cadastrada.\n");
        return;
    }

    maquinas[totalMaquinas].idMaquina = id;
    maquinas[totalMaquinas].categoria = categoria;
    maquinas[totalMaquinas].ativa = 1;
    maquinas[totalMaquinas].highScore = 0;
    strcpy(maquinas[totalMaquinas].recordista, "---");
    maquinas[totalMaquinas].receita = 0;
    totalMaquinas++;

    salvarMaquinasCSV();
    printf("[OK] Maquina cadastrada e salva em 'maquinas.csv'!\n");
}

/* LISTAGEM formatada de todas as maquinas */
// Autor: Douglas Luan
void listarMaquinas()
{
    int i;

    printf("\n--- MAQUINAS (%d cadastradas) ---", totalMaquinas);
    if (totalMaquinas == 0)
    {
        printf("\nNenhuma maquina cadastrada.\n");
        return;
    }

    imprimirCabecalhoMaquinas();
    for (i = 0; i < totalMaquinas; i++)
    {
        imprimirMaquina(i);
    }
}

/*
 * BUSCA TEXTUAL: strstr procura o texto digitado dentro do nome. Ela
 * devolve o endereco onde o texto comecou, ou NULL se nao achou.
 * Varias maquinas podem ter o mesmo jogo, entao o vetor todo e percorrido.
 */
// Autor: Douglas Luan
void buscarMaquinaPorNome()
{
    char termo[30];
    int i;
    int encontradas = 0;

    printf("\nDigite parte do nome do jogo: ");
    scanf(" %29[^\n]", termo);

    for (i = 0; i < totalMaquinas; i++)
    {
        if (strstr(maquinas[i].nome, termo) != NULL)
        {
            if (encontradas == 0)
            {
                imprimirCabecalhoMaquinas();
            }
            imprimirMaquina(i);
            encontradas++;
        }
    }

    if (encontradas == 0)
    {
        printf("Nenhuma maquina encontrada com \"%s\".\n", termo);
    }
    else
    {
        printf("%d maquina(s) encontrada(s).\n", encontradas);
    }
}

/*
 * EDICAO: altera nome, categoria ou status na memoria e depois regrava
 * o CSV inteiro. O recorde e a receita nao sao editados aqui: eles
 * mudam sozinhos quando uma partida e registrada.
 */
// Autor: Douglas Luan
void editarMaquina()
{
    int id, pos, escolha, status;
    char categoria;

    printf("\nID da maquina a EDITAR: ");
    id = lerInteiro();
    pos = buscarIndiceMaquina(id);
    if (pos == -1)
    {
        printf("[ERRO] Maquina nao encontrada!\n");
        return;
    }

    imprimirCabecalhoMaquinas();
    imprimirMaquina(pos);

    printf("\nO que deseja alterar?\n");
    printf("1 - Nome do jogo\n");
    printf("2 - Categoria\n");
    printf("3 - Status (Ativa/Manutencao)\n");
    printf("0 - Cancelar\n");
    printf("Escolha: ");
    escolha = lerInteiro();

    switch (escolha)
    {
    case 1:
        printf("Novo nome do jogo: ");
        scanf(" %39[^\n]", maquinas[pos].nome);
        limparSeparador(maquinas[pos].nome);
        break;
    case 2:
        categoria = lerCategoria();
        if (categoria == '?')
        {
            printf("[ERRO] Categoria invalida! Nada foi alterado.\n");
            return;
        }
        maquinas[pos].categoria = categoria;
        break;
    case 3:
        printf("Novo status (1 - Ativa, 0 - Manutencao): ");
        status = lerInteiro();
        if (status != 0 && status != 1)
        {
            printf("[ERRO] Status invalido! Nada foi alterado.\n");
            return;
        }
        maquinas[pos].ativa = status;
        break;
    case 0:
        printf("Edicao cancelada.\n");
        return;
    default:
        printf("[ERRO] Opcao invalida! Nada foi alterado.\n");
        return;
    }

    salvarMaquinasCSV();
    printf("[OK] Maquina atualizada na memoria e no arquivo CSV!\n");
}

/*
 * REMOCAO: as maquinas seguintes sobem uma posicao para cobrir o buraco,
 * o contador diminui e o CSV e regravado sem a maquina removida.
 */
// Autor: Douglas Luan
void removerMaquina()
{
    int id, pos, j, confirma;

    printf("\nID da maquina a REMOVER: ");
    id = lerInteiro();
    pos = buscarIndiceMaquina(id);
    if (pos == -1)
    {
        printf("[ERRO] Maquina nao encontrada!\n");
        return;
    }

    imprimirCabecalhoMaquinas();
    imprimirMaquina(pos);
    printf("\nConfirma a remocao? (1 - Sim, 0 - Nao): ");
    confirma = lerInteiro();
    if (confirma != 1)
    {
        printf("Remocao cancelada.\n");
        return;
    }

    for (j = pos; j < totalMaquinas - 1; j++)
    {
        maquinas[j] = maquinas[j + 1];
    }
    totalMaquinas--;

    salvarMaquinasCSV();
    printf("[OK] Maquina removida do sistema e do CSV!\n");
}

/*
 * SUBMENU de Maquinas. Cada case chama uma funcao propria. A tela e
 * limpa a cada volta e o 0 devolve o controle ao menu principal.
 */
// Autor: Douglas Luan
void moduloMaquinas()
{
    int opcao;

    do
    {
        limparTela();
        printf("--- [MODULO 1: MAQUINAS (Douglas)] ---\n");
        printf("1. Cadastrar nova maquina\n");
        printf("2. Listar maquinas\n");
        printf("3. Buscar maquina por nome\n");
        printf("4. Editar maquina\n");
        printf("5. Remover maquina\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        opcao = lerInteiro();

        switch (opcao)
        {
        case 1:
            cadastrarMaquina();
            pausar();
            break;
        case 2:
            listarMaquinas();
            pausar();
            break;
        case 3:
            buscarMaquinaPorNome();
            pausar();
            break;
        case 4:
            editarMaquina();
            pausar();
            break;
        case 5:
            removerMaquina();
            pausar();
            break;
        case 0:
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo de Maquinas!\n");
            pausar();
        }
    } while (opcao != 0);
}

#endif
