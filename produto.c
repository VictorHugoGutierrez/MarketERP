#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO "produto.dat"

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
    Data vencimento;
} Produto;

int obterProximoCodigo(){
    FILE *f = fopen(ARQUIVO, "rb");
    Produto produto;
    int id = 0;

    if(f == NULL) return 1;

    while(fread(&produto, sizeof(Produto), 1, f)){
        if(produto.codigo > id){
            id = produto.codigo;
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
        printf("Código: %d | Nome: %s | Quantidade: %d | Preço: R$ %.2f | Data Vencimento: %02d/%02d/%d\n", produto.codigo, produto.nome, produto.quantidade, produto.preco, produto.vencimento.dia, produto.vencimento.mes, produto.vencimento.ano);
    }

    fclose(f);
}

void buscarProduto(int codigo){
    FILE *f = fopen(ARQUIVO, "rb");
    Produto produto;
    int encontrado = 0;

    if(f == NULL){
        printf("Nenhum produto cadastrado...\n");
        return;
    }

    while(fread(&produto, sizeof(Produto), 1, f)){
        if(produto.codigo == codigo){
            printf("Produto encontrado: %s | Quantidade: %d | Preço: R$ %.2f | Data Vencimento: %02d/%02d/%d\n", produto.nome, produto.quantidade, produto.preco, produto.vencimento.dia, produto.vencimento.mes, produto.vencimento.ano);
            encontrado = 1;
            break;
        }
    }

    if(!encontrado){
        printf("Produto com código %d não encontrado...\n", codigo);
    }
    fclose(f);
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

void mainProduto(){
    setlocale(LC_ALL, "Portuguese");

    int opcao, codigo;
    Produto produto;

    do{
        printf("\n=== MARKET ERP ===\n");
        printf("1. Cadastrar Produto \n");
        printf("2. Listar Produtos \n");
        printf("3. Buscar produto por código \n");
        printf("4. Remover Produto\n");
        printf("0. Sair \n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                system("cls");
                produto.codigo = obterProximoCodigo();
                printf("\nCódigo: %d\n", produto.codigo);
                printf("Nome: ");
                getchar();
                gets(produto.nome);
                printf("Quantidade: ");
                scanf("%d", &produto.quantidade);
                printf("Preço: ");
                scanf("%f", &produto.preco);
                printf("Data de Vecimento (dd/mm/aaaa): ");
                scanf("%02d/%02d/%d", &produto.vencimento.dia, &produto.vencimento.mes, &produto.vencimento.ano);
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
                printf("\nDigite o código do produto: ");
                scanf("%d", &codigo);
                buscarProduto(codigo);
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
            case 0:
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
