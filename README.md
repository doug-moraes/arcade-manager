# Arcade Manager - Gestão de Fliperama Anos 90

Projeto Integrador (P2) da disciplina **Algoritmos e Pensamento Computacional** (Prof. Rafael Duarte).

Sistema em **C**, executado no terminal, que administra um fliperama dos anos 90: máquinas de arcade, jogadores com saldo de fichas e o histórico de partidas com recordes (*high scores*). Os dados ficam em vetores de structs na memória e são salvos e carregados de arquivos **.csv** (separador `;`).

## Equipe e perspectivas

Cada integrante é dono de uma perspectiva completa (struct, vetor, arquivo CSV e CRUD):

| Integrante | Perspectiva | Módulo | Arquivo de dados |
|---|---|---|---|
| Douglas Luan | Máquinas | `maquinas.h` | `maquinas.csv` |
| Leonardo Messias | Jogadores e caixa de fichas | `jogadores.h` | `jogadores.csv` |
| Eduardo Pan | Partidas e relatórios | `partidas.h` | `partidas.csv` |

O `utilitarios.h` reúne as funções de apoio comuns (limpar tela, pausa e leitura segura de números). O `main.c` importa os módulos, carrega os CSV ao abrir e chama os submenus.

## Structs

| Struct | Campos |
|---|---|
| `Maquina` | `int idMaquina`, `char nome[40]`, `char categoria`, `int ativa`, `int highScore`, `char recordista[50]`, `float receita` |
| `Jogador` | `int idJogador`, `char nome[50]`, `char telefone[20]`, `char nivel`, `int fichas`, `float totalGasto` |
| `Partida` | `int idPartida`, `int idJogador`, `int idMaquina`, `int dia`, `int pontos`, `float valor`, `char nomeJogador[50]`, `char resultado` |

Cada struct tem seu vetor global de 50 posições (`maquinas`, `jogadores` e `historico`) e um contador de registros.

## Funcionalidades

| Módulo | Operações |
|---|---|
| Máquinas | Cadastrar (bloqueia ID repetido), listar, buscar por nome (`strstr`), editar e remover, tudo refletido no CSV |
| Jogadores | Cadastrar, listar, buscar por nome, editar, **comprar fichas** no caixa (atualiza o nível) e remover |
| Partidas | Registrar partida, listar histórico, buscar por jogador, editar o dia, estornar |
| Relatórios | Relatório geral, ranking de máquinas por recorde (Bubble Sort em structs), matriz de uso semanal (2D) e partidas de um jogador |

### Regras de negócio (cruzamento entre módulos)

Ao registrar uma partida, o sistema:

1. valida que o **jogador existe** e tem pelo menos 1 ficha;
2. valida que a **máquina existe** e está ativa (máquina em manutenção recusa a partida);
3. **baixa 1 ficha** do jogador e soma o valor da ficha na **receita da máquina**;
4. se a pontuação superar o recorde, atualiza `highScore` e `recordista` da máquina;
5. regrava os três arquivos CSV que mudaram.

Estornar uma partida faz o caminho inverso: devolve a ficha ao jogador e retira o valor da receita da máquina.

O nível do jogador depende do total gasto em fichas: Bronze (até R$ 24,99), Prata (R$ 25,00 a R$ 59,99) e Ouro (R$ 60,00 ou mais).

## Arquivos CSV

Cada linha é um registro e os campos são separados por `;`. O programa carrega os três arquivos ao abrir e regrava o arquivo inteiro a cada cadastro, edição ou remoção.

| Arquivo | Campos |
|---|---|
| `maquinas.csv` | `id;nome;categoria;ativa;highScore;recordista;receita` |
| `jogadores.csv` | `id;nome;telefone;nivel;fichas;totalGasto` |
| `partidas.csv` | `id;idJogador;idMaquina;dia;pontos;valor;nomeJogador;resultado` |

Os arquivos já vêm com 10 máquinas, 10 jogadores e 16 partidas. Os números são coerentes entre si (por exemplo, a receita de cada máquina é a soma das suas partidas).
