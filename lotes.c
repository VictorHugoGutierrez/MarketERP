#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "produto.h"
#include "lotes.h"

#define ARQUIVO "lotes.dat"

int obterProximoIdLote(){
    FILE *f = fopen(ARQUIVO, "rb");
    LoteProduto lote;
    int id = 0;

    if(f == NULL) return 1;

    while(fread(&lote, sizeof(LoteProduto), 1, f)){
        if(lote.id > id){
            id = lote.id;
        }
    }

    fclose(f);
    return id + 1;
}

void salvarMovimentacaoLote(LoteProduto lote){
    time_t t = time(NULL);
    struct tm *dataAtual = localtime(&t);

    lote.diaMovimentacao = dataAtual->tm_mday;
    lote.mesMovimentacao = dataAtual->tm_mon + 1;
    lote.anoMovimentacao = dataAtual->tm_year + 1900;

    FILE *f = fopen(ARQUIVO, "ab");
    fwrite(&lote, sizeof(LoteProduto), 1, f);
    fclose(f);
}

int verificarValidade(int dia, int mes, int ano) {
    time_t t = time(NULL);
    struct tm *dataAtual = localtime(&t);

    int anoAtual = dataAtual->tm_year + 1900;
    int mesAtual = dataAtual->tm_mon + 1;
    int diaAtual = dataAtual->tm_mday;

    if (ano > anoAtual) return 1;
    if (ano == anoAtual && mes > mesAtual) return 1;
    if (ano == anoAtual && mes == mesAtual && dia >= diaAtual) return 1;

    return 0;
}

const char* tipoParaTexto(TipoMovimentacao tipo){
    switch (tipo){
        case ENTRADA:
            return "Entrada";
        case SAIDA:
            return "Saída";
        default:
            return "Desconhecido";
    }
}

int obterQuantidadeDisponivelLote(int idLoteEntrada){
    FILE *f = fopen(ARQUIVO, "rb");
    if (!f) return 0;

    LoteProduto lote;
    int totalEntrada = 0;
    int totalSaida = 0;

    while (fread(&lote, sizeof(LoteProduto), 1, f)){
        if (lote.id == idLoteEntrada && lote.tipo == ENTRADA){
            totalEntrada = lote.quantidade;
        } else if (lote.tipo == SAIDA && lote.loteOrigemId == idLoteEntrada){
            totalSaida += lote.quantidade;
        }
    }

    fclose(f);
    int disponivel = totalEntrada - totalSaida;
    if(disponivel < 0) disponivel = 0;

    return disponivel;
}

void listarLotes(){
    FILE *f = fopen(ARQUIVO, "rb");
     if(f == NULL){
        printf("Nenhum lote cadastrado.\n");
        return;
    }

    LoteProduto lote;
    printf("\n=== LOTES ===\n");

    while(fread(&lote, sizeof(LoteProduto), 1, f)){
        Produto prod = buscarProdutoId(lote.produtoId, 0);
        printf("Lote ID: %d\n", lote.id);
        printf("Produto: %s\n", prod.nome);
        printf("Tipo: %s\n", tipoParaTexto(lote.tipo));
        printf("Quantidade: %d\n", lote.quantidade);
        printf("Validade: %02d/%02d/%04d\n", lote.diaValidade, lote.mesValidade, lote.anoValidade);
        printf("Data: %02d/%02d/%04d\n", lote.diaMovimentacao, lote.mesMovimentacao, lote.anoValidade);

        if(lote.tipo == ENTRADA){
            int disponivel = obterQuantidadeDisponivelLote(lote.id);
            printf("Quantidade disponível: %d\n", disponivel);
            printf("Status: %s\n", verificarValidade(lote.diaValidade, lote.mesValidade, lote.anoValidade) ? "Válido" : "Vencido");
        }
        printf("-------------------------\n");
    }

    fclose(f);
}

void registrarSaidaLote() {
    int id, quantidade;

    listarLotes();
    printf("\nDigite o ID do lote de entrada para saída: ");
    scanf("%d", &id);

    FILE *f = fopen("lotes.dat", "rb");
    if (!f) {
        printf("Erro ao abrir arquivo de lotes.\n");
        return;
    }

    LoteProduto loteEntrada;
    int encontrado = 0;

    while (fread(&loteEntrada, sizeof(LoteProduto), 1, f)) {
        if (loteEntrada.id == id && loteEntrada.tipo == ENTRADA) {
            encontrado = 1;
            break;
        }
    }
    fclose(f);

    if (!encontrado) {
        printf("Lote de entrada com ID %d não encontrado.\n", id);
        return;
    }

    if (!verificarValidade(loteEntrada.diaValidade, loteEntrada.mesValidade, loteEntrada.anoValidade)) {
        printf("Este lote está vencido e não pode ser utilizado para saída.\n");
        return;
    }

    int disponivel = obterQuantidadeDisponivelLote(id);
    if (disponivel <= 0) {
        printf("Este lote não possui quantidade disponível.\n");
        return;
    }

    printf("Quantidade disponível no lote %d: %d\n", id, disponivel);
    printf("Quantidade a retirar: ");
    scanf("%d", &quantidade);

    if (quantidade <= 0 || quantidade > disponivel) {
        printf("Quantidade inválida ou insuficiente.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm *dataAtual = localtime(&t);

    if (dataAtual->tm_year + 1900 < loteEntrada.anoMovimentacao ||
        (dataAtual->tm_year + 1900 == loteEntrada.anoMovimentacao && dataAtual->tm_mon + 1 < loteEntrada.mesMovimentacao) ||
        (dataAtual->tm_year + 1900 == loteEntrada.anoMovimentacao && dataAtual->tm_mon + 1 == loteEntrada.mesMovimentacao && dataAtual->tm_mday < loteEntrada.diaMovimentacao)) {
        printf("Data da saída não pode ser anterior à data de entrada do lote.\n");
        return;
    }

    LoteProduto saida;
    saida.id = obterProximoIdLote();
    saida.produtoId = loteEntrada.produtoId;
    saida.quantidade = quantidade;
    saida.diaMovimentacao = dataAtual->tm_mday;
    saida.mesMovimentacao = dataAtual->tm_mon + 1;
    saida.anoMovimentacao = dataAtual->tm_year + 1900;
    saida.tipo = SAIDA;
    saida.loteOrigemId = loteEntrada.id;
    saida.diaValidade = loteEntrada.diaValidade;
    saida.mesValidade = loteEntrada.mesValidade;
    saida.anoValidade = loteEntrada.anoValidade;

    salvarMovimentacaoLote(saida);

    printf("Saída registrada com sucesso.\n");
}

int temSaidasAssociadas(int idLoteEntrada) {
    FILE *f = fopen(ARQUIVO, "rb");
    if (!f) return 0;

    LoteProduto lote;
    int encontrado = 0;

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.tipo == SAIDA && lote.loteOrigemId == idLoteEntrada) {
            encontrado = 1;
            break;
        }
    }

    fclose(f);
    return encontrado;
}

void removerLote(int id){
    FILE *f = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    LoteProduto lote;
    int encontrado = 0;

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.id == id) {
            encontrado = 1;

            if(lote.tipo == ENTRADA && temSaidasAssociadas(lote.id)){
                printf("Este Lote possui saídas associadas e não pode ser removido.\n");
                fwrite(&lote, sizeof(LoteProduto), 1, temp);
                continue;
            }
        } else {
            fwrite(&lote, sizeof(LoteProduto), 1, temp);
        }
    }

    fclose(f);
    fclose(temp);
    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Lote removido com sucesso.\n");
    } else {
        printf("Lote não encontrado.\n");
    }
}

void editarLote(int id) {
    FILE *f = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    LoteProduto lote;
    int encontrado = 0;

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.id == id) {
            encontrado = 1;

            if (lote.tipo == ENTRADA && temSaidasAssociadas(lote.id)) {
                printf("Este lote possui saídas associadas e não pode ser editado.\n");
                fwrite(&lote, sizeof(LoteProduto), 1, temp);
                continue;
            }

            printf("\n--- Editar Lote ---\n");
            printf("ID atual: %d\n", lote.id);
            printf("Produto ID atual: %d\n", lote.produtoId);
            printf("Quantidade atual: %d\n", lote.quantidade);
            printf("Validade atual: %02d/%02d/%04d\n", lote.diaValidade, lote.mesValidade, lote.anoValidade);
            printf("Data atual: %02d/%02d/%04d\n", lote.diaMovimentacao, lote.mesMovimentacao, lote.anoMovimentacao);
            printf("Tipo (não pode ser alterado): %s\n", tipoParaTexto(lote.tipo));

            printf("\nNovo Produto ID: ");
            scanf("%d", &lote.produtoId);

            printf("Nova Quantidade: ");
            scanf("%d", &lote.quantidade);

            printf("Nova Data de Validade (dd/mm/aaaa): ");
            scanf("%d/%d/%d", &lote.diaValidade, &lote.mesValidade, &lote.anoValidade);

            printf("Nova Data (dd/mm/aaaa): ");
            scanf("%d/%d/%d", &lote.diaMovimentacao, &lote.mesMovimentacao, &lote.anoMovimentacao);
        }

        fwrite(&lote, sizeof(LoteProduto), 1, temp);
    }

    fclose(f);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Lote editado com sucesso.\n");
    } else {
        printf("Lote não encontrado.\n");
    }
}

void buscarLoteId(int id) {
    FILE *f = fopen(ARQUIVO, "rb");
    if (f == NULL) {
        printf("Nenhum lote cadastrado.\n");
        return;
    }

    LoteProduto lote;
    int encontrado = 0;

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.id == id) {
            Produto prod = buscarProdutoId(lote.produtoId, 0);
            printf("\n=== LOTE ENCONTRADO ===\n");
            printf("Lote ID: %d\n", lote.id);
            printf("Produto: %s\n", prod.nome);
            printf("Quantidade: %d\n", lote.quantidade);
            printf("Data: %02d/%02d/%04d\n", lote.diaMovimentacao, lote.mesMovimentacao, lote.anoMovimentacao);
            printf("Validade: %02d/%02d/%04d\n", lote.diaValidade, lote.mesValidade, lote.anoValidade);
            printf("Status: %s\n", verificarValidade(lote.diaValidade, lote.mesValidade, lote.anoValidade) ? "Válido" : "Vencido");
            printf("-------------------------\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Lote com ID %d não encontrado.\n", id);
    }

    fclose(f);
}

void buscarLotesProdutoCodigo(int codigoProduto) {
    FILE *f = fopen(ARQUIVO, "rb");
    if (f == NULL) {
        printf("Nenhum lote cadastrado.\n");
        return;
    }

    LoteProduto lote;
    int encontrado = 0;

    printf("\n=== LOTES DO PRODUTO ID %d ===\n", codigoProduto);

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        Produto prod = buscarProdutoCodigo(codigoProduto, 0);
        if (lote.produtoId == prod.id) {
            printf("Lote ID: %d\n", lote.id);
            printf("Produto: %s\n", prod.nome);
            printf("Quantidade: %d\n", lote.quantidade);
            printf("Data: %02d/%02d/%04d\n", lote.diaMovimentacao, lote.mesMovimentacao, lote.anoMovimentacao);
            printf("Validade: %02d/%02d/%04d\n", lote.diaValidade, lote.mesValidade, lote.anoValidade);
            printf("Status: %s\n", verificarValidade(lote.diaValidade, lote.mesValidade, lote.anoValidade) ? "Válido" : "Vencido");
            printf("-------------------------\n");
            encontrado = 1;
        }
    }

    if (!encontrado) {
        printf("Nenhum lote encontrado para o produto ID %d.\n", codigoProduto);
    }

    fclose(f);
}

void mainLote(){
    setlocale(LC_ALL, "Portuguese");
    system("chcp 1252 > null");

    int opcao, id, busca, cadastro;
    LoteProduto lote;

    do{
        printf("\n=== Controle de Lote ===\n");
        printf("1. Cadastrar\n");
        printf("2. Editar\n");
        printf("3. Remover\n");
        printf("4. Listar\n");
        printf("5. Buscar\n");
        printf("0. Sair \n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                system("cls");
                printf("\n1. Entrada");
                printf("\n2. Saída");
                printf("\n0. Sair");
                printf("\nDeseja fazer qual tipo de cadastro: ");
                scanf("%d", &cadastro);

                switch (cadastro){
                    case 1:
                        system("cls");
                        listarProdutos();
                        printf("\nEscolha um produto pelo ID: ");
                        scanf("%d", &lote.produtoId);
                        printf("Quantidade: ");
                        scanf("%d", &lote.quantidade);
                        printf("Data de Validade (dd/mm/aaaa): ");
                        scanf("%d/%d/%d", &lote.diaValidade, &lote.mesValidade, &lote.anoValidade);
                        lote.id = obterProximoIdLote();
                        lote.tipo = ENTRADA;
                        salvarMovimentacaoLote(lote);
                        break;
                    case 2:
                        system("cls");
                        registrarSaidaLote();
                        break;
                    case 0:
                        system("cls");
                        printf("\nSaindo...\n");
                        break;
                    default:
                        printf("Opção inválida!\n");
                }
                break;
            case 2:
                system("cls");
                printf("\nDigite o id do lote: ");
                scanf("%d", &id);
                editarLote(id);
                break;
            case 3:
                system("cls");
                printf("\nDigite o id do lote: ");
                scanf("%d", &id);
                removerLote(id);
                break;
            case 4:
                system("cls");
                listarLotes();
                break;
            case 5:
                system("cls");
                printf("\n1. Id do Lote");
                printf("\n2. Código do Produto");
                printf("\n0. Sair ");
                printf("\nEscolha a opção desejada: ");
                scanf("%d", &busca);

                switch (busca){
                    case 1:
                        system("cls");
                        printf("\nDigite o Id do Lote: ");
                        scanf("%d", &busca);
                        buscarLoteId(busca);
                        break;
                    case 2:
                        system("cls");
                        printf("\nDigite o Código do Produto: ");
                        scanf("%d", &busca);
                        buscarLotesProdutoCodigo(busca);
                        break;
                    case 0:
                        system("cls");
                        printf("\nSaindo...\n");
                        break;
                    default:
                        printf("Opção inválida!\n");

                }
                break;
            case 0:
                system("cls");
                printf("\nSaindo...\n");
                break;
            default:
                printf("Opção inválida!\n");

        }
        system("pause");
        system("cls");
    } while(opcao != 0);

    return;
}
