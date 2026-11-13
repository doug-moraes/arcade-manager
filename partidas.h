/*
 * Arquivo: partidas.h
 * Perspectiva do Integrante C: PARTIDAS e RELATORIOS.
 * Struct, vetor global, leitura/escrita em partidas.csv, CRUD, a regra de
 * negocio que cruza Jogadores e Maquinas, relatorios, ranking e matriz 2D.
 */
#ifndef PARTIDAS_H
#define PARTIDAS_H

#include <stdio.h>
#include <string.h>
#include "utilitarios.h"
#include "maquinas.h"
#include "jogadores.h"

/* O pre-processador troca MAX_PARTIDAS pelo numero antes de compilar */
#define MAX_PARTIDAS 50

// Autor: Eduardo Pan
struct Partida
{
    int idPartida;
    int idJogador;
    int idMaquina;
    int dia;    /* 0-Dom, 1-Seg, 2-Ter, 3-Qua, 4-Qui, 5-Sex, 6-Sab */
    int pontos;
    float valor; /* valor de uma ficha na hora da partida (em reais) */
    char nomeJogador[TAM_NOME_PESSOA]; /* guardado para o historico */
    char resultado; /* R-bateu o recorde da maquina, N-normal */
};

struct Partida historico[MAX_PARTIDAS];
int totalPartidas = 0;

/* Nomes dos dias da semana: 7 textos de ate 3 letras (+ o \0 do final) */
char nomesDia[7][4] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

/*
 * PERSISTENCIA: regrava o arquivo inteiro a partir do vetor. O modo "w"
 * zera o arquivo, entao qualquer partida nova, edicao ou estorno e
 * refletido.
 */
// Autor: Eduardo Pan
void salvarPartidasCSV()
{
    FILE *arq;
    int i;

    arq = fopen("partidas.csv", "w");
    if (arq == NULL)
    {
        printf("[ERRO] Nao foi possivel gravar partidas.csv!\n");
        return;
    }

    for (i = 0; i < totalPartidas; i++)
    {
        fprintf(arq, "%d;%d;%d;%d;%d;%.2f;%s;%c\n",
                historico[i].idPartida, historico[i].idJogador,
                historico[i].idMaquina, historico[i].dia,
                historico[i].pontos, historico[i].valor,
                historico[i].nomeJogador, historico[i].resultado);
    }
    fclose(arq);
}

/*
 * PERSISTENCIA: carrega partidas.csv para o vetor ao abrir o programa.
 * O fscanf devolve quantos campos leu (8). Numeros e o char levam &;
 * o texto nomeJogador nao, pois o nome do vetor ja e o endereco dele.
 */
// Autor: Eduardo Pan
void carregarPartidasCSV()
{
    FILE *arq;

    arq = fopen("partidas.csv", "r");
    if (arq == NULL)
    {
        return;
    }

    totalPartidas = 0;
    while (totalPartidas < MAX_PARTIDAS &&
           fscanf(arq, "%d;%d;%d;%d;%d;%f;%49[^;]; %c\n",
                  &historico[totalPartidas].idPartida,
                  &historico[totalPartidas].idJogador,
                  &historico[totalPartidas].idMaquina,
                  &historico[totalPartidas].dia,
                  &historico[totalPartidas].pontos,
                  &historico[totalPartidas].valor,
                  historico[totalPartidas].nomeJogador,
                  &historico[totalPartidas].resultado) == 8)
    {
        totalPartidas++;
    }
    fclose(arq);
}

/* Devolve a POSICAO do id da partida no vetor, ou -1 se nao existir */
// Autor: Eduardo Pan
int buscarIndicePartida(int idProcurado)
{
    int i;

    for (i = 0; i < totalPartidas; i++)
    {
        if (historico[i].idPartida == idProcurado)
        {
            return i;
        }
    }
    return -1;
}

