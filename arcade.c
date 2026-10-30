/*
 * Projeto Integrador (P2) - Sistema de Gestao de Fliperama (CRUD em C)
 * Disciplina: Algoritmos e Pensamento Computacional
 *
 * Integrante A: Douglas Luan
 * Modulo: Insercao e Cadastro (validacao de limites na RAM)
 *
 * Integrante B: Leonardo Messias
 * Modulo: Busca e Modificacao (atualizacao de status e dados)
 *
 * Integrante C: Douglas Luan
 * Modulo: Remocao e Matriz (grade de controle do tema)
 *
 * Integrante D: Eduardo Pan
 * Modulo: Relatorios e Ordenacao (calculos analiticos e Bubble Sort)
 *
 * REGRA DE INTEGRACAO: cada integrante escreve SOMENTE dentro da sua
 * regiao marcada (funcoes e submenu do modulo). Nao use "Formatar codigo"
 * da IDE no arquivo inteiro, para nao gerar conflitos no Git.
 */

#include <stdio.h>

/* Limite de maquinas que cabem no vetor (capacidade da RAM) */
#define MAX_MAQUINAS 50
#define TAM_NOME 50
#define TAM_RECORDISTA 30

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

struct Caixa
{
    float precoFicha;
    float totalArrecadado;
    int fichasVendidas;
    int fichasEmMaos;
};

struct Maquina maquinas[MAX_MAQUINAS] = {
    {1, "Street Fighter II", 12, 15000, "Ana", 1, 30.0},
    {2, "Street Fighter II", 9, 11800, "Diego", 1, 22.5},
    {3, "Pac-Man", 7, 8200, "Bruno", 1, 17.5},
    {4, "Pac-Man", 15, 9600, "Elisa", 1, 37.5},
    {5, "Pinball", 20, 23400, "Carla", 1, 50.0},
    {6, "Pinball", 4, 17500, "Felipe", 1, 10.0},
    {7, "Street Fighter II", 18, 13200, "Gabi", 0, 45.0},
    {8, "Pac-Man", 11, 7400, "Hugo", 0, 27.5},
    {9, "Pinball", 6, 20100, "Iara", 0, 15.0},
    {10, "Street Fighter II", 3, 5900, "Joao", 0, 7.5}};

#define DIAS_SEMANA 7

/* Fichas inseridas por maquina em cada dia (Dom, Seg, ..., Sab).
   A soma de cada linha e igual ao fichasInseridas da maquina. */
int usoSemanal[MAX_MAQUINAS][DIAS_SEMANA] = {
    {1, 0, 1, 2, 1, 3, 4},
    {0, 1, 0, 1, 2, 1, 4},
    {0, 1, 1, 0, 1, 1, 3},
    {2, 1, 2, 1, 2, 3, 4},
    {2, 1, 2, 2, 3, 4, 6},
    {0, 0, 1, 0, 1, 0, 2},
    {2, 2, 1, 2, 3, 3, 5},
    {1, 0, 2, 1, 1, 2, 4},
    {0, 1, 0, 1, 1, 1, 2},
    {0, 0, 1, 0, 0, 1, 1}};

/* Quantas posicoes do vetor estao realmente em uso */
int totalMaquinas = 10;

/* Proximo id a ser usado no cadastro. So aumenta, assim nunca repete
   um id, mesmo depois de o modulo C remover uma maquina */
int proximoId = 11;

struct Caixa caixa = {2.5, 262.5, 105, 0};

/* Prototipos de funcoes do modulo B usadas antes da sua definicao */
int lerInteiroB(const char *mensagem);
int buscarIndicePorIdB(int id);

/* ============================================================
 * INICIO DO MODULO A - Insercao e Cadastro (Douglas Luan)
 * ============================================================ */

/*
 * Modulo A: cadastra uma nova maquina no vetor.
 * Antes de gravar, valida o limite do vetor para nao escrever fora
 * da memoria reservada.
 */
// Autor: Douglas Luan
void cadastrarMaquina(void)
{
    struct Maquina nova;

    /* Validacao de limite: se o vetor esta cheio, nao cadastra */
    if (totalMaquinas >= MAX_MAQUINAS)
    {
        printf("Limite de maquinas atingido (%d).\n", MAX_MAQUINAS);
        return;
    }

    nova.id = proximoId;

    /* O espaco antes do % descarta o Enter que sobrou da leitura
       anterior; [^\n] aceita nomes com espaco; 49 evita estourar
       o tamanho de nome[50] */
    printf("Nome do jogo: ");
    scanf(" %49[^\n]", nova.nome);

    nova.fichasInseridas = 0;
    nova.highScore = 0;
    nova.recordista[0] = '\0';
    nova.ativa = 1;
    nova.receita = 0;

    /* Grava na primeira posicao livre e atualiza os contadores */
    maquinas[totalMaquinas] = nova;
    totalMaquinas++;
    proximoId++;

    printf("Maquina cadastrada com ID %d.\n", nova.id);
}

