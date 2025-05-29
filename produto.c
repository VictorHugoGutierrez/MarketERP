#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO "produto.dat"

typedef struct {
    int id;
    int codigo;
    char nome[100];
    float preco;
} Produto;

int obterProximoIdProduto(){
    FILE *f = fopen(ARQUIVO, "rb");
    Produto produto;
    int id = 0;

    if(f == NULL) return 1;

    while(fread(&produto, sizeof(Produto), 1, f)){
        if(produto.id > id){
            id = produto.id;
        }
    }

    fclose(f);
    return id + 1;
}

void salvarProduto(Produto produto){
    FILE *f = fopen(ARQUIVO, "ab");
    fwrite(&produto, sizeof(Produto), 1, f);
    fclose(f);
}

void listarProdutos(){
    FILE *f = fopen(ARQUIVO, "rb");
    Produto produto;

    if(f == NULL){
        printf("Nenhum produto cadastrado...\n");
        return;
    }

    printf("\n--- LISTA DE PRODUTOS ---\n");

    while (fread(&produto, sizeof(Produto), 1, f)) {
        printf("Id: %d | Código: %d | Nome: %s | Preço: R$ %.2f\n", produto.id, produto.codigo, produto.nome, produto.preco);
        printf("-------------------------\n");
    }

    fclose(f);
}

Produto buscarProdutoCodigo(int codigo, int exibe){
    FILE *f = fopen(ARQUIVO, "rb");
    Produto produto;
    int encontrado = 0;

    if(f == NULL){
        printf("Nenhum produto cadastrado...\n");
        return produto;
    }

    while(fread(&produto, sizeof(Produto), 1, f)){
        if(produto.codigo == codigo){
            if(exibe == 1){
                printf("\nId: %d | Código: %d | Nome: %s | Preço: R$ %.2f\n", produto.id, produto.codigo, produto.nome, produto.preco);
            }
            encontrado = 1;
            break;
        }
    }

    if(!encontrado){
        printf("Produto com código %d não encontrado...\n", codigo);
    }
    fclose(f);

    return produto;
}

Produto buscarProdutoId(int id, int exibe){
    FILE *f = fopen(ARQUIVO, "rb");
    Produto produto;
    int encontrado = 0;

    if(f == NULL){
        printf("Nenhum produto cadastrado...\n");
        return produto;
    }

    while(fread(&produto, sizeof(Produto), 1, f)){
        if(produto.id == id){
            if(exibe == 1){
                printf("\nId: %d | Código: %d | Nome: %s | Preço: R$ %.2f\n", produto.id, produto.codigo, produto.nome, produto.preco);
            }
            encontrado = 1;
            break;
        }
    }

    if(!encontrado){
        printf("Produto com id %d não encontrado...\n", id);
        return produto;
    }
    fclose(f);

    return produto;
}

void removerProduto(int codigo) {
    FILE *f = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    Produto produto;
    int encontrado = 0;

    while (fread(&produto, sizeof(Produto), 1, f)) {
        if (produto.codigo != codigo) {
            fwrite(&produto, sizeof(Produto), 1, temp);
        } else {
            encontrado = 1;
        }
    }

    fclose(f);
    fclose(temp);
    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Produto removido com sucesso.\n");
    } else {
        printf("Produto não encontrado.\n");
    }
}

void editarProduto(int codigo) {
    FILE *f = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    Produto produto;
    int encontrado = 0;

    while (fread(&produto, sizeof(Produto), 1, f)) {
        if (produto.codigo == codigo) {
            encontrado = 1;

            printf("\n--- Editar Produto ---\n");
            printf("ID: %d\n", produto.id);
            printf("Código: %d\n", produto.codigo);
            printf("Nome atual: %s\n", produto.nome);
            printf("Preço atual: %f\n", produto.preco);

            printf("\nNovo código do Produto: ");
            scanf("%d", &produto.codigo);
            getchar();
            printf("Novo nome do Produto: ");
            gets(produto.nome);
            printf("Novo preço do Produto: ");
            scanf("%f", &produto.preco);
        }

        fwrite(&produto, sizeof(Produto), 1, temp);
    }

    fclose(f);
    fclose(temp);

    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) {
        printf("Produto editado com sucesso.\n");
    } else {
        printf("Produto não encontrado.\n");
    }
}

void mainProduto(){
    setlocale(LC_ALL, "Portuguese");

    int opcao, codigo, busca;
    Produto produto;

    do{
        printf("\n=== Produtos ===\n");
        printf("1. Cadastrar\n");
        printf("2. Listar\n");
        printf("3. Buscar\n");
        printf("4. Remover\n");
        printf("5. Editar\n");
        printf("0. Sair \n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                system("cls");
                produto.id = obterProximoIdProduto();
                printf("\nId: %d", produto.id);
                printf("\nCódigo: ");
                scanf("%d", &produto.codigo);
                getchar();
                printf("Nome: ");
                gets(produto.nome);
                printf("Preço: ");
                scanf("%f", &produto.preco);
                salvarProduto(produto);
                system("pause");
                system("cls");
                break;
            case 2:
                system("cls");
                listarProdutos();
                system("pause");
                system("cls");
                break;
            case 3:
                system("cls");
                printf("\n1. Código Produto");
                printf("\n2. Id Produto");
                printf("\n0. Sair");
                printf("\nDeseja fazer qual tipo de busca: ");
                scanf("%d", &busca);

                switch(busca){
                    case 1:
                        printf("Digite o código do produto: ");
                        scanf("%d", &codigo);
                        system("cls");
                        buscarProdutoCodigo(codigo, 1);
                        break;
                    case 2:
                        printf("Digite o id do produto: ");
                        scanf("%d", &codigo);
                        system("cls");
                        buscarProdutoId(codigo, 1);
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
                break;
            case 4:
                system("cls");
                printf("\nDigite o código do produto: ");
                scanf("%d", &codigo);
                removerProduto(codigo);
                system("pause");
                system("cls");
                break;
            case 5:
                system("cls");
                printf("\nDigite o código do produto: ");
                scanf("%d", &codigo);
                editarProduto(codigo);
                system("pause");
                system("cls");
                break;
            case 0:
                system("cls");
                printf("\nSaindo...\n");
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