/*
 * O id da partida e gerado sozinho: o maior id existente mais 1.
 * Assim nunca se repete, mesmo depois de estornos.
 */
// Autor: Eduardo Pan
int proximoIdPartida()
{
    int i;
    int maior = 0;

    for (i = 0; i < totalPartidas; i++)
    {
        if (historico[i].idPartida > maior)
        {
            maior = historico[i].idPartida;
        }
    }
    return maior + 1;
}

/* Cabecalho da tabela de partidas */
// Autor: Eduardo Pan
void imprimirCabecalhoPartidas()
{
    printf("\n%-4s %-20s %-20s %-4s %-8s %-9s %s\n",
           "ID", "Jogador", "Maquina", "Dia", "Pontos", "Valor", "Res");
}

/*
 * Imprime UMA partida (pela posicao no vetor). O nome da maquina e
 * buscado no modulo de Maquinas; se ela foi removida, mostra "(removida)".
 */
// Autor: Eduardo Pan
void imprimirPartida(int pos)
{
    int posMaq;

    posMaq = buscarIndiceMaquina(historico[pos].idMaquina);
    printf("%-4d %-20s %-20s %-4s %-8d R$ %-6.2f %c\n",
           historico[pos].idPartida, historico[pos].nomeJogador,
           posMaq != -1 ? maquinas[posMaq].nome : "(removida)",
           nomesDia[historico[pos].dia], historico[pos].pontos,
           historico[pos].valor, historico[pos].resultado);
}

/*
 * REGRA DE NEGOCIO (cruza os modulos Jogadores e Maquinas):
 *  - o jogador precisa existir e ter pelo menos 1 ficha;
 *  - a maquina precisa existir e estar ativa;
 *  - gasta 1 ficha do jogador, soma o valor na receita da maquina e,
 *    se a pontuacao superar o recorde, atualiza highScore e recordista.
 * Depois grava os tres arquivos CSV que mudaram.
 */
// Autor: Eduardo Pan
void registrarPartida()
{
    int idJog, idMaq, posJog, posMaq, dia, pontos, p;

    if (totalPartidas >= MAX_PARTIDAS)
    {
        printf("[ERRO] Historico lotado (%d partidas)!\n", MAX_PARTIDAS);
        return;
    }

    printf("\nID do jogador: ");
    idJog = lerInteiro();
    posJog = buscarIndiceJogador(idJog);
    if (posJog == -1)
    {
        printf("[ERRO] Jogador nao encontrado!\n");
        return;
    }
    if (jogadores[posJog].fichas <= 0)
    {
        printf("[ERRO] %s nao tem fichas. Compre fichas no modulo de Jogadores!\n",
               jogadores[posJog].nome);
        return;
    }

    printf("ID da maquina: ");
    idMaq = lerInteiro();
    posMaq = buscarIndiceMaquina(idMaq);
    if (posMaq == -1)
    {
        printf("[ERRO] Maquina nao encontrada!\n");
        return;
    }
    if (maquinas[posMaq].ativa == 0)
    {
        printf("[ERRO] A maquina %d esta em manutencao!\n", idMaq);
        return;
    }

    printf("Dia da semana (0-Dom 1-Seg 2-Ter 3-Qua 4-Qui 5-Sex 6-Sab): ");
    dia = lerInteiro();
    if (dia < 0 || dia > 6)
    {
        printf("[ERRO] Dia invalido! A ficha nao foi gasta.\n");
        return;
    }

    printf("Pontuacao obtida: ");
    pontos = lerInteiro();
    if (pontos < 0)
    {
        printf("[ERRO] Pontuacao invalida! A ficha nao foi gasta.\n");
        return;
    }

    /* Grava a partida na primeira posicao livre do historico */
    p = totalPartidas;
    historico[p].idPartida = proximoIdPartida();
    historico[p].idJogador = idJog;
    historico[p].idMaquina = idMaq;
    historico[p].dia = dia;
    historico[p].pontos = pontos;
    historico[p].valor = PRECO_FICHA;
    strcpy(historico[p].nomeJogador, jogadores[posJog].nome);
    historico[p].resultado = 'N';

    /* Cruzamento: baixa 1 ficha do jogador e soma a receita da maquina */
    jogadores[posJog].fichas--;
    maquinas[posMaq].receita += PRECO_FICHA;

    if (pontos > maquinas[posMaq].highScore)
    {
        maquinas[posMaq].highScore = pontos;
        strcpy(maquinas[posMaq].recordista, jogadores[posJog].nome);
        historico[p].resultado = 'R';
        printf("\n*** NOVO RECORDE na maquina %d: %d pontos! ***\n", idMaq, pontos);
    }
    else
    {
        printf("\nO recorde da maquina continua em %d pontos (%s).\n",
               maquinas[posMaq].highScore, maquinas[posMaq].recordista);
    }
    totalPartidas++;

    salvarPartidasCSV();
    salvarJogadoresCSV();
    salvarMaquinasCSV();
    printf("[OK] Partida registrada! %s ainda tem %d ficha(s).\n",
           jogadores[posJog].nome, jogadores[posJog].fichas);
}

