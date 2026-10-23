/*
 * Projeto Integrador (P2) - Sistema de Gestao de Fliperama (CRUD em C)
 * Disciplina: Algoritmos e Pensamento Computacional
 *
 * Integrante A: Douglas Luan
 * Modulo: Insercao e Cadastro (validacao de limites na RAM)
 *
 * Integrante B: (pendente) - Busca e Modificacao
 * Integrante C: (pendente) - Remocao e Matriz
 *
 * Integrante D: Eduardo
 * Modulo: Relatorios e Ordenacao (calculos analiticos e Bubble Sort)
 *
 * Neste arquivo, a funcao cadastrarMaquina() e a parte do integrante A.
 * A funcao listarMaquinas() e o main() com o menu sao provisorios e
 * servem apenas para testar o cadastro.
 *
 * REGRA DE INTEGRACAO: cada integrante escreve SOMENTE dentro da sua
 * regiao marcada (funcoes e linhas do menu). Nao use "Formatar codigo"
 * da IDE no arquivo inteiro, para nao gerar conflitos no Git.
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

/* ============================================================
 * INICIO DO MODULO B - Busca e Modificacao (Integrante B)
 * Escreva suas funcoes SOMENTE entre estas linhas.
 * ============================================================ */

/*
 * Modulo B: le um numero inteiro com seguranca.
 * Se o usuario digitar letras, o scanf falha e o texto ficaria no
 * buffer, causando loop infinito. Aqui o buffer e limpo e a pergunta
 * e repetida ate a entrada ser valida.
 */
