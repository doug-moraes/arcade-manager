/*
 * Projeto Integrador (P2) - Arcade Manager: Gestao de Fliperama Anos 90
 * Disciplina: Algoritmos e Pensamento Computacional
 * Professor: Rafael Duarte
 *
 * Divisao por perspectiva (um modulo .h por integrante):
 *   Integrante A: Douglas Luan     - maquinas.h   (maquinas.csv)
 *   Integrante B: Leonardo Messias - jogadores.h  (jogadores.csv)
 *   Integrante C: Eduardo Pan      - partidas.h   (partidas.csv)
 *   Apoio comum a todos            - utilitarios.h
 *
 * O main.c so importa os modulos, carrega os dados dos arquivos CSV e
 * chama os submenus.
 */

#include <stdio.h>
#include <locale.h>
#include "utilitarios.h"
#include "maquinas.h"
#include "jogadores.h"
#include "partidas.h"

/* Cabecalho do menu principal, com a quantidade de dados carregados */
// Autor: Douglas Luan
void exibirCabecalho()
{
    printf("======================================================\n");
    printf("    ARCADE MANAGER - FLIPERAMA ANOS 90 (GESTAO)\n");
    printf("======================================================\n");
    printf("Dados carregados: %d maquinas | %d jogadores | %d partidas\n\n",
           totalMaquinas, totalJogadores, totalPartidas);
}

// Autor: Douglas Luan
int main()
{
    int opcao;

    /* LC_CTYPE traduz os caracteres acentuados da tabela do Portugues.
       Nao usamos LC_ALL porque ele trocaria o ponto decimal por virgula
       e quebraria a leitura dos numeros decimais dos arquivos CSV. */
    setlocale(LC_CTYPE, "Portuguese");

    /* Carrega os dados dos arquivos CSV para os vetores na memoria */
    carregarMaquinasCSV();
    carregarJogadoresCSV();
    carregarPartidasCSV();

    do
    {
        limparTela();
        exibirCabecalho();
        printf("SELECIONE A PERSPECTIVA DO SISTEMA:\n");
        printf("1. Modulo de Maquinas (Douglas)\n");
        printf("2. Modulo de Jogadores (Leonardo)\n");
        printf("3. Modulo de Partidas e Relatorios (Eduardo)\n");
        printf("0. Encerrar o Sistema\n");
        printf("------------------------------------------------------\n");
        printf("Digite a opcao desejada: ");
        opcao = lerInteiro();

        switch (opcao)
        {
        case 1:
            moduloMaquinas();
            break;
        case 2:
            moduloJogadores();
            break;
        case 3:
            moduloPartidas();
            break;
        case 0:
            printf("\nEncerrando o sistema... Ate logo!\n");
            break;
        default:
            printf("\n[ALERTA] Opcao invalida! Digite um numero de 0 a 3.\n");
            pausar();
        }
    } while (opcao != 0);

    return 0;
}