/* LISTAGEM formatada de todo o historico */
// Autor: Eduardo Pan
void listarPartidas()
{
    int i;

    printf("\n--- HISTORICO DE PARTIDAS (%d registradas) ---", totalPartidas);
    if (totalPartidas == 0)
    {
        printf("\nNenhuma partida registrada.\n");
        return;
    }

    imprimirCabecalhoPartidas();
    for (i = 0; i < totalPartidas; i++)
    {
        imprimirPartida(i);
    }
    printf("Res: R-bateu o recorde da maquina | N-partida normal\n");
}

/*
 * BUSCA TEXTUAL: strstr procura o texto digitado dentro do nome do
 * jogador guardado em cada partida.
 */
// Autor: Eduardo Pan
void buscarPartidaPorJogador()
{
    char termo[30];
    int i;
    int encontradas = 0;

    printf("\nDigite parte do nome do jogador: ");
    scanf(" %29[^\n]", termo);

    for (i = 0; i < totalPartidas; i++)
    {
        if (strstr(historico[i].nomeJogador, termo) != NULL)
        {
            if (encontradas == 0)
            {
                imprimirCabecalhoPartidas();
            }
            imprimirPartida(i);
            encontradas++;
        }
    }

    if (encontradas == 0)
    {
        printf("Nenhuma partida encontrada para \"%s\".\n", termo);
    }
    else
    {
        printf("%d partida(s) encontrada(s).\n", encontradas);
    }
}

/*
 * EDICAO: corrige o dia da semana de uma partida e regrava o CSV. So o
 * dia pode ser editado: pontos e valor mexeriam no recorde e na receita.
 */
// Autor: Eduardo Pan
void editarPartida()
{
    int id, pos, dia;

    printf("\nID da partida a EDITAR: ");
    id = lerInteiro();
    pos = buscarIndicePartida(id);
    if (pos == -1)
    {
        printf("[ERRO] Partida nao encontrada!\n");
        return;
    }

    imprimirCabecalhoPartidas();
    imprimirPartida(pos);

    printf("\nNovo dia (0-Dom 1-Seg 2-Ter 3-Qua 4-Qui 5-Sex 6-Sab): ");
    dia = lerInteiro();
    if (dia < 0 || dia > 6)
    {
        printf("[ERRO] Dia invalido! Nada foi alterado.\n");
        return;
    }

    historico[pos].dia = dia;
    salvarPartidasCSV();
    printf("[OK] Dia atualizado na memoria e no arquivo CSV!\n");
}

/*
 * REMOCAO (estorno): devolve a ficha ao jogador e tira o valor da receita
 * da maquina (se eles ainda existirem), puxa as partidas seguintes para
 * cobrir o buraco e regrava os CSV. O recorde da maquina nao e refeito.
 */