// Autor: Douglas Luan
void listarMaquinas(void)
{
    int i;

    printf("\nID   Jogo                 Fichas  Recorde  Recordista  Status\n");
    for (i = 0; i < totalMaquinas; i++)
    {
        printf("%-4d %-20s %-7d %-8d ",
               maquinas[i].id, maquinas[i].nome,
               maquinas[i].fichasInseridas, maquinas[i].highScore);

        if (maquinas[i].recordista[0] == '\0')
        {
            printf("%-11s ", "---");
        }
        else
        {
            printf("%-11s ", maquinas[i].recordista);
        }

        if (maquinas[i].ativa == 1)
        {
            printf("Ativa\n");
        }
        else
        {
            printf("Manutencao\n");
        }
    }
}

/*
 * Modulo A: vende fichas no caixa. O valor a pagar e a quantidade
 * multiplicada pelo preco da ficha.
 */
// Autor: Douglas Luan
void comprarFichas(void)
{
    int quantidade;
    float valor;

    quantidade = lerInteiroB("Quantidade de fichas (1 a 100): ");
    if (quantidade < 1 || quantidade > 100)
    {
        printf("Quantidade invalida. Nada foi vendido.\n");
        return;
    }

    valor = quantidade * caixa.precoFicha;
    caixa.fichasEmMaos += quantidade;
    caixa.fichasVendidas += quantidade;
    caixa.totalArrecadado += valor;

    printf("Venda de %d ficha(s): R$ %.2f. Fichas em maos: %d.\n",
           quantidade, valor, caixa.fichasEmMaos);
}

/*
 * Modulo A: gasta uma ficha numa maquina ativa. Atualiza as fichas
 * da maquina, a receita, a matriz de uso semanal e, se a pontuacao
 * superar o recorde, o high score e o recordista.
 */
// Autor: Douglas Luan
void jogarMaquina(void)
{
    int id, pos, dia, pontos;

    if (caixa.fichasEmMaos == 0)
    {
        printf("Voce nao tem fichas. Compre fichas no caixa primeiro.\n");
        return;
    }

    id = lerInteiroB("ID da maquina: ");
    pos = buscarIndicePorIdB(id);

    if (pos == -1)
    {
        printf("Maquina com ID %d nao encontrada.\n", id);
        return;
    }
    if (maquinas[pos].ativa == 0)
    {
        printf("A maquina %d esta em manutencao.\n", id);
        return;
    }

    dia = lerInteiroB("Dia (0-Dom 1-Seg 2-Ter 3-Qua 4-Qui 5-Sex 6-Sab): ");
    if (dia < 0 || dia >= DIAS_SEMANA)
    {
        printf("Dia invalido. A ficha nao foi gasta.\n");
        return;
    }

    pontos = lerInteiroB("Pontuacao obtida: ");
    if (pontos < 0)
    {
        printf("Pontuacao invalida. A ficha nao foi gasta.\n");
        return;
    }

    caixa.fichasEmMaos--;
    maquinas[pos].fichasInseridas++;
    maquinas[pos].receita += caixa.precoFicha;
    usoSemanal[pos][dia]++;

    if (pontos > maquinas[pos].highScore)
    {
        maquinas[pos].highScore = pontos;
        printf("Novo recorde! Nome do recordista: ");
        scanf(" %29[^\n]", maquinas[pos].recordista);
    }
    else
    {
        printf("O recorde da maquina continua em %d pontos.\n",
               maquinas[pos].highScore);
    }
}

/* Modulo A: mostra os dados do caixa */
// Autor: Douglas Luan
void consultarCaixa(void)
{
    printf("\n========== CAIXA ==========\n");
    printf("Preco da ficha ..........: R$ %.2f\n", caixa.precoFicha);
    printf("Fichas vendidas .........: %d\n", caixa.fichasVendidas);
    printf("Total arrecadado ........: R$ %.2f\n", caixa.totalArrecadado);
    printf("Fichas em maos ..........: %d\n", caixa.fichasEmMaos);
    printf("===========================\n");
}

