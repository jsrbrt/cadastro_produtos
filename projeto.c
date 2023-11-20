#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef struct{
    char nome[50], unidade[3];
    int quantEstoque;
    float precoUnit;
} Tcadastro;

FILE *pCadastro;
Tcadastro itens, itensNull;
int posicao, quantidade, temp2;
char opcao, tempNome[50], confirma, itemCorreto;
float temp3;

void abrirArquivo(){
    pCadastro = fopen("cadastros.txt", "r+b");
    if (pCadastro == NULL) pCadastro = fopen("cadastros.txt", "w+b");
}

void lerRegistro(){
    rewind(pCadastro);
    fread(&itens, sizeof(Tcadastro), 1, pCadastro);

    for(quantidade = 0; feof(pCadastro) == 0; quantidade++)
        fread(&itens, sizeof(Tcadastro), 1, pCadastro);
}

void cabecalho(){
    system("cls");
    printf("\n============= COMERCIO LTDA ============");
    printf("\n==== SISTEMA DE CONTROLE DE ESTOQUE ====\n");
}

char fazerNovamente(char nomeOp[10]){
    printf("\n\nDeseja %s outro item (S/N)? ", nomeOp);
    scanf("%s", &opcao);

    return opcao;
}

void mostrarItem(int pos){
    fseek(pCadastro, pos*sizeof(Tcadastro), SEEK_SET);
    fread(&itens, sizeof(Tcadastro), 1, pCadastro);
   
    printf("\n========================================");
    printf("\nNome do Item.........: %s", itens.nome);
    printf("\nPreco do Item........: R$%.2f", itens.precoUnit);
    printf("\nQuantidade em estoque: %d", itens.quantEstoque);
    printf("\nUnidade..............: %s", itens.unidade);
    printf("\n========================================\n");
}

int descobrirPosicao(char cod[50]){
    rewind(pCadastro);
    fread(&itens, sizeof(Tcadastro), 1, pCadastro);
   
    for(int p = 0; feof(pCadastro) == 0; p++){
        if (strcmp(itens.nome, cod) == 0) return p;

        fread(&itens, sizeof(Tcadastro), 1, pCadastro);
    }
    return -1;
}

char checarCerteza(int pos){
    itemCorreto = 'n';

    if (pos == -1){
        printf("\nNome do item invalido ou nao existe.\n");
        system("pause");
    }
    else{
        mostrarItem(pos);
        printf("\nEste e o item correto (S/N)? ");
        scanf("%s", &itemCorreto);
    }
    return itemCorreto;
}

void confirmarAcao(char nomeOp[10]){
    printf("\nConfirma %s de item (S/N)? ", nomeOp);
    scanf("%s", &confirma);
}

void removerItem(){
    do{
        cabecalho();
        printf("\n============= REMOVER ITEM =============\n\n");

        printf("Insira o nome do item: ");
        fflush(stdin);
        gets(tempNome);

        posicao = descobrirPosicao(tempNome);
        itemCorreto = checarCerteza(posicao);

        if(itemCorreto == 's'){
            strcpy(itensNull.nome, "0");

            confirmarAcao("remocao");
            if(confirma == 's'){
                fseek(pCadastro, posicao *sizeof(Tcadastro), SEEK_SET);
                fwrite(&itensNull, sizeof(Tcadastro), 1, pCadastro);
                //quantidade--;
                printf("Item removido com sucesso.");
            }
            else printf("Remocao cancelada.");
        }
        opcao = fazerNovamente("remover");
    } while (opcao == 's');
}

void consultarItens(){
    do{
        cabecalho();
        printf("\nInsira o nome do item: ");
        fflush(stdin);
        gets(tempNome);

        posicao = descobrirPosicao(tempNome);

        if(posicao == -1) {
            printf("Item nao encontrado.\n");
            system("pause");
        }
        else mostrarItem(posicao);

        opcao = fazerNovamente("consultar");
    } while(opcao == 's');
}

void alterarItem(){
    do{
        cabecalho();
        printf("\n========== ALTERAR ITENS ==========\n");

        printf("Insira o nome do item: ");
        fflush(stdin);
        gets(tempNome);

        posicao = descobrirPosicao(tempNome);
        itemCorreto = checarCerteza(posicao);

        if (itemCorreto == 's'){
            printf("\nAlterando item...\n\n");

            do{
                printf("PRECO.........: ");
                scanf("%f", &temp3);

                if(temp3 == 0) printf("Preco deve ser diferente de 0!\n");
                else itens.precoUnit = temp3;
            } while (temp3 == 0);

            printf("UNIDADE.......: ");
            fflush(stdin);
            gets(itens.unidade);

            do{
                printf("QUANTIDADE....: ");
                scanf("%d", &temp2);

                if(temp2 < 0) printf("A quantidade deve ser maior ou igual a 0!\n");
                else itens.quantEstoque = temp2;
            } while (temp2 < 0);

            confirmarAcao("alteracao");
            if(confirma == 's'){
                fseek(pCadastro, posicao *sizeof(Tcadastro), SEEK_SET);
                fwrite(&itens, sizeof(Tcadastro), 1, pCadastro);
                mostrarItem(posicao);
                printf("\nItem alterado com sucesso!");
            }
            else printf("Inclusao cancelada");
        }
        opcao = fazerNovamente("alterar");
    } while(opcao == 's');  
}