// Autor: Eduardo Pan
void removerPartida()
{
    int id, pos, j, posJog, posMaq, confirma;

    printf("\nID da partida a ESTORNAR: ");
    id = lerInteiro();
    pos = buscarIndicePartida(id);
    if (pos == -1)
    {
        printf("[ERRO] Partida nao encontrada!\n");
        return;
    }

    imprimirCabecalhoPartidas();
    imprimirPartida(pos);
    printf("\nConfirma o estorno? (1 - Sim, 0 - Nao): ");
    confirma = lerInteiro();
    if (confirma != 1)
    {
        printf("Estorno cancelado.\n");
        return;
    }

    posJog = buscarIndiceJogador(historico[pos].idJogador);
    if (posJog != -1)
    {
        jogadores[posJog].fichas++;
    }
    posMaq = buscarIndiceMaquina(historico[pos].idMaquina);
    if (posMaq != -1)
    {
        maquinas[posMaq].receita -= historico[pos].valor;
        if (maquinas[posMaq].receita < 0)
        {
            maquinas[posMaq].receita = 0;
        }
    }

    for (j = pos; j < totalPartidas - 1; j++)
    {
        historico[j] = historico[j + 1];
    }
    totalPartidas--;

    salvarPartidasCSV();
    salvarJogadoresCSV();
    salvarMaquinasCSV();
    printf("[OK] Partida estornada! Ficha devolvida e CSV atualizado.\n");
}

/*
 * RELATORIO ANALITICO: calcula totais, media, extremos e os campeoes de
 * uso, percorrendo os vetores de partidas, maquinas e jogadores.
 */
// Autor: Eduardo Pan
void relatorioGeral()
{
    int i, j, contador;
    int partidasRecorde = 0;
    int melhorPontos = 0;
    int posMelhor = 0;
    int maisJogadas = 0;
    int posMaqMais = -1;
    int maisPartidas = 0;
    int posJogMais = -1;
    int posJogGasto = -1;
    int posMaqMelhor;
    float receitaTotal = 0;
    float somaPontos = 0;
    float media;
    float maiorGasto = 0;

    if (totalPartidas == 0)
    {
        printf("Nenhuma partida registrada para gerar relatorio.\n");
        return;
    }

    for (i = 0; i < totalPartidas; i++)
    {
        receitaTotal += historico[i].valor;
        somaPontos += historico[i].pontos;
        if (historico[i].resultado == 'R')
        {
            partidasRecorde++;
        }
        if (historico[i].pontos > melhorPontos)
        {
            melhorPontos = historico[i].pontos;
            posMelhor = i;
        }
    }
    /* somaPontos e float, entao a divisao nao e inteira */
    media = somaPontos / totalPartidas;

    /* Maquina mais jogada: conta as partidas de cada maquina */
    for (i = 0; i < totalMaquinas; i++)
    {
        contador = 0;
        for (j = 0; j < totalPartidas; j++)
        {
            if (historico[j].idMaquina == maquinas[i].idMaquina)
            {
                contador++;
            }
        }
        if (contador > maisJogadas)
        {
            maisJogadas = contador;
            posMaqMais = i;
        }
    }

    /* Jogador mais ativo: conta as partidas de cada jogador */
    for (i = 0; i < totalJogadores; i++)
    {
        contador = 0;
        for (j = 0; j < totalPartidas; j++)
        {
            if (historico[j].idJogador == jogadores[i].idJogador)
            {
                contador++;
            }
        }
        if (contador > maisPartidas)
        {
            maisPartidas = contador;
            posJogMais = i;
        }
        if (jogadores[i].totalGasto > maiorGasto)
        {
            maiorGasto = jogadores[i].totalGasto;
            posJogGasto = i;
        }
    }

    printf("\n========== RELATORIO GERAL ==========\n");
    printf("Partidas registradas ....: %d\n", totalPartidas);
    printf("Partidas com recorde ....: %d\n", partidasRecorde);
    printf("Receita das partidas ....: R$ %.2f\n", receitaTotal);
    printf("Media de pontos .........: %.2f\n", media);

    posMaqMelhor = buscarIndiceMaquina(historico[posMelhor].idMaquina);
    printf("Maior pontuacao .........: %d (%s na maquina %s)\n",
           melhorPontos, historico[posMelhor].nomeJogador,
           posMaqMelhor != -1 ? maquinas[posMaqMelhor].nome : "(removida)");

    if (posMaqMais != -1)
    {
        printf("Maquina mais jogada .....: %s (ID %d, %d partidas)\n",
               maquinas[posMaqMais].nome, maquinas[posMaqMais].idMaquina,
               maisJogadas);
    }
    if (posJogMais != -1)
    {
        printf("Jogador mais ativo ......: %s (%d partidas)\n",
               jogadores[posJogMais].nome, maisPartidas);
    }
    if (posJogGasto != -1)
    {
        printf("Jogador que mais gastou .: %s (R$ %.2f)\n",
               jogadores[posJogGasto].nome, maiorGasto);
    }
    printf("=====================================\n");
}