/*
 * Submenu do Modulo A: cadastro de maquinas e operacoes do caixa.
 */
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
        opcao = lerInteiroB("Escolha uma acao: ");

        switch (opcao)
        {
        case 1:
            cadastrarMaquina();
            break;
        case 2:
            listarMaquinas();
            break;
        case 3:
            comprarFichas();
            break;
        case 4:
            jogarMaquina();
            break;
        case 5:
            consultarCaixa();
            break;
        case 0:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo A!\n");
        }
    } while (opcao != 0);
}

/* ===================== FIM DO MODULO A ==================== */

/* ============================================================
 * INICIO DO MODULO B - Busca e Modificacao (Leonardo Messias)
 * Escreva suas funcoes SOMENTE entre estas linhas.
 * ============================================================ */

/*
 * Modulo B: le um numero inteiro com seguranca.
 * Se o usuario digitar letras, o scanf falha e o texto ficaria no
 * buffer, causando loop infinito. Aqui o buffer e limpo e a pergunta
 * e repetida ate a entrada ser valida.
 */
// Autor: Leonardo Messias
int lerInteiroB(const char *mensagem)
{
    int valor;
    int c;

    printf("%s", mensagem);
    while (scanf("%d", &valor) != 1)
    {
        /* Descarta tudo ate o fim da linha */
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
        if (c == EOF)
        {
            return 0;
        }
        printf("Entrada invalida. %s", mensagem);
    }
    return valor;
}

/*
 * Modulo B: busca linear pelo id.
 * Retorna a POSICAO no vetor (0 a totalMaquinas-1) ou -1 se nao achar.
 * O id NAO e a posicao: o modulo C pode remover maquinas e deixar os
 * ids "pulando" numeros, por isso e preciso comparar o campo id.
 */
