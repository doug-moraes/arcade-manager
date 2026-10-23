# 🕹️ Arcade Manager

Sistema de gestão de um **fliperama anos 90**, desenvolvido em **C** e executado no terminal.

Projeto Integrador (P2) da disciplina **Algoritmos e Pensamento Computacional**, com o tema *Fliperama / Arcade Anos 90 (Fichas e High Scores)*.

---

## 📖 Sobre o projeto

O programa simula a administração de um fliperama: o cliente compra fichas no caixa, escolhe uma máquina em funcionamento e joga, tentando bater o recorde (*high score*) daquela máquina. O sistema permite cadastrar, buscar, modificar e remover máquinas, além de gerar relatórios e rankings.

Todos os dados ficam na **memória RAM** (`struct` + vetor), sem uso de arquivos, e o programa funciona por um **menu interativo em loop contínuo** (`do-while` + `switch-case`).

## ✨ Funcionalidades

| Módulo | Funcionalidade | Status |
|---|---|---|
| A | Cadastro de máquinas, com validação do limite do vetor | ✅ Pronto |
| A | Listagem das máquinas (versão provisória para testes) | 🔧 Provisória |
| B | Busca e modificação de dados e status das máquinas | ⏳ Em desenvolvimento |
| B | Compra de fichas no caixa e opção de jogar | ⏳ Em desenvolvimento |
| C | Remoção de máquinas | ⏳ Em desenvolvimento |
| C | Matriz de controle do tema | ⏳ Em desenvolvimento |
| D | Relatórios e cálculos analíticos | ⏳ Em desenvolvimento |
| D | Ordenação do ranking por high score (Bubble Sort) | ⏳ Em desenvolvimento |

## 🧱 Estrutura de dados

O sistema usa dois `struct`.

**`Maquina`** representa uma máquina do fliperama:

| Campo | Tipo | Descrição |
|---|---|---|
| `id` | `int` | Identificador único da máquina |
| `nome` | `char[50]` | Nome do jogo |
| `fichasInseridas` | `int` | Total de partidas já jogadas na máquina |
| `highScore` | `int` | Recorde de pontos |
| `recordista` | `char[30]` | Nome de quem fez o recorde |
| `ativa` | `bool` | `true` em funcionamento, `false` em manutenção |

**`Caixa`** representa o balcão onde as fichas são compradas:

| Campo | Tipo | Descrição |
|---|---|---|
| `precoFicha` | `int` | Preço de cada ficha (R$ 1,00) |
| `saldo` | `int` | Dinheiro que sobrou de compras anteriores |
| `fichasEmMaos` | `int` | Fichas compradas que ainda não foram usadas |

As máquinas ficam em um vetor global `maquinas[MAX_MAQUINAS]`. A variável `totalMaquinas` indica quantas posições estão em uso, e `proximoId` gera ids que nunca se repetem, mesmo depois de remoções.

## 🎮 Como o sistema funciona

1. O menu inicial oferece, entre outras opções, **jogar** e **comprar fichas**.
2. Ao **comprar fichas**, o sistema mostra o valor da ficha, pergunta a quantidade e pede o dinheiro. O que passar do valor da compra fica como saldo.
3. Ao **jogar** sem fichas, o sistema avisa que não há fichas e volta ao menu.
4. Ao **jogar** com fichas, a pessoa escolhe uma máquina em funcionamento, gasta uma ficha e, se superar o recorde, entra para o ranking.

Os dados de exemplo já vêm carregados: **10 máquinas** de 3 jogos (Street Fighter II, Pac-Man e Pinball), sendo 6 em funcionamento e 4 em manutenção.

## 👥 Equipe

Cada integrante é responsável por um módulo do sistema.

| Integrante | Módulo | Responsável |
|---|---|---|
| A | Inserção e Cadastro (validação de limites na RAM) | Douglas Luan |
| B | Busca e Modificação (atualização de status e dados) | Leonardo Messias |
| C | Remoção e Matriz (grade de controle do tema) | Douglas Luan |
| D | Relatórios e Ordenação (cálculos analíticos e Bubble Sort) | Eduardo Pan |


## 📁 Arquivos do repositório

```
arcade-manager/
├── arcade.c        código-fonte do sistema
├── README.md     este arquivo
└── .gitignore    arquivos ignorados pelo Git (executáveis)
```