/*
 * RANKING com BUBBLE SORT em structs: ordena uma COPIA do vetor de
 * maquinas por highScore (do maior para o menor). Como a copia e que
 * muda de ordem, as posicoes do vetor original nao sao alteradas.
 */
// Autor: Eduardo Pan
void rankingMaquinas()
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

    /* 2. Bubble Sort decrescente: troca vizinhos fora de ordem. A cada
       volta do laco externo, o menor valor "afunda" para o fim */
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
    printf("\n========== RANKING DE MAQUINAS POR RECORDE ==========\n");
    for (i = 0; i < totalMaquinas; i++)
    {
        printf("%2d lugar - [%2d] %-22s | Recorde: %7d | %s\n",
               i + 1, copia[i].idMaquina, copia[i].nome, copia[i].highScore,
               copia[i].recordista);
    }
    printf("=====================================================\n");
}

/*
 * MATRIZ 2D: linhas = maquinas, colunas = dias da semana. Cada celula
 * conta as partidas daquela maquina naquele dia. A matriz e montada a
 * partir do historico, entao sempre bate com os dados reais.
 */
// Autor: Eduardo Pan
void matrizUsoSemanal()
{
    int matriz[MAX_MAQUINAS][7];
    int totalDia[7];
    int i, d, pos, totalMaquina, totalGeral;

    if (totalMaquinas == 0)
    {
        printf("Nenhuma maquina cadastrada para mostrar a matriz.\n");
        return;
    }

    /* Zera a matriz e os totais por dia */
    for (i = 0; i < totalMaquinas; i++)
    {
        for (d = 0; d < 7; d++)
        {
            matriz[i][d] = 0;
        }
    }
    for (d = 0; d < 7; d++)
    {
        totalDia[d] = 0;
    }

    /* Cada partida soma 1 na linha da maquina e na coluna do dia */
    for (i = 0; i < totalPartidas; i++)
    {
        pos = buscarIndiceMaquina(historico[i].idMaquina);
        if (pos != -1)
        {
            matriz[pos][historico[i].dia]++;
        }
    }

    printf("\n========== USO SEMANAL (PARTIDAS POR DIA) ==========\n");
    printf("%-4s %-22s", "ID", "Jogo");
    for (d = 0; d < 7; d++)
    {
        printf("%4s", nomesDia[d]);
    }
    printf("%7s\n", "Total");

    totalGeral = 0;
    for (i = 0; i < totalMaquinas; i++)
    {
        totalMaquina = 0;
        printf("%-4d %-22s", maquinas[i].idMaquina, maquinas[i].nome);
        for (d = 0; d < 7; d++)
        {
            printf("%4d", matriz[i][d]);
            totalMaquina += matriz[i][d];
            totalDia[d] += matriz[i][d];
        }
        printf("%7d\n", totalMaquina);
        totalGeral += totalMaquina;
    }

    printf("%-27s", "Total do dia");
    for (d = 0; d < 7; d++)
    {
        printf("%4d", totalDia[d]);
    }
    printf("%7d\n", totalGeral);
    printf("====================================================\n");
}

/*
 * RELATORIO por jogador: lista as partidas de UM jogador (cruzando com o
 * modulo de Jogadores) e calcula quantas foram, quanto gastou e o melhor
 * resultado.
 */
// Autor: Eduardo Pan
void partidasDeUmJogador()
{
    int id, posJog, i;
    int quantidade = 0;
    int melhor = 0;
    float gasto = 0;

    printf("\nID do jogador: ");
    id = lerInteiro();
    posJog = buscarIndiceJogador(id);
    if (posJog == -1)
    {
        printf("[ERRO] Jogador nao encontrado!\n");
        return;
    }

    printf("\n--- Partidas de %s ---", jogadores[posJog].nome);
    for (i = 0; i < totalPartidas; i++)
    {
        if (historico[i].idJogador == id)
        {
            if (quantidade == 0)
            {
                imprimirCabecalhoPartidas();
            }
            imprimirPartida(i);
            quantidade++;
            gasto += historico[i].valor;
            if (historico[i].pontos > melhor)
            {
                melhor = historico[i].pontos;
            }
        }
    }

    if (quantidade == 0)
    {
        printf("\n%s ainda nao jogou nenhuma partida.\n", jogadores[posJog].nome);
        return;
    }
    printf("\nPartidas: %d | Gasto em partidas: R$ %.2f | Melhor pontuacao: %d\n",
           quantidade, gasto, melhor);
    printf("Fichas restantes: %d | Total gasto em fichas: R$ %.2f\n",
           jogadores[posJog].fichas, jogadores[posJog].totalGasto);
}

/*
 * SUBMENU de Partidas e Relatorios. Cada case chama uma funcao propria.
 * A tela e limpa a cada volta e o 0 devolve o controle ao menu principal.
 */
// Autor: Eduardo Pan
void moduloPartidas()
{
    int opcao;

    do
    {
        limparTela();
        printf("--- [MODULO 3: PARTIDAS E RELATORIOS (Eduardo)] ---\n");
        printf("1. Registrar nova partida\n");
        printf("2. Listar historico de partidas\n");
        printf("3. Buscar partidas por jogador\n");
        printf("4. Editar dia de uma partida\n");
        printf("5. Estornar (remover) partida\n");
        printf("6. [RELATORIO] Relatorio geral\n");
        printf("7. [RELATORIO] Ranking de maquinas (Bubble Sort)\n");
        printf("8. [RELATORIO] Matriz de uso semanal\n");
        printf("9. [RELATORIO] Partidas de um jogador\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha uma acao: ");
        opcao = lerInteiro();

        switch (opcao)
        {
        case 1:
            registrarPartida();
            pausar();
            break;
        case 2:
            listarPartidas();
            pausar();
            break;
        case 3:
            buscarPartidaPorJogador();
            pausar();
            break;
        case 4:
            editarPartida();
            pausar();
            break;
        case 5:
            removerPartida();
            pausar();
            break;
        case 6:
            relatorioGeral();
            pausar();
            break;
        case 7:
            rankingMaquinas();
            pausar();
            break;
        case 8:
            matrizUsoSemanal();
            pausar();
            break;
        case 9:
            partidasDeUmJogador();
            pausar();
            break;
        case 0:
            break;
        default:
            printf("\n[ERRO] Opcao invalida no Modulo de Partidas!\n");
            pausar();
        }
    } while (opcao != 0);
}

#endif