char impedirIgual(char cod[50]){
    rewind(pCadastro);
    fread(&itens, sizeof(Tcadastro), 1, pCadastro);

    while(feof(pCadastro) == 0){
        if (strcmp(itens.nome, cod) == 0){
            printf("\nEste item ja esta cadastrado!");
            mostrarItem(descobrirPosicao(cod));
            system("pause");
            
            return 'n';
        }
        fread(&itens, sizeof(Tcadastro), 1, pCadastro);
    }
    return 's';
}

void incluirItens(){
    char aux;
    do{
        do{
            cabecalho();
            printf("\n========== INCLUSAO DE ITENS ==========\n\n");
            printf("NOME..........: ");
            fflush(stdin);
            gets(tempNome);
            aux = impedirIgual(tempNome);

            if (aux == 's') strcpy(itens.nome,tempNome);
        } while(aux == 'n');
       
        do{
            printf("PRECO.........: ");
            scanf("%f", &temp3);

            if(temp3 == 0) printf("Preco deve ser diferente de 0!\n");
            else itens.precoUnit = temp3;
        } while (temp3 == 0);
       
        printf("UNIDADE.......: ");
        fflush(stdin);
        gets(itens.unidade);
       
        do{
            printf("QUANTIDADE....: ");
            scanf("%d", &temp2);

            if(temp2 < 0) printf("A quantidade deve ser maior ou igual a 0!\n");
            else itens.quantEstoque = temp2;
        } while (temp2 < 0);

        confirmarAcao("inclusao");
        if(confirma == 's'){
            fseek(pCadastro, 0, SEEK_END);
            fwrite(&itens, sizeof(Tcadastro), 1, pCadastro);
            quantidade++;
            printf("\nItem cadastrado com sucesso!\n");
        }
        else printf("Inclusao cancelada");
     
        opcao = fazerNovamente("cadastrar");
    } while(opcao == 's');
}

void entradaItens(){
    int qntd;
    do{
        cabecalho();
        printf("\n========== ENTRADA DE ITENS ==========\n");
        printf("\nNome do produto: ");
        fflush(stdin);
        gets(tempNome);

        posicao = descobrirPosicao(tempNome);
        itemCorreto = checarCerteza(posicao);

        if(itemCorreto == 's'){
            printf("\n========== ENTRADA DE ITENS ==========\n");
           
            printf("Quantidade de entrada: ");
            scanf("%d", &qntd);
            itens.quantEstoque = itens.quantEstoque + qntd;
           
            fseek(pCadastro, posicao *sizeof(Tcadastro), SEEK_SET);
            fwrite(&itens, sizeof(Tcadastro), 1, pCadastro);

            printf("\nEstoque alterado com sucesso.\n");
            mostrarItem(posicao);
        }
        opcao = fazerNovamente("aumentar o estoque de");
    } while(opcao == 's');
}

void saidaItens(){
    int qntd;
    do{
        cabecalho();
        printf("\n========== SAIDA DE ITENS ==========\n");
        printf("\nNome do produto: ");
        fflush(stdin);
        gets(tempNome);

        posicao = descobrirPosicao(tempNome);
        itemCorreto = checarCerteza(posicao);

        if(itemCorreto == 's'){
            printf("\n========== SAIDA DE ITENS ==========\n");
            printf("Quantidade de saida: ");
            scanf("%d", &qntd);
            itens.quantEstoque = itens.quantEstoque - qntd;
           
            fseek(pCadastro, posicao *sizeof(Tcadastro), SEEK_SET);
            fwrite(&itens, sizeof(Tcadastro), 1, pCadastro);

            printf("\nEstoque alterado com sucesso!\n");
            mostrarItem(posicao);
        }
        opcao = fazerNovamente("diminuir o estoque de");
    } while(opcao == 's');
}

void swap(Tcadastro** lista, int n1, int n2){
    Tcadastro* temp = lista[n2];
    lista[n2] = lista[n1];
    lista[n1] = temp;
}

void sort(Tcadastro** lista, int n){
    int i, j;
    unsigned int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp(lista[j]->nome, lista[j + 1]->nome) > 0) {
                swap(lista, j, (j + 1));
                swapped = 1;
            }
        }
        if (swapped == 0) break;
    }
}

void listaPreco(){
    cabecalho();

    Tcadastro** pointer = (Tcadastro**) malloc(sizeof(Tcadastro*) * quantidade);
    rewind(pCadastro);
    for(posicao = 0; feof(pCadastro) == 0; posicao++){
        pointer[posicao] = (Tcadastro*) malloc(sizeof(Tcadastro));
        fread(pointer[posicao], sizeof(Tcadastro), 1, pCadastro);
    }
    sort(pointer, quantidade);

    printf("\nNome do Item \tUnidade \tPreco\n");
    for(posicao = 0; posicao < quantidade; posicao++){
        while(strcmp(pointer[posicao]->nome, "0") == 0) posicao++;

        printf("%s \t\t%s \t\tR$%.2f\n", pointer[posicao]->nome, pointer[posicao]->unidade, pointer[posicao]->precoUnit);
    }
    system("pause");
}

void balancoProdutos(){
    cabecalho();

    Tcadastro** pointer = (Tcadastro**) malloc(sizeof(Tcadastro*) * quantidade);
    rewind(pCadastro);
    for(posicao = 0; feof(pCadastro) == 0; posicao++){
        pointer[posicao] = (Tcadastro*) malloc(sizeof(Tcadastro));
        fread(pointer[posicao], sizeof(Tcadastro), 1, pCadastro);
    }
    sort(pointer, quantidade);

    printf("\nNome do Item \tUnidade \tPreco Unitario \tQuantidade \tPreco total\n");
    for(posicao = 0; posicao < quantidade; posicao++){
        while(strcmp(pointer[posicao]->nome, "0") == 0) posicao++;

        printf("%s \t\t%s \t\tR$%.2f \t\t%d \t\tR$%.2f \n", pointer[posicao]->nome, pointer[posicao]->unidade, pointer[posicao]->precoUnit, pointer[posicao]->quantEstoque, pointer[posicao]->precoUnit*pointer[posicao]->quantEstoque);
    }
    system("pause");
}

void relatorios(){
    int pag;
    do{
        cabecalho();
        printf("\n============== RELATORIO ===============\n");
        printf("\n\t1- LISTA DE PRECOS \n\t2- BALANCO FISICO/FINANCEIRO \n\t0- RETORNAR\n");
        printf("\n\tOpcao: ");
        scanf("%d", &pag);
       
        switch(pag){
            case 1: listaPreco();      break;
            case 2: balancoProdutos(); break;
            case 0: break;
            default: printf("Opcao invalida!"); break;
        }
    } while(pag != 0);
}

void reajustarPreco(){
    float percentual;
    do{
        cabecalho();
        printf("\n========== REAJUSTE DE PRECO ==========\n");
        printf("\nNome do produto: ");
        fflush(stdin);
        gets(tempNome);

        posicao = descobrirPosicao(tempNome);
        itemCorreto = checarCerteza(posicao);

        if(itemCorreto == 's'){
            printf("\n========== REAJUSTANDO PRECO ==========\n");
            printf("PERCENTUAL DE REAJUSTE (3.5 ou -15.8): ");
            scanf("%f", &percentual);

            itens.precoUnit = itens.precoUnit + ((percentual*itens.precoUnit)/100);

            confirmarAcao("reajuste");
            if(confirma == 's'){
                fseek(pCadastro, posicao *sizeof(Tcadastro), SEEK_SET);
                fwrite(&itens, sizeof(Tcadastro), 1, pCadastro);

                printf("\nPreco alterado com sucesso.\n");
                mostrarItem(posicao);
            }
            else printf("Reajuste cancelado");
        }
        opcao = fazerNovamente("alterar o preco de");
    } while(opcao == 's');
}

void movimentarItem(){
    int pag;
    do{
        cabecalho();
        printf("\n==========MOVIMENTACAO DE ITENS==========\n");
        printf("\n\t1- ENTRADA \n\t2- SAIDA \n\t0- RETORNAR\n");
        printf("\n\tOpcao: ");
        scanf("%d", &pag);
       
        switch(pag){
            case 1: entradaItens(); break;
            case 2: saidaItens();   break;
            case 0: break;
            default: printf("Opcao invalida!"); break;
        }
    } while (pag != 0);
}

void cadastrarItem(){
    int pagina;    
    do{
        cabecalho();
        printf("\n========== CADASTRO DE ITENS ===========\n");
        printf("\n\t1- INCLUIR \n\t2- ALTERAR \n\t3- CONSULTAR \n\t4- EXCLUIR \n\t0- RETORNAR\n");
        printf("\n\tOpcao: ");
        scanf("%d", &pagina);

        switch(pagina){
            case 1: incluirItens();   break;
            case 2: alterarItem();    break;
            case 3: consultarItens(); break;
            case 4: removerItem();    break;
            case 0: break;
            default: printf("Opcao invalida!"); break;
        }
    } while (pagina != 0);
}

int main(){
    int pag;
    abrirArquivo();
    lerRegistro();

    do{
        cabecalho();
        printf("============ MENU PRINCIPAL ============\n");
        printf("\n\t1- CADASTRO DE ITENS \n\t2- MOVIMENTACAO \n\t3- REAJUSTE DE VALOR \n\t4- RELATORIO \n\t0- FINALIZAR\n");
        printf("\n\tOpcao: ");
        scanf("%d", &pag);

        switch(pag){
            case 1: cadastrarItem();   break;
            case 2: movimentarItem();  break;
            case 3: reajustarPreco();  break;
            case 4: relatorios();      break;
            case 0: fclose(pCadastro); break;
            default: printf("Opcao invalida\n\n"); break;
        }
    } while (pag != 0);
}
