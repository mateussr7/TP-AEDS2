
#include <funcionarios.h>
#include <stdlib.h>
#include <limits.h>

#include <particoes.h>

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc) {
    rewind(arq); //posiciona cursor no inicio do arquivo


    int reg = 0;

    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++) {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], p);
                imprime_funcionario(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n){
    int qtdLidos = 0;
    TFunc** vet = malloc(M * sizeof(TFunc*));
    int inVet = 0;

    FILE* repo = fopen("repository.dat", "wb+");
    int inRepo = 0;


}

TFunc *getFuncionario(FILE* arq, int* total){
    fseek(arq, *total * tamanho_registro(), SEEK_SET);
    TFunc* func = le_funcionario(arq);
    (*total)++;
    return func;
}

void mudarNomesDasParticoes(Lista* nomes, int* numero){
    if(nomes->prox == NULL){
        char *novoNome = malloc(5 * sizeof(char));
        (*numero)++;
        sprintf(novoNome, "p%d.dat", *numero);
        nomes->prox = cria(novoNome, NULL);
    }
}

int podeLerMais(int lidos, int nFunc){
    return lidos < nFunc;
}

FILE *abrirParticao(char* nome){
    FILE* particao = fopen(nome, "w+");
    return particao;
}

int procuraMenor(TFunc* funcionarios[], int tam){
    int menor = 0;
    for(int i = 1; i < tam; i++){
        if(funcionarios[menor]->cod > funcionarios[i]->cod)
            menor = i;
    }
    return menor;
}