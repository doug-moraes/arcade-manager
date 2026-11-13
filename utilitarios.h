/*
 * Arquivo: utilitarios.h
 * Funcoes de apoio usadas por todos os modulos: limpar tela, pausa,
 * leitura segura de numeros e protecao do separador do CSV.
 */
#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <stdio.h>
#include <stdlib.h>

/* Preco de uma ficha, em reais. O #define troca o nome pelo numero antes
   da compilacao: nao ocupa memoria e, se o preco mudar, muda so aqui. */
#define PRECO_FICHA 2.50

/* Tamanho dos textos de nomes de pessoas (jogadores e recordistas) */
#define TAM_NOME_PESSOA 50

/*
 * Limpa o terminal. _WIN32 so existe quando o programa e compilado no
 * Windows; nos outros sistemas o comando de limpar tela e o "clear".
 */
// Autor: Douglas Luan
void limparTela()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
 * Pausa para o usuario ler o resultado antes de a tela ser limpa.
 * Le uma palavra qualquer e descarta (o scanf espera o ENTER).
 */
// Autor: Douglas Luan
void pausar()
{
    char lixo[50];

    printf("\nDigite qualquer texto e pressione ENTER para continuar... ");
    scanf("%49s", lixo);
}

/*
 * Le um numero inteiro do teclado. O scanf devolve quantos valores leu:
 *  - 1 quando leu o numero;
 *  - 0 quando digitaram letras. O texto errado e lido como string e
 *    descartado, e a leitura se repete (sem isso, o texto ficaria no
 *    teclado e o programa entraria em loop infinito);
 *  - um valor negativo quando a entrada acabou (Ctrl+Z no Windows).
 *    Nesse caso nao ha mais o que ler, entao o programa se encerra.
 */
// Autor: Douglas Luan
int lerInteiro()
{
    int valor, lidos;
    char lixo[50];

    lidos = scanf("%d", &valor);
    while (lidos != 1)
    {
        if (lidos < 0)
        {
            printf("\nEntrada encerrada. Fechando o programa...\n");
            exit(0);
        }
        scanf("%49s", lixo);
        printf("Entrada invalida. Digite um numero: ");
        lidos = scanf("%d", &valor);
    }
    return valor;
}

/*
 * O CSV separa os campos com ponto e virgula. Se o usuario digitar um ';'
 * dentro de um nome, a leitura do arquivo ficaria desalinhada. Esta
 * funcao troca cada ';' do texto por ','.
 */
// Autor: Douglas Luan
void limparSeparador(char *texto)
{
    int i;

    for (i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] == ';')
        {
            texto[i] = ',';
        }
    }
}

#endif
