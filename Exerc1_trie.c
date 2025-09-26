
//Executar: ./trie lexico.txt
//pelo que estudei, diferente da AVL/RB, a trie é mais rapida para buscas de strings por 
//usar caracteres como chave (e nao palavras inteiras é em avl/rb)

//preciso de:
//1. carregar o arquivo do link com palavras e polaridades
//2. construir a trie na memória
//3. permitir buscar e editar polaridade de palavras
//4. salvar em outro arquivo
//complexidade sera O(N)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_LINHA 512 // podia ser 1024 tbm ou outro

//precisa de struct para estrutura trie e seus filhos e cada nó pode ter ate 256 filhos
typedef struct NoTrie{
    
    struct NoTrie *filhos[256]; // ponteiro para os filhos
    int fimPalavra; //saber se chegou ao fim da palavra
    char *polaridade; //polaridade associada a palavra
} NoTrie;


//basico de aed: criar novo nó

NoTrie* criarNovoNoTrie(){

    NoTrie *novoNo = (NoTrie*) malloc(sizeof(NoTrie));
    if(novoNo == NULL){
        printf("Erro de memória\n");
        exit(1);
    }
    for(int i=0; i<256; i++){
        novoNo->filhos[i] = NULL;
    }
    novoNo->fimPalavra = 0;
    novoNo->polaridade = NULL;
    return novoNo;
}

//inserir palavra na trie com polaridade e se tiver a palavra, atualiza a polaridade
// de inicio faia sem unsigned, porem, o OpLexicon é em portugues e tem acentos,
// entao, para evitar problemas com caracteres negativos,como ç, é melhor usar unsigned char

void inserirPalavraNaTrie(NoTrie *raiz, unsigned char *palavra, char *polaridade){
    NoTrie *noAtual = raiz;
    int TamanhoPalavra = strlen((char*)palavra);

    for(int i=0; palavra[i] < TamanhoPalavra ; i++){
        unsigned char caractere = palavra[i];
        if(noAtual->filhos[caractere] == NULL){
            noAtual->filhos[caractere] = criarNovoNoTrie();
        }
        noAtual = noAtual->filhos[caractere];
    }

    //marca o fim da palavra
    noAtual->fimPalavra = 1;

    //atualiza ou insere a polaridade
    if(noAtual->polaridade != NULL){
        free(noAtual->polaridade);
    }
    noAtual->polaridade = (char*) malloc(strlen(polaridade) + 1);
    strcpy(noAtual->polaridade, polaridade);
}

//agora precisa buscar a polaridade se encontrada ou NULL se nao existir
char* buscarPolaridadeNaTrie(NoTrie *raiz, unsigned char *palavra){

    NoTrie *noAtual = raiz;
    int TamanhoPalavra = strlen((char*)palavra);

    for(int i=0; palavra[i] < TamanhoPalavra ; i++){
        unsigned char caractere = palavra[i]; // compilador do vscode deu sugestão de usar unsigned char em todos os locais assim
        if(noAtual->filhos[caractere] == NULL){
            return NULL; //nao existe
        }
        noAtual = noAtual->filhos[caractere];
    }

    if(noAtual->fimPalavra){

        return noAtual->polaridade;
    } else{

        return NULL;
    }
}

//como é arvore, quis fazer funcao recursiva para liberar memoria
//ideia de aed1
void liberarMemoriaTrie(NoTrie *noAtual){

    if(noAtual == NULL) return;

    for(int i = 0; i < 256; i++){

        if(noAtual->filhos[i] != NULL){

            liberarMemoriaTrie(noAtual->filhos[i]);
        }
    }
    if(noAtual->polaridade != NULL){
        free(noAtual->polaridade);
    }
    free(noAtual);
}
//salvar: percorre arvore, mas sem usar arquivos em si, dava dando erro, ate por que ainda precisa ver
//desculpa nome ´pessimo 
//os espaços entre palavras
void salvar (NoTrie *noAtual, unsigned char *PalavraTemporaria, int posicaoAtual, FILE *arquivoSaida) {
    if(noAtual == NULL) return;

    if(noAtual->fimPalavra) {
        PalavraTemporaria[posicaoAtual] = '\0'; // finaliza a string
        fprintf(arquivoSaida, "%s\t%s\n",  PalavraTemporaria, noAtual->polaridade);
    }
}


//salva todas as palavras e polaridades da trie em um arquivo
//percorre a trie recursivamente acumulando os caracteres
//pq recursivo? por ser arvore e usei dessa dica em todos os exercicios de aed1 sobre arvore: leetcode

