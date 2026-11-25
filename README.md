# MiniComp — Compilador para a Linguagem MiniLang

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![GCC / G++](https://img.shields.io/badge/GCC%20%2F%20G%2B%2B-A42E2B?style=for-the-badge&logo=gnu&logoColor=white)
![Make](https://img.shields.io/badge/Make-427819?style=for-the-badge&logo=gnu&logoColor=white)
![JFLAP](https://img.shields.io/badge/JFLAP-2B5B84?style=for-the-badge)
![Git](https://img.shields.io/badge/Git-F05032?style=for-the-badge&logo=git&logoColor=white)

## 📌 Visão Geral do Projeto

Este repositório contém o desenvolvimento do **MiniComp**, um compilador para a linguagem de programação hipotética **MiniLang**, implementado em **C++**. O objetivo principal é desenvolver um compilador funcional capaz de realizar o processamento e a interpretação de programas escritos em MiniLang, aplicando conceitos fundamentais de linguagens formais, teoria da computação e construção de compiladores.

O projeto abrange as etapas de **análise léxica**, **análise sintática preditiva LL(1)**, **construção de Árvore Sintática Abstrata (AST)**, **gerenciamento de tabela de símbolos** e **interpretação semântica**, incluindo o tratamento de erros e a execução de comandos e expressões da linguagem.

## Arquitetura do Projeto

O compilador foi estruturado em componentes integrados, responsáveis pelas diferentes etapas de processamento do código-fonte. O fluxo inicia com a identificação dos tokens, passa pela validação sintática e construção da AST, e finaliza com a interpretação semântica do programa:

```text
                [ Código-Fonte MiniLang ]
                            |
                            v
                  [ Analisador Léxico ]
                            |
                            v
                       [ Tokens ]
                            |
                            v
                [ Analisador Sintático ]
                         [ LL(1) ]
                            |
                            v
                [ Árvore Sintática Abstrata ]
                          [ AST ]
                            |
                            v
                   [ Tabela de Símbolos ]
                            |
                            v
                     [ Interpretador ]
                            |
                            v
                  [ Execução do Programa ]
                            |
                            v
                    [ Saída / Resultados ]
```

### Componentes do Compilador

* **Analisador Léxico (Lexer):** responsável por reconhecer identificadores, números inteiros, palavras reservadas, operadores e delimitadores, descartando espaços em branco e comentários e identificando erros léxicos com indicação de posição.
* **Analisador Sintático (Parser LL(1)):** responsável por verificar a estrutura gramatical do código-fonte por meio de análise sintática preditiva, utilizando os conjuntos FIRST e FOLLOW e a tabela de parsing LL(1).
* **Árvore Sintática Abstrata (AST):** representa a estrutura hierárquica dos comandos e expressões reconhecidos pelo parser, servindo como base para a interpretação do programa.
* **Tabela de Símbolos:** gerencia as variáveis globais da linguagem, armazenando identificadores e seus respectivos valores durante a execução.
* **Interpretador:** percorre a AST e executa comandos de atribuição, leitura, escrita, estruturas condicionais `if-then-else`, laços `while-do` e expressões aritméticas e relacionais.

## Funcionalidades

* Reconhecimento léxico de tokens.
* Identificação e tratamento de erros léxicos.
* Análise sintática preditiva LL(1).
* Construção de Árvore Sintática Abstrata (AST).
* Gerenciamento de tabela de símbolos global.
* Execução de expressões aritméticas e relacionais.
* Processamento de comandos de atribuição, leitura e escrita.
* Execução de estruturas condicionais e laços de repetição.
* Tratamento de divisão por zero.
* Rastreamento da execução do programa.
* Validação por meio de casos de teste válidos e inválidos.

## Tecnologias e Ferramentas

| Tecnologia | Aplicação |
| :--- | :--- |
| **C++** | Implementação do compilador e interpretador |
| **GCC / G++** | Compilação do código-fonte |
| **Make** | Automação da compilação |
| **JFLAP** | Modelagem de autômatos e gramáticas |
| **Git / GitHub** | Versionamento e gerenciamento do código |

## 📂 Estrutura do Projeto

```text
MiniComp/
│
├── src/
│   ├── main.cpp
│   ├── lexer.cpp
│   ├── parser.cpp
│   ├── ast.cpp
│   ├── interpreter.cpp
│   └── symbol_table.cpp
│
├── include/
│   ├── lexer.h
│   ├── parser.h
│   ├── ast.h
│   ├── interpreter.h
│   └── symbol_table.h
│
├── tests/
│   ├── valid/
│   ├── invalid/
│   └── results/
│
├── docs/
│   ├── ESPECIFICACAO_MINILANG.md
│   ├── grammar.md
│   ├── first_follow.md
│   └── report.md
│
├── jflap/
│   ├── automata/
│   └── grammar/
│
├── Makefile
├── README.md
└── .gitignore
```

## Testes e Validação

O compilador será validado por meio de programas de teste que abrangem diferentes construções da MiniLang, incluindo:

* Cálculo de fatorial.
* Identificação do maior valor entre entradas.
* Soma de valores até a leitura de zero.
* Operações aritméticas e relacionais.
* Estruturas condicionais e de repetição.
* Identificação de erros léxicos e sintáticos.
* Tratamento de divisão por zero.

## Documentação Técnica

A documentação contempla a especificação formal da MiniLang, a definição dos tokens, a gramática livre de contexto, as transformações necessárias para obtenção de uma gramática LL(1), os conjuntos FIRST e FOLLOW, a tabela de parsing, os diagramas JFLAP, as decisões de implementação e os resultados dos testes.

## 🎯 Objetivo Acadêmico

Projeto desenvolvido como atividade prática da disciplina de **Compiladores**, com o objetivo de aplicar conceitos de linguagens formais, autômatos finitos, gramáticas livres de contexto, análise léxica e sintática, construção de árvores sintáticas abstratas e interpretação semântica.

O desenvolvimento integra fundamentos teóricos e implementação prática de um compilador, adotando boas práticas de organização de código, documentação, testes e versionamento.
