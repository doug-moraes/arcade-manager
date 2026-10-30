# 🕹️ Arcade Manager

Sistema de gestão de um **fliperama anos 90**, desenvolvido em **C** e executado no terminal.

Projeto Integrador (P2) da disciplina **Algoritmos e Pensamento Computacional**, com o tema *Fliperama / Arcade Anos 90 (Fichas e High Scores)*.

---

## 📖 Sobre o projeto

O programa simula a administração de um fliperama: o cliente compra fichas no caixa, escolhe uma máquina em funcionamento e joga, tentando bater o recorde (*high score*) daquela máquina. O sistema permite cadastrar, buscar, modificar e remover máquinas, além de gerar relatórios e rankings.

Todos os dados ficam na **memória RAM** (`struct` + vetor), sem uso de arquivos, e o programa funciona por um **menu interativo em loop contínuo** (`do-while` + `switch-case`), com um submenu para cada módulo.

## ✨ Funcionalidades

| Módulo | Funcionalidade | Status |
|---|---|---|
| A | Cadastro de máquinas, com validação do limite do vetor | ✅ Pronto |
| A | Listagem das máquinas | ✅ Pronto |
| A | Compra de fichas no caixa, jogar e consulta do caixa | ✅ Pronto |
| B | Busca de máquinas por ID e por nome (ou parte do nome) | ✅ Pronto |
| B | Modificação de nome, recorde e status das máquinas | ✅ Pronto |
| C | Remoção de máquinas | ✅ Pronto |
| C | Matriz de uso semanal (máquinas × dias da semana) | ✅ Pronto |
| D | Relatório geral e cálculos analíticos | ✅ Pronto |
| D | Ranking por high score (Bubble Sort) | ✅ Pronto |

## 🧭 Menus

O menu principal leva a um submenu por módulo. Em todos eles, a opção `0` volta ao menu anterior.

| Menu | Opções |
|---|---|
| **Principal** | 1 - Módulo A · 2 - Módulo B · 3 - Módulo C · 4 - Módulo D · 0 - Sair |
| **Módulo A** | 1 - Cadastrar máquina · 2 - Listar máquinas · 3 - Comprar fichas · 4 - Jogar · 5 - Consultar caixa |
| **Módulo B** | 1 - Buscar por ID · 2 - Buscar por nome · 3 - Modificar nome ou recorde · 4 - Alterar status |
| **Módulo C** | 1 - Remover máquina · 2 - Matriz de uso semanal |
| **Módulo D** | 1 - Relatório geral · 2 - Ranking por high score |

As leituras de número usam uma função que repete a pergunta quando a entrada é inválida, então digitar uma letra no menu não trava o programa.

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
| `ativa` | `int` | `1` em funcionamento, `0` em manutenção |
| `receita` | `float` | Total em reais arrecadado pela máquina |

**`Caixa`** representa o balcão onde as fichas são vendidas:

| Campo | Tipo | Descrição |
|---|---|---|
| `precoFicha` | `float` | Preço de cada ficha (R$ 2,50) |
| `totalArrecadado` | `float` | Total em reais vendido no caixa |
| `fichasVendidas` | `int` | Total de fichas vendidas |
| `fichasEmMaos` | `int` | Fichas compradas que ainda não foram usadas |

As máquinas ficam em um vetor global `maquinas[MAX_MAQUINAS]`, com capacidade para **50** máquinas. A variável `totalMaquinas` indica quantas posições estão em uso, e `proximoId` gera ids que nunca se repetem, mesmo depois de remoções.

A **matriz de uso semanal** `usoSemanal[MAX_MAQUINAS][DIAS_SEMANA]` guarda quantas fichas cada máquina recebeu em cada dia (Dom a Sáb). A linha `i` da matriz pertence à máquina que está na posição `i` do vetor, e as duas andam juntas na remoção.

## 🎮 Como o sistema funciona

1. No **Módulo A**, a opção **Comprar fichas** pergunta a quantidade (de 1 a 100), calcula o valor e soma as fichas ao que o cliente tem em mãos.
2. Ao **jogar** sem fichas, o sistema avisa que não há fichas e volta ao submenu.
3. Ao **jogar** com fichas, a pessoa informa o ID de uma máquina em funcionamento, o dia da semana e a pontuação obtida. Uma ficha é gasta, e a máquina, a matriz de uso e a receita são atualizadas.
4. Se a pontuação superar o recorde da máquina, o sistema pede o nome do novo recordista.
5. O **Módulo D** consolida tudo em um relatório geral (totais, média, extremos de high score, receita) e em um ranking ordenado por Bubble Sort, feito sobre uma cópia do vetor.

Os dados de exemplo já vêm carregados: **10 máquinas** de 3 jogos (Street Fighter II, Pac-Man e Pinball), sendo 6 em funcionamento e 4 em manutenção.

## ▶️ Como compilar e executar

```
gcc arcade.c -o arcade
./arcade
```

No Windows, o executável gerado é `arcade.exe`. O projeto também compila no Dev-C++.

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
├── arcade.c      código-fonte do sistema
├── README.md     este arquivo

```