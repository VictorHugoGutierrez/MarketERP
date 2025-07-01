# Sistema de Gerenciamento de Estoque - Market ERP

## Visão Geral do Projeto

Este repositório contém um sistema de gerenciamento de estoque, desenvolvido como parte do Projeto Interdisciplinar para as disciplinas de Algoritmos e Programação, Linguagem de Programação 1 e Gestão de Projetos do Instituto Federal de São Paulo no campus de Campinas. O programa visa demonstrar o controle de entradas e saídas de produtos e a organização do estoque em um ambiente de console.

## Estrutura do Repositório

O projeto está organizado em módulos, cada um responsável por um conjunto específico de funcionalidades:

* **Módulo de Produtos:** Gerencia o cadastro, listagem, busca, remoção e edição de produtos.
* **Módulo de Lotes:** Controla as movimentações de estoque, incluindo o registro de entradas e saídas de produtos através de lotes.
* **Módulo de Relatórios:** Oferece diversas funcionalidades para consulta e análise de dados do estoque, como estoque geral, estoque crítico, lotes vencidos, entre outros.

## Como Compilar e Rodar

O executável do projeto já está disponível na pasta `bin/`. Você tem duas opções para executar o programa:

1.  **Executar Diretamente (Recomendado):**
    * Navegue até a pasta `bin/` em seu terminal.
    * Execute o arquivo `main.exe` (no Windows) ou `./main` (no Linux/macOS).
    * Exemplo (Windows): `cd bin` e depois `main.exe`
    * Exemplo (Linux/macOS): `cd bin` e depois `./main`

2.  **Compilar e Rodar (Caso queira modificar o código):**
    * **Pré-requisitos:**
        * Compilador C (ex: GCC, que é padrão no Code::Blocks).
        * Ambiente de Desenvolvimento: Code::Blocks (recomendado).
    * **Clonar o Repositório:**
        ```bash
        git clone https://github.com/VictorHugoGutierrez/MarketERP.git
        cd MarketERP
        ```
    * **Abrir no Code::Blocks:**
        * Abra o arquivo de projeto (`.cbp`) na raiz do seu repositório no Code::Blocks.
    * **Compilar e Executar:**
        * No Code::Blocks, vá em `Build > Build and Run` (ou pressione `F9`).
        * A aplicação será executada em um terminal de console.

## Contribuição

Este é um projeto desenvolvido para fins acadêmicos. Contribuições diretas não são esperadas, mas sugestões e feedback são sempre bem-vindos para futuras melhorias.

---