void salvarTrieEmArquivoRecursivo(NoTrie *noAtual, FILE *arquivoSaida, unsigned char *PalavraTemporaria, int posicaoAtual){
    if(noAtual == NULL) return;

    if(noAtual -> fimPalavra){
      PalavraTemporaria[posicaoAtual] = '\0'; //finaliza a string e uso de buffer/PalavraTemporaria sempre que preciso de vetor temporario
        fprintf(arquivoSaida, "%s\t%s\n", PalavraTemporaria, noAtual->polaridade);
    }

    for(int i = 0; i < 256; i++){
        if(noAtual->filhos[i] != NULL){
            PalavraTemporaria[posicaoAtual] = (unsigned char)i;
            salvarTrieEmArquivoRecursivo(noAtual->filhos[i], arquivoSaida,PalavraTemporaria, posicaoAtual + 1); 

// posicaoAtual +1 foi ideia de aed1 tambem, como posicaoAtual diz onde estamos na palavra, para andar/ descer filho, precisamos do +1
        }
    }
}
//carregar cada linha e inserir na trie
int carregarLexico(NoTrie *raiz, char *nomeArquivo){
    FILE *arquivoEntrada = fopen(nomeArquivo, "r");
    if(arquivoEntrada == NULL){
        printf("Erro ao abrir o arquivo %s", nomeArquivo);
        return 0;
    }

    char linha[TAMANHO_LINHA];
    char palavra[TAMANHO_LINHA], polaridade[TAMANHO_LINHA];
    int contadorInseridos = 0;

    while(fgets(linha, TAMANHO_LINHA, arquivoEntrada)){
        char *token;

        //quebra a linha em palavra e polaridade, usando strtok
        token = strtok(linha, "\t; ,");
        if(token == NULL) continue;
        strcpy(palavra, token);

        token = strtok(NULL, "\t; ,");
        if(token == NULL) continue;
        strcpy(polaridade, token);

        inserirPalavraNaTrie(raiz, (unsigned char*)palavra, polaridade);
        contadorInseridos++;
    }

    fclose(arquivoEntrada);
    return contadorInseridos;
}

int main(){

    NoTrie *raiz = criarNovoNoTrie();
    char nomeArquivoEntrada[100];

    //arquivo pode vir como argumento ou ser pedido ao usuário
    printf("Digite o caminho do arquivo de léxico: ");
    scanf("%s", nomeArquivoEntrada);

    int totalPalavras = carregarLexico(raiz, nomeArquivoEntrada);
    printf("Arquivo carregado. %d palavras inseridas.\n", totalPalavras);

    char comando[20];
    char palavra[TAMANHO_LINHA];
    char novaPolaridade[TAMANHO_LINHA];

    while(1){
        printf("\nComandos: buscar, editar, salvar, sair\n> ");
        scanf("%s", comando);

        if(strcmp(comando, "sair") == 0){
            break;
        }
        else if(strcmp(comando, "buscar") == 0){
            printf("Digite a palavra: ");
            scanf("%s", palavra);
            char *polaridadeEncontrada = buscarPolaridadeNaTrie(raiz, (unsigned char*)palavra);
            if(polaridadeEncontrada != NULL)
                printf("Polaridade: %s\n", polaridadeEncontrada);
            else
                printf("Palavra não encontrada.\n");
        }
        else if(strcmp(comando, "editar") == 0){
            printf("Digite a palavra: ");
            scanf("%s", palavra);
            printf("Digite a nova polaridade: ");
            scanf("%s", novaPolaridade);
            inserirPalavraNaTrie(raiz, (unsigned char*)palavra, novaPolaridade);
            printf("Polaridade foi atualizada.\n");
        }
        else if(strcmp(comando, "salvar") == 0){
            char nomeArquivoSaida[100];
            printf("Digite o nome do arquivo para salvar: ");
            scanf("%s", nomeArquivoSaida);
            FILE *arquivoSaida = fopen(nomeArquivoSaida, "w");
            if(arquivoSaida == NULL){
                printf("Erro ao abrir, para salvar.\n");
                continue;
            }
            unsigned char PalavraTemporaria[TAMANHO_LINHA];
            salvarTrieEmArquivoRecursivo(raiz, arquivoSaida,  PalavraTemporaria, 0);
            fclose(arquivoSaida);
            printf("Arquivo salvo em %s\n", nomeArquivoSaida);
        }
        else{
            printf("invalido");

}
    }

}
