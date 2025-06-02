#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "lotes.h"
#include "produto.h"

#define ARQUIVO "estoque.dat"

typedef struct {
    int id;
    int produtoId;
    int loteId;
    char tipo[10];
    int quantidade;
    int dia, mes, ano;
    char observacao[100];
} Estoque;

int obterEstoqueProduto(int produtoId) {
    FILE *f = fopen("lotes.dat", "rb");
    if (f == NULL) return 0;

    LoteProduto lote;
    int total = 0;

    while (fread(&lote, sizeof(LoteProduto), 1, f)) {
        if (lote.produtoId == produtoId && verificarValidade(lote.dia, lote.mes, lote.ano)) {
            total += lote.quantidade;
        }
    }

    fclose(f);
    return total;
}

void obterEstoqueTodosProdutos(){
    FILE *f =fopen("produto.dat", "rb");

    if(f == NULL){
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    Produto produto;
    while (fread(&produto, sizeof(produto), 1, f)){
        if(produto.id != 0){
            buscarProdutoId(produto.id, 1);
            printf("Estoque disponível: %d\n", obterEstoqueProduto(produto.id));
        }
    }
}

void mainEstoque(){
    setlocale(LC_ALL, "Portuguese");

    int opcao, codigo;
    Estoque estoque;

    do{
        printf("\n=== Controle de Estoque ===\n");
        printf("1. Listar o estoque de todos os produtos \n");
        printf("2. Listar o estoque de um produto  \n");
        printf("3. Registrar saída do estoque \n");
        printf("0. Sair \n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                system("cls");
                obterEstoqueTodosProdutos();
                system("pause");
                system("cls");
                break;
            case 2:
                system("cls");
                printf("\nDigite o ID do produto: ");
                scanf("%d", &codigo);
                Produto produto = buscarProdutoId(codigo, 1);
                if(produto.id != 0){
                    printf("Estoque disponível: %d\n", obterEstoqueProduto(codigo));
                }
                system("pause");
                system("cls");
                break;
            case 0:
                system("cls");
                printf("Saindo...\n");
                system("pause");
                system("cls");
                break;
            default:
                printf("Opção inválida!\n");
                system("pause");
                system("cls");
        }

    } while(opcao != 0);

    return;
}