int lerInteiroB(const char *mensagem) {
    int valor;
    int c;

    printf("%s", mensagem);
    while (scanf("%d", &valor) != 1) {
        /* Descarta tudo ate o fim da linha */
        while ((c = getchar()) != '\n' && c != EOF) {
        }
        if (c == EOF) {
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
int buscarIndicePorIdB(int id) {
    for (int i = 0; i < totalMaquinas; i++) {
        if (maquinas[i].id == id) {
            return i;
        }
    }
    return -1;
}

/* Modulo B: converte letra maiuscula em minuscula (sem ctype.h) */
char minusculaB(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

/*
 * Modulo B: verifica se 'trecho' aparece dentro de 'texto', ignorando
 * maiusculas/minusculas. Ex.: "pac" encontra "Pac-Man".
 * Retorna 1 se encontrou e 0 se nao.
 */
int contemTrechoB(const char *texto, const char *trecho) {
    /* Trecho vazio nao busca nada */
    if (trecho[0] == '\0') {
        return 0;
    }

    for (int i = 0; texto[i] != '\0'; i++) {
        int j = 0;
        while (trecho[j] != '\0' && texto[i + j] != '\0' &&
               minusculaB(texto[i + j]) == minusculaB(trecho[j])) {
            j++;
        }
        /* Chegou ao fim do trecho: todas as letras bateram */
        if (trecho[j] == '\0') {
            return 1;
        }
    }
    return 0;
}

/* Modulo B: imprime UMA maquina (pela posicao no vetor) */
void mostrarMaquinaB(int i) {
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

/* Modulo B: cabecalho da tabela (igual ao do listarMaquinas do A) */
void cabecalhoB(void) {
    printf("\nID   Jogo                 Fichas  Recorde  Recordista  Status\n");
}

/*
 * Modulo B (opcao 3): busca uma maquina pelo ID e mostra os dados.
 */
void buscarMaquinaPorId(void) {
    int id = lerInteiroB("ID da maquina: ");
    int pos = buscarIndicePorIdB(id);

    if (pos == -1) {
        printf("Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    cabecalhoB();
    mostrarMaquinaB(pos);
}

/*
 * Modulo B (opcao 4): busca maquinas pelo nome (ou parte dele).
 * Varias maquinas podem ter o mesmo jogo, entao o vetor inteiro e
 * percorrido e TODAS as correspondencias sao mostradas.
 */
void buscarMaquinaPorNome(void) {
    char trecho[TAM_NOME];
    int encontradas = 0;

    printf("Nome (ou parte do nome) do jogo: ");
    scanf(" %49[^\n]", trecho);

    for (int i = 0; i < totalMaquinas; i++) {
        if (contemTrechoB(maquinas[i].nome, trecho)) {
            /* Imprime o cabecalho so antes da primeira correspondencia */
            if (encontradas == 0) {
                cabecalhoB();
            }
            mostrarMaquinaB(i);
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf("Nenhuma maquina encontrada com \"%s\".\n", trecho);
    } else {
        printf("%d maquina(s) encontrada(s).\n", encontradas);
    }
}

/*
 * Modulo B (opcao 5): modifica os dados de uma maquina (nome ou
 * recorde). Primeiro localiza pelo ID; so altera se ela existir.
 */
void modificarMaquina(void) {
    int id = lerInteiroB("ID da maquina a modificar: ");
    int pos = buscarIndicePorIdB(id);

    if (pos == -1) {
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
    int escolha = lerInteiroB("Escolha: ");

    switch (escolha) {
        case 1:
            printf("Novo nome do jogo: ");
            scanf(" %49[^\n]", maquinas[pos].nome);
            printf("Nome atualizado.\n");
            break;

        case 2: {
            int novoScore = lerInteiroB("Novo recorde (pontos): ");

            /* Validacao: pontuacao negativa nao faz sentido */
            if (novoScore < 0) {
                printf("Recorde invalido. Nada foi alterado.\n");
                break;
            }

            maquinas[pos].highScore = novoScore;
            printf("Nome do recordista: ");
            scanf(" %29[^\n]", maquinas[pos].recordista);
            printf("Recorde atualizado.\n");
            break;
        }

        case 0:
            printf("Modificacao cancelada.\n");
            break;

        default:
            printf("Opcao invalida. Nada foi alterado.\n");
    }
}

/*
 * Modulo B (opcao 6): altera o status da maquina (Ativa/Manutencao).
 * No struct do A, o status e o campo booleano 'ativa':
 * true = Ativa, false = Manutencao.
 */
void alterarStatusMaquina(void) {
    int id = lerInteiroB("ID da maquina: ");
    int pos = buscarIndicePorIdB(id);

    if (pos == -1) {
        printf("Maquina com ID %d nao encontrada.\n", id);
        return;
    }

    printf("Status atual da maquina %d (%s): %s\n",
           maquinas[pos].id, maquinas[pos].nome,
           maquinas[pos].ativa ? "Ativa" : "Manutencao");

    printf("1 - Colocar como Ativa\n");
    printf("2 - Colocar em Manutencao\n");
    printf("0 - Cancelar\n");
    int escolha = lerInteiroB("Escolha: ");

    if (escolha == 1) {
        maquinas[pos].ativa = true;
        printf("Maquina %d agora esta Ativa.\n", maquinas[pos].id);
    } else if (escolha == 2) {
        maquinas[pos].ativa = false;
        printf("Maquina %d agora esta em Manutencao.\n", maquinas[pos].id);
    } else if (escolha == 0) {
        printf("Alteracao cancelada.\n");
    } else {
        printf("Opcao invalida. Nada foi alterado.\n");
    }
}

/* ===================== FIM DO MODULO B ==================== */


/* ============================================================
 * INICIO DO MODULO C - Remocao e Matriz (Integrante C)
 * Escreva suas funcoes SOMENTE entre estas linhas.
 * ============================================================ */



/* ===================== FIM DO MODULO C ==================== */


/* ============================================================
 * INICIO DO MODULO D - Relatorios e Ordenacao (Eduardo)
 * Este modulo so LE os dados globais (nao altera o vetor), entao
 * nao depende das funcoes de B e C: usa apenas maquinas[] e
 * totalMaquinas.
 * ============================================================ */

/*
 * Modulo D (Eduardo): auxiliar que imprime uma maquina em uma linha.
 * Recebe a maquina como parametro (copia), sem alterar o vetor.
 */
void imprimirLinhaMaquina(Maquina m) {
    printf("[%2d] %-18s | Score: %6d | Recordista: ",
           m.id, m.nome, m.highScore);

    if (m.recordista[0] == '\0') {
        printf("---");
    } else {
        printf("%s", m.recordista);
    }

    if (m.ativa) {
        printf(" | Ativa\n");
    } else {
        printf(" | Manutencao\n");
    }
}

/*
 * Modulo D (Eduardo): relatorio geral com totais, extremos de
 * high score, media e situacao das maquinas.
 */
void relatorioGeral(void) {
    int totalFichas = 0;
    int somaScores = 0;
    int ativas = 0;
    int manutencao = 0;
    int posMaior, posMenor, posMaisJogada;
    float media;

    if (totalMaquinas == 0) {
        printf("Nenhuma maquina cadastrada para gerar relatorio.\n");
        return;
    }

    /* A primeira maquina real assume a lideranca (nunca comecar em 0) */
    posMaior = 0;
    posMenor = 0;
    posMaisJogada = 0;

    for (int i = 0; i < totalMaquinas; i++) {
        totalFichas += maquinas[i].fichasInseridas;
        somaScores += maquinas[i].highScore;

        if (maquinas[i].ativa) {
            ativas++;
        } else {
            manutencao++;
        }

        if (maquinas[i].highScore > maquinas[posMaior].highScore) {
            posMaior = i;
        }
        if (maquinas[i].highScore < maquinas[posMenor].highScore) {
            posMenor = i;
        }
        if (maquinas[i].fichasInseridas >
            maquinas[posMaisJogada].fichasInseridas) {
            posMaisJogada = i;
        }
    }

    /* O cast para float evita a divisao inteira */
    media = (float)somaScores / totalMaquinas;

    printf("\n========== RELATORIO GERAL ==========\n");
    printf("Total de maquinas .......: %d\n", totalMaquinas);
    printf("Ativas / Manutencao .....: %d / %d\n", ativas, manutencao);
    printf("Total de fichas inseridas: %d\n", totalFichas);
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
 * Modulo D (Eduardo): ranking de high scores (maior para menor)
 * com Bubble Sort. Ordena uma COPIA do vetor, entao as posicoes do
 * vetor original (usadas pelos modulos A, B e C) nao mudam.
 */
void rankingPorHighScore(void) {
    Maquina copia[MAX_MAQUINAS];
    Maquina aux;

    if (totalMaquinas == 0) {
        printf("Nenhuma maquina cadastrada para gerar ranking.\n");
        return;
    }

    /* 1. Copia o vetor original */
    for (int i = 0; i < totalMaquinas; i++) {
        copia[i] = maquinas[i];
    }

    /* 2. Bubble Sort decrescente por highScore */
    for (int i = 0; i < totalMaquinas - 1; i++) {
        for (int j = 0; j < totalMaquinas - 1 - i; j++) {
            if (copia[j].highScore < copia[j + 1].highScore) {
                aux = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = aux;
            }
        }
    }

    /* 3. Mostra o ranking */
    printf("\n========== RANKING DE HIGH SCORES ==========\n");
    for (int i = 0; i < totalMaquinas; i++) {
        printf("%2d lugar - ", i + 1);
        imprimirLinhaMaquina(copia[i]);
    }
    printf("============================================\n");
}

/* ===================== FIM DO MODULO D ==================== */


int main(void) {
    int opcao;

    do {
        printf("\n=== FLIPERAMA ===\n");
        printf("1 - Cadastrar maquina\n");
        printf("2 - Listar maquinas\n");
        /* Opcoes 3 a 6: modulo B (Integrante B adiciona aqui) */
        /* Opcoes 7 e 8: modulo C (Integrante C adiciona aqui) */
        printf("9 - Relatorio geral\n");
        printf("10 - Ranking por high score\n");
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
            /* case 3 a 6: modulo B (Integrante B adiciona aqui) */
            /* case 7 e 8: modulo C (Integrante C adiciona aqui) */
            case 9:
                relatorioGeral();
                break;
            case 10:
                rankingPorHighScore();
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