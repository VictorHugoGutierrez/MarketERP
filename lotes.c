#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "produto.h"

#define ARQUIVO "lotes.dat"

typedef struct {
    int id;
    int produtoId;
    int quantidade;
    int dia, mes, ano;
} LoteProduto;

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

void salvarLote(LoteProduto lote){
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
        printf("Quantidade: %d\n", lote.quantidade);
        printf("Validade: %02d/%02d/%04d\n", lote.dia, lote.mes, lote.ano);
        printf("Status: %s\n", verificarValidade(lote.dia, lote.mes, lote.ano) ? "Válido" : "Vencido");
        printf("-------------------------\n");
    }

    fclose(f);
}

void removerLote(int id){
    FILE *f = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    LoteProduto lote;
    int encontrado = 0;

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.id != id) {
            fwrite(&lote, sizeof(LoteProduto), 1, temp);
        } else {
            encontrado = 1;
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

            printf("\n--- Editar Lote ---\n");
            printf("ID atual: %d\n", lote.id);
            printf("Produto ID atual: %d\n", lote.produtoId);
            printf("Quantidade atual: %d\n", lote.quantidade);
            printf("Validade atual: %02d/%02d/%04d\n", lote.dia, lote.mes, lote.ano);

            printf("\nNovo Produto ID: ");
            scanf("%d", &lote.produtoId);

            printf("Nova Quantidade: ");
            scanf("%d", &lote.quantidade);

            printf("Nova Data de Validade (dd/mm/aaaa): ");
            scanf("%d/%d/%d", &lote.dia, &lote.mes, &lote.ano);
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
            printf("Validade: %02d/%02d/%04d\n", lote.dia, lote.mes, lote.ano);
            printf("Status: %s\n", verificarValidade(lote.dia, lote.mes, lote.ano) ? "Válido" : "Vencido");
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
            printf("Validade: %02d/%02d/%04d\n", lote.dia, lote.mes, lote.ano);
            printf("Status: %s\n", verificarValidade(lote.dia, lote.mes, lote.ano) ? "Válido" : "Vencido");
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

    int opcao, id, opcao2;
    LoteProduto lote;

    do{
        printf("\n=== Controle Lote ===\n");
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
                listarProdutos();
                printf("\nEscolha um produto: ");
                scanf("%d", &lote.produtoId);
                printf("Quantidade: ");
                scanf("%d", &lote.quantidade);
                printf("Data de Validade (dd/mm/aaaa): ");
                scanf("%d/%d/%d", &lote.dia, &lote.mes, &lote.ano);
                lote.id = obterProximoIdLote();
                salvarLote(lote);
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
                scanf("%d", &opcao2);

                switch (opcao2){
                    case 1:
                        system("cls");
                        printf("\nDigite o Id do Lote: ");
                        scanf("%d", &opcao2);
                        buscarLoteId(opcao2);
                        break;
                    case 2:
                        system("cls");
                        printf("\nDigite o Código do Produto: ");
                        scanf("%d", &opcao2);
                        buscarLotesProdutoCodigo(opcao2);
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