// Autor: Leonardo Messias
int buscarIndicePorIdB(int id)
{
    int i;

    for (i = 0; i < totalMaquinas; i++)
    {
        if (maquinas[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

/* Modulo B: converte letra maiuscula em minuscula (sem ctype.h) */
// Autor: Leonardo Messias
char minusculaB(char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + ('a' - 'A');
    }
    return c;
}

/*
 * Modulo B: verifica se 'trecho' aparece dentro de 'texto', ignorando
 * maiusculas/minusculas. Ex.: "pac" encontra "Pac-Man".
 * Retorna 1 se encontrou e 0 se nao.
 */
// Autor: Leonardo Messias
int contemTrechoB(const char *texto, const char *trecho)
{
    int i, j;

    /* Trecho vazio nao busca nada */
    if (trecho[0] == '\0')
    {
        return 0;
    }

    for (i = 0; texto[i] != '\0'; i++)
    {
        j = 0;
        while (trecho[j] != '\0' && texto[i + j] != '\0' &&
               minusculaB(texto[i + j]) == minusculaB(trecho[j]))
        {
            j++;
        }
        /* Chegou ao fim do trecho: todas as letras bateram */
        if (trecho[j] == '\0')
        {
            return 1;
        }
    }
    return 0;
}

/* Modulo B: imprime UMA maquina (pela posicao no vetor) */
// Autor: Leonardo Messias
void mostrarMaquinaB(int i)
{
    printf("%-4d %-20s %-7d %-8d ",
           maquinas[i].id, maquinas[i].nome,
           maquinas[i].fichasInseridas, maquinas[i].highScore);

    if (maquinas[i].recordista[0] == '\0')
    {
        printf("%-11s ", "---");
    }
    else
    {
        printf("%-11s ", maquinas[i].recordista);
    }

    if (maquinas[i].ativa == 1)
    {
        printf("Ativa\n");
    }
    else
    {
        printf("Manutencao\n");
    }
}

/* Modulo B: cabecalho da tabela (igual ao do listarMaquinas do A) */
// Autor: Leonardo Messias
void cabecalhoB(void)
{
    printf("\nID   Jogo                 Fichas  Recorde  Recordista  Status\n");
}

/*
 * Modulo B: busca uma maquina pelo ID e mostra os dados.
 */
// Autor: Leonardo Messias
void buscarMaquinaPorId(void)
{
    int id, pos;

    id = lerInteiroB("ID da maquina: ");
    pos = buscarIndicePorIdB(id);

    if (pos == -1)
    {
        printf("Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    cabecalhoB();
    mostrarMaquinaB(pos);
}

/*
 * Modulo B: busca maquinas pelo nome (ou parte dele).
 * Varias maquinas podem ter o mesmo jogo, entao o vetor inteiro e
 * percorrido e TODAS as correspondencias sao mostradas.
 */
// Autor: Leonardo Messias
void buscarMaquinaPorNome(void)
{
    char trecho[TAM_NOME];
    int encontradas = 0;
    int i;

    printf("Nome (ou parte do nome) do jogo: ");
    scanf(" %49[^\n]", trecho);

    for (i = 0; i < totalMaquinas; i++)
    {
        if (contemTrechoB(maquinas[i].nome, trecho))
        {
            /* Imprime o cabecalho so antes da primeira correspondencia */
            if (encontradas == 0)
            {
                cabecalhoB();
            }
            mostrarMaquinaB(i);
            encontradas++;
        }
    }

    if (encontradas == 0)
    {
        printf("Nenhuma maquina encontrada com \"%s\".\n", trecho);
    }
    else
    {
        printf("%d maquina(s) encontrada(s).\n", encontradas);
    }
}

/*
 * Modulo B: modifica os dados de uma maquina (nome ou recorde).
 * Primeiro localiza pelo ID; so altera se ela existir.
 */
// Autor: Leonardo Messias
void modificarMaquina(void)
{
    int id, pos, escolha, novoScore;

    id = lerInteiroB("ID da maquina a modificar: ");
    pos = buscarIndicePorIdB(id);

    if (pos == -1)
    {
        printf("Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    /* Mostra os dados atuais para o usuario conferir */
    cabecalhoB();
    mostrarMaquinaB(pos);

    printf("\nO que deseja alterar?\n");
    printf("1 - Nome do jogo\n");
    printf("2 - Recorde e recordista\n");
    printf("0 - Cancelar\n");
    escolha = lerInteiroB("Escolha: ");

    switch (escolha)
    {
    case 1:
        printf("Novo nome do jogo: ");
        scanf(" %49[^\n]", maquinas[pos].nome);
        printf("Nome atualizado.\n");
        break;

    case 2:
        novoScore = lerInteiroB("Novo recorde (pontos): ");

        /* Validacao: pontuacao negativa nao faz sentido */
        if (novoScore < 0)
        {
            printf("Recorde invalido. Nada foi alterado.\n");
            break;
        }

        maquinas[pos].highScore = novoScore;
        printf("Nome do recordista: ");
        scanf(" %29[^\n]", maquinas[pos].recordista);
        printf("Recorde atualizado.\n");
        break;

    case 0:
        printf("Modificacao cancelada.\n");
        break;

    default:
        printf("Opcao invalida. Nada foi alterado.\n");
    }
}

/*
 * Modulo B: altera o status da maquina (Ativa/Manutencao).
 * O status e o campo 'ativa': 1 = Ativa, 0 = Manutencao.
 */
// Autor: Leonardo Messias
void alterarStatusMaquina(void)
{
    int id, pos, escolha;

    id = lerInteiroB("ID da maquina: ");
    pos = buscarIndicePorIdB(id);

    if (pos == -1)
    {
        printf("Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    printf("Status atual da maquina %d (%s): %s\n",
           maquinas[pos].id, maquinas[pos].nome,
           maquinas[pos].ativa == 1 ? "Ativa" : "Manutencao");

    printf("1 - Colocar como Ativa\n");
    printf("2 - Colocar em Manutencao\n");
    printf("0 - Cancelar\n");
    escolha = lerInteiroB("Escolha: ");

    if (escolha == 1)
    {
        maquinas[pos].ativa = 1;
        printf("Maquina %d agora esta Ativa.\n", maquinas[pos].id);
    }
    else if (escolha == 2)
    {
        maquinas[pos].ativa = 0;
        printf("Maquina %d agora esta em Manutencao.\n", maquinas[pos].id);
    }
    else if (escolha == 0)
    {
        printf("Alteracao cancelada.\n");
    }
    else
    {
        printf("Opcao invalida. Nada foi alterado.\n");
    }
}

/*
 * Submenu do Modulo B: chama as rotinas de busca e modificacao.
 */
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
        opcao = lerInteiroB("Escolha uma acao: ");

        switch (opcao)
        {
        case 1:
            buscarMaquinaPorId();
            break;
        case 2:
            buscarMaquinaPorNome();
            break;
        case 3:
            modificarMaquina();
            break;
        case 4:
            alterarStatusMaquina();
            break;
        case 0:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo B!\n");
        }
    } while (opcao != 0);
}

/* ===================== FIM DO MODULO B ==================== */

/* ============================================================
 * INICIO DO MODULO C - Remocao e Matriz (Douglas Luan)
 * A matriz de uso semanal acompanha o vetor maquinas[]: a linha i
 * da matriz pertence a maquina que esta na posicao i do vetor.
 * ============================================================ */

/*
 * Modulo C: remove uma maquina pelo ID.
 * Para nao deixar buraco no vetor, as maquinas seguintes sobem uma
 * posicao e totalMaquinas diminui. A matriz sobe junto, senao as
 * linhas ficariam desencontradas das maquinas.
 */
// Autor: Douglas Luan
void removerMaquina(void)
{
    int id, pos, confirma, i, d;

    if (totalMaquinas == 0)
    {
        printf("Nenhuma maquina cadastrada para remover.\n");
        return;
    }

    id = lerInteiroB("ID da maquina a remover: ");
    pos = buscarIndicePorIdB(id);

    if (pos == -1)
    {
        printf("Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    cabecalhoB();
    mostrarMaquinaB(pos);

    confirma = lerInteiroB("Confirma a remocao? (1 - Sim, 0 - Nao): ");
    if (confirma != 1)
    {
        printf("Remocao cancelada.\n");
        return;
    }

    for (i = pos; i < totalMaquinas - 1; i++)
    {
        maquinas[i] = maquinas[i + 1];
        for (d = 0; d < DIAS_SEMANA; d++)
        {
            usoSemanal[i][d] = usoSemanal[i + 1][d];
        }
    }

    /* A ultima linha ficou repetida: zera para uma futura maquina
       cadastrada nessa posicao comecar sem uso */
    for (d = 0; d < DIAS_SEMANA; d++)
    {
        usoSemanal[totalMaquinas - 1][d] = 0;
    }

    totalMaquinas--;
    printf("Maquina %d removida.\n", id);
}

/*
 * Modulo C: mostra a matriz de uso semanal (maquinas x dias da
 * semana) com o total de cada maquina e o total de cada dia.
 */
// Autor: Douglas Luan
void mostrarMatrizUso(void)
{
    int totalDia[DIAS_SEMANA] = {0};
    int totalGeral = 0;
    int totalMaquina;
    int i, d;

    if (totalMaquinas == 0)
    {
        printf("Nenhuma maquina cadastrada para mostrar a matriz.\n");
        return;
    }

    printf("\n========== USO SEMANAL (FICHAS POR DIA) ==========\n");
    printf("ID   Jogo                 Dom Seg Ter Qua Qui Sex Sab Total\n");

    for (i = 0; i < totalMaquinas; i++)
    {
        totalMaquina = 0;

        printf("%-4d %-20s", maquinas[i].id, maquinas[i].nome);
        for (d = 0; d < DIAS_SEMANA; d++)
        {
            printf("%4d", usoSemanal[i][d]);
            totalMaquina += usoSemanal[i][d];
            totalDia[d] += usoSemanal[i][d];
        }
        printf("%6d\n", totalMaquina);
        totalGeral += totalMaquina;
    }

    printf("%-25s", "Total do dia");
    for (d = 0; d < DIAS_SEMANA; d++)
    {
        printf("%4d", totalDia[d]);
    }
    printf("%6d\n", totalGeral);
    printf("==================================================\n");
}

/*
 * Submenu do Modulo C: chama as rotinas de remocao e da matriz.
 */
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
        opcao = lerInteiroB("Escolha uma acao: ");

        switch (opcao)
        {
        case 1:
            removerMaquina();
            break;
        case 2:
            mostrarMatrizUso();
            break;
        case 0:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo C!\n");
        }
    } while (opcao != 0);
}

/* ===================== FIM DO MODULO C ==================== */

/* ============================================================
 * INICIO DO MODULO D - Relatorios e Ordenacao (Eduardo Pan)
 * Este modulo so LE os dados globais (nao altera o vetor), entao
 * nao depende das funcoes de B e C: usa apenas maquinas[] e
 * totalMaquinas.
 * ============================================================ */

/*
 * Modulo D: auxiliar que imprime uma maquina em uma linha.
 * Recebe a maquina como parametro (copia), sem alterar o vetor.
 */
// Autor: Eduardo Pan
void imprimirLinhaMaquina(struct Maquina m)
{
    printf("[%2d] %-18s | Score: %6d | Recordista: ",
           m.id, m.nome, m.highScore);

    if (m.recordista[0] == '\0')
    {
        printf("---");
    }
    else
    {
        printf("%s", m.recordista);
    }

    if (m.ativa == 1)
    {
        printf(" | Ativa\n");
    }
    else
    {
        printf(" | Manutencao\n");
    }
}

/*
 * Modulo D: relatorio geral com totais, extremos de high score,
 * media e situacao das maquinas.
 */
// Autor: Eduardo Pan
void relatorioGeral(void)
{
    int totalFichas = 0;
    int somaScores = 0;
    int ativas = 0;
    int manutencao = 0;
    int posMaior, posMenor, posMaisJogada;
    int i;
    float media;
    float receitaTotal = 0;

    if (totalMaquinas == 0)
    {
        printf("Nenhuma maquina cadastrada para gerar relatorio.\n");
        return;
    }

    /* A primeira maquina real assume a lideranca (nunca comecar em 0) */
    posMaior = 0;
    posMenor = 0;
    posMaisJogada = 0;

    for (i = 0; i < totalMaquinas; i++)
    {
        totalFichas += maquinas[i].fichasInseridas;
        somaScores += maquinas[i].highScore;
        receitaTotal += maquinas[i].receita;

        if (maquinas[i].ativa == 1)
        {
            ativas++;
        }
        else
        {
            manutencao++;
        }

        if (maquinas[i].highScore > maquinas[posMaior].highScore)
        {
            posMaior = i;
        }
        if (maquinas[i].highScore < maquinas[posMenor].highScore)
        {
            posMenor = i;
        }
        if (maquinas[i].fichasInseridas >
            maquinas[posMaisJogada].fichasInseridas)
        {
            posMaisJogada = i;
        }
    }

    /* O cast para float evita a divisao inteira */
    media = (float)somaScores / totalMaquinas;

    printf("\n========== RELATORIO GERAL ==========\n");
    printf("Total de maquinas .......: %d\n", totalMaquinas);
    printf("Ativas / Manutencao .....: %d / %d\n", ativas, manutencao);
    printf("Total de fichas inseridas: %d\n", totalFichas);
    printf("Receita das maquinas ....: R$ %.2f\n", receitaTotal);
    printf("Media dos high scores ...: %.2f\n", media);

    printf("\nMaior high score:\n  ");
    imprimirLinhaMaquina(maquinas[posMaior]);

    printf("Menor high score:\n  ");
    imprimirLinhaMaquina(maquinas[posMenor]);

    printf("Maquina mais jogada (%d fichas):\n  ",
           maquinas[posMaisJogada].fichasInseridas);
    imprimirLinhaMaquina(maquinas[posMaisJogada]);
    printf("=====================================\n");
}

/*
 * Modulo D: ranking de high scores (maior para menor) com Bubble
 * Sort. Ordena uma COPIA do vetor, entao as posicoes do vetor
 * original (usadas pelos modulos A, B e C) nao mudam.
 */
// Autor: Eduardo Pan
void rankingPorHighScore(void)
{
    struct Maquina copia[MAX_MAQUINAS];
    struct Maquina aux;
    int i, j;

    if (totalMaquinas == 0)
    {
        printf("Nenhuma maquina cadastrada para gerar ranking.\n");
        return;
    }

    /* 1. Copia o vetor original */
    for (i = 0; i < totalMaquinas; i++)
    {
        copia[i] = maquinas[i];
    }

    /* 2. Bubble Sort decrescente por highScore */
    for (i = 0; i < totalMaquinas - 1; i++)
    {
        for (j = 0; j < totalMaquinas - 1 - i; j++)
        {
            if (copia[j].highScore < copia[j + 1].highScore)
            {
                aux = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = aux;
            }
        }
    }

    /* 3. Mostra o ranking */
    printf("\n========== RANKING DE HIGH SCORES ==========\n");
    for (i = 0; i < totalMaquinas; i++)
    {
        printf("%2d lugar - ", i + 1);
        imprimirLinhaMaquina(copia[i]);
    }
    printf("============================================\n");
}

/*
 * Submenu do Modulo D: chama o relatorio geral e o ranking.
 */
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
        opcao = lerInteiroB("Escolha uma acao: ");

        switch (opcao)
        {
        case 1:
            relatorioGeral();
            break;
        case 2:
            rankingPorHighScore();
            break;
        case 0:
            printf("\nRetornando ao menu principal...\n");
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo D!\n");
        }
    } while (opcao != 0);
}

/* ===================== FIM DO MODULO D ==================== */

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
        opcao = lerInteiroB("Escolha: ");

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