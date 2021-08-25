
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
    Lista* nomes = nome_arquivos_saida;
    TFunc** vet = malloc(M * sizeof(TFunc*));
    int inVet = 0;

    FILE* repo = fopen("repository.dat", "wb+");
    int inRepo = 0;

    Lista* nomesAtuais = nomes;
    FILE* particaoAtual = NULL;
    int inParticaoAtual = 0;

    while(pode_ler_mais(qtdLidos, nFunc)){
        if(inVet < M){
            vet[inVet] = pegar_funcionario(arq, &qtdLidos);
            inVet++;
        }

        if(inVet < M)
            continue;

        int menor = procura_menor(vet, inVet);
        if(particaoAtual == NULL){
            char* nome = nomes->nome;
            particaoAtual = abrir_particao(nome);
            nomesAtuais = nomes;

            mudar_nomes_das_particoes(nomes, n);
            nomes = nomes->prox;
        }

        if(inParticaoAtual >= M){
            fclose(particaoAtual);
            particaoAtual = NULL;
            inParticaoAtual = 0;
            continue;
        }
        fseek(particaoAtual, inParticaoAtual * tamanho_registro(), SEEK_SET);
        salva_funcionario(vet[menor], particaoAtual);
        nomesAtuais->tamanho++;
        inParticaoAtual++;

        if(!pode_ler_mais(qtdLidos, nFunc)){
            for(int i = menor; i < inVet - 1; i++){
                vet[i] = vet[i + 1];
            }
            inVet--;
            break;
        }

        int code = vet[menor]->cod;
        free(vet[menor]);
        vet[menor] = pegar_funcionario(arq, &qtdLidos);

        if(vet[menor]->cod < code){
            fseek(repo, inRepo * tamanho_registro(), SEEK_SET);
            salva_funcionario(vet[menor], repo);
            inRepo++;
            if(!pode_ler_mais(qtdLidos, nFunc)){
                for(int i = menor; i < inVet; i++){
                    vet[i] = vet[i + 1];
                }
                inVet--;
                break;
            }
            vet[menor] = pegar_funcionario(arq, &qtdLidos);
        }

        if(inRepo < n){
            continue;
        }
        fclose(particaoAtual);

        char* nome = nomes->nome;
        particaoAtual = abrir_particao(nome);
        nomesAtuais = nomes;
        mudar_nomes_das_particoes(nomes, n);
        nomes = nomes->prox;

        int* ultimoSalvo = NULL;
        int i = 0;
        while (inVet > 0){
            fseek(particaoAtual, i = tamanho_registro(), SEEK_SET);
            int menor2 = procuraMenor(vet, inVet);
            salva_funcionario(vet[menor2], particaoAtual);
            nomesAtuais->tamanho++;
            for(int i = menor2; i < inVet - 1; i++){
                vet[i] = vet[i + 1];
            }
            inVet--;
            i++;
        }

        for(int i = 0; i < inVet; i++){
            free(vet[i]);
        }

        inVet = 0;
        for(int i = 0; i < inRepo, i++){
            fseek(repo, i * tamanho_registro(), SEEK_SET);
            vet[i] = le_funcionario(repo);
            inVet++;
        }
        inRepo = 0;

        fclose(particaoAtual);
        particaoAtual = NULL;
        inParticaoAtual = 0;
    }

    if(particaoAtual){
        fclose(particaoAtual);
        particaoAtual = NULL;
        inParticaoAtual = 0;
    }
    int inUltimaParticao = 0;
    if(!(inVet <= 0 && inRepo <= 0)){
        if(inRepo > 0){
            char *nome = nomes->nome;
            particaoAtual = abrir_particao(nome);
            nomesAtuais = nomes;
            inParticaoAtual = 0;
            for(int i = 0; i < inRepo, i++){
                fseek(repo, i * tamanho_registro(), SEEK_SET);
                TFunc* funcionario = le_funcionario(repo);

                fseek(particaoAtual, inParticaoAtual * tamanho_registro(), SEEK_SET);
                salva_funcionario(funcionario, particaoAtual);
                nomesAtuais->tamanho++;

                inParticaoAtual++;
            }
            fclose(particaoAtual);
            inParticaoAtual = 0;
        }
        if(inVet > 0){
            mudar_nomes_das_particoes(nomes, n);
            nomes = nomes->prox;
            char* nome = nomes->nome;
            particaoAtual = abrir_particao(nome);
            nomesAtuais = nomes;
            inParticaoAtual = 0;
            for(int i = 0; i < inVet; i++){
                fseek(particaoAtual, i * tamanho_registro(), SEEK_SET);
                salva_funcionario(vet[i], particaoAtual);
                nomesAtuais->tamanho++;
                inParticaoAtual++;
            }
            fclose(particaoAtual);
        }
    }
    fclose(repo);
    free(vet);
}

TFunc* pegar_funcionario(FILE* arq, int* total){
    fseek(arq, *total * tamanho_registro(), SEEK_SET);
    TFunc* func = le_funcionario(arq);
    (*total)++;
    return func;
}

void mudar_nomes_das_particoes(Lista* nomes, int* numero){
    if(nomes->prox == NULL){
        char *novoNome = malloc(5 * sizeof(char));
        (*numero)++;
        sprintf(novoNome, "p%d.dat", *numero);
        nomes->prox = cria(novoNome, NULL);
    }
}

int pode_ler_mais(int lidos, int nFunc){
    return lidos < nFunc;
}

FILE* abrir_particao(char* nome){
    FILE* particao = fopen(nome, "w+");
    return particao;
}

int procuta_menor(TFunc* funcionarios[], int tam){
    int menor = 0;
    for(int i = 1; i < tam; i++){
        if(funcionarios[menor]->cod > funcionarios[i]->cod)
            menor = i;
    }
    return menor;
}