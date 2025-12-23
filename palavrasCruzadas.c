#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PALAVRAS 10
#define MAX_TAMPALAVRA 20
//#define MAX_TAM_TABULEIRO 50

// para preencher o tabuleiro com letras aleatórias ja que é palavras  cruzadas
char letraAleatoria() {
    return 'A' + rand() % 26;
}

// preencher toda a matriz (tabuleiro usa forma de quadrado, que seria um n x m)
// com essas letras aleatórias
//   numero: tamanho do tabuleiro (numero x numero)
void preencherTabuleiro(char **tabuleiro, int numero) {
    for (int i= 0 ; i < numero; i++) {
        for (int j= 0; j < numero; j++) {
            tabuleiro[i[j] = letraAleatoria();
        }
    }
}
void imprimirTabuleiro(char **tabuleiro , int numero){
    for(int j= 0; j< numero; j++)
        printf("%d ", j); // Imprime numeros de colunas

    for (int i = 0; i< numero; i++) {       // Percorre linhas
        printf("%d ", i);                   // Imprime número da linha
        for (int j = 0; j< numero; j++) {   // Percorre colunas da linha
            printf(" %c ", tabuleiro[i][j]); // Imprime letra da posição (linha i, coluna j), deu erro sem isso
        }
    }
}

// ler palavras do arquivo palavras.txt, ja que foi pedido isto
int lerPalavras(char palavras[MAX_PALAVRAS][MAX_TAMPALAVRA]) {
    FILE *arquivo = fopen("palavras.txt", "r");
    if (!arquivo)) {
        printf("Erro ao abrir arquivo");
        return 0;
    }

    int contador = 0;                              // Contador de palavras lidas
    while (fscanf(arquivo, "%s", palavras[contador]) == 1 && contador < MAX_PALAVRAS)
        contador++;                                // Lê palavra por palavra até o limite MAX_PALAVRAS

    fclose(arquivo);
    return contador;
}

// checar se a palavra cabe na matriz na posição e direção escolhidas, se é valida
//   numero é tamanho da matriz
//   palavra seria a palavra que queremos colocar
//   linha, coluna é  posição inicial
//   dLinha, dColuna: direção da palavra (8 direções possíveis) para outra funçao
// Retorna: 1 se cabe, 0 se não
int cabePalavra(char **tabuleiro, int numero, char *palavra, int linha, int coluna, int dLinha, int dColuna) {
    int len = strlen(palavra);
    int x = linha, y = coluna;                 // Começa na posição inicial

    for (int i = 0; i < len; i++) {
        if (x < 0 || x >= numero || y < 0 || y >= numero)
            return 0;                          //  nao cabe
        x += dLinha;                            // Avança na direção da linha
        y += dColuna;                           // Avança na direção da coluna
    }
    return 1;                                  // Palavra cabe
}

// tem que colocar palavra aleatoriamente no tabuleiro e precisa das posiçoes escolhidas com base na direçao
// fiz algum leetcode em aed1 com toto usando vetores de direçao e entao so aproveitei a ideia mesmo
void colocarPalavra(char **tabuleiro, int numero, char *palavra) {
    int dLinha[] = {0, 1, 1, -1, 0, -1, 1, -1};   // linha 0-7
    int dColuna[] = {1, 0, 1, 1, -1, 0, -1, -1};  //  coluna 0-7
    int len = strlen(palavra);
    int colocada = 0;                              // se coloca ou não

    while (!colocada) {                            // Tenta até conseguir colocar e tudo aleatorio
        int dir = rand() % 8;
        int linha = rand() % numero;
        int coluna = rand() % numero;

        //ve se cabe na posição e direção escolhida
        if (cabePalavra(tabuleiro, numero, palavra, linha, coluna, dLinha[dir], dColuna[dir])) {
            int x = linha, y = coluna;             // Posição inicial
            for (int i = 0; i < len; i++) {
                 // Se a posição estiver vazia ou já tiver a letra correta, insere a letra
                tabuleiro[x][y] = palavra[i];     // Insere letra
                x += dLinha[dir];                  // Avança linha
                y += dColuna[dir];                 // Avança coluna
            }
            colocada = 1;                          // foi inserida
        }
    }
}

// usuário buscar palavras no caça-palavras e retorna 1 se encontrada, 0 se não
int buscarPalavra(char **tabuleiro, int numero, char *palavraToda, int *linhaEncontrada, int *colunaEncontrada, int *dirEncontrada) {
    int dLinha[] = {0, 1, 1, -1, 0, -1, 1, -1};    // Vetores de direção linha
    int dColuna[] = {1, 0, 1, 1, -1, 0, -1, -1};   // Vetores de direção coluna
    int len = strlen(palavraToda);

    for (int i = 0; i < numero; i++) {            // Percorre linhas
        for (int j = 0; j < numero; j++) {        // Percorre colunas
            for (int d = 0; d < 8; d++) {         // Percorre todas as direções de 0 a 7
                 // Verifica se a palavra cabe na posição (i, j) e tive que atribuir pos dava erro usando apenas x,y
                int x = i, y = j;
                int k = 0; //precisei de outra variavel para verificar cada letra da palavra cruzada
                while (k < len) {

                    if (tabuleiro[x][y] != palavraToda[k]) // Letra diferente
                        break;
                    x += dLinha[d];               // Avança linha
                    y += dColuna[d];              // Avança coluna
                    k++;
                }
                if (k == len) {                     // Palavra inteira encontrada
                    *linhaEncontrada = i;
                    *colunaEncontrada = j;
                    *dirEncontrada = d;
                    return 1;
                }
            }
        }
    }
    return 0;                                      // nao achou
}

//  marcar palavra encontrada com *
void destacarPalavra(char **tabuleiro, char *palavraToda, int linha, int coluna, int dir, int numero) {
    int dLinha[] = {0, 1, 1, -1, 0, -1, 1, -1};
    int dColuna[] = {1, 0, 1, 1, -1, 0, -1, -1};
    int len = strlen(palavraToda);
    int x = linha, y = coluna;
    for (int i = 0; i < len; i++) {
        tabuleiro[x][y] = '*';
        x += dLinha[dir];
        y += dColuna[dir];
    }
}

int main() {
    srand(time(NULL));                             // Inicializa random

    int numero;
    printf("Diga qual sera tamanho do tabuleiro como - N x N: ");
    scanf("%d", &numero);                          // Lê tamanho total do tabuleiro

    // Aloca matriz
    char **tabuleiro = malloc(numero * sizeof(char*));
    for (int i = 0; i < numero; i++)
        tabuleiro[i] = malloc(numero * sizeof(char));

    preencherTabuleiro(tabuleiro, numero);         //funçaoo para preencher com letras aleatórias

    char palavras[MAX_PALAVRAS][MAX_TAMPALAVRA];
    int numPalavras = lerPalavras(palavras);       // Lê do arquivo dado

    for (int i = 0; i < numPalavras; i++)
        colocarPalavra(tabuleiro, numero, palavras[i]); // Coloca cada palavra no tabuleiro

    printf("\nTabuleiro inicial:\n");
    imprimirTabuleiro(tabuleiro, numero);          // tabuleiro inicial

    char palavraBusca[MAX_TAM_PALAVRA];
    while (1) {
        printf(" palavra para buscar (ou digite 0 para sair): ");
        scanf("%s", palavraBusca);
        if (strcmp(palavraBusca, "0") == 0)
            break;                                 // Sai do jogo senao fica em loop

        int linha, coluna, direcao;
        if (buscarPalavra(tabuleiro, numero, palavraBusca, &linha, &coluna, &direcao)) {
            printf("Palavra encontrada em (%d, %d) e na direcao %d\n", linha, coluna, direcao);
            destacarPalavra(tabuleiro, palavraBusca, linha, coluna, direcao, numero); // Marca palavra com *
        } else {
            printf("nao encontrou a palavra.\n");
        }
        imprimirTabuleiro(tabuleiro, numero);      //atualiza tabuleiro
    }

    // free alocaçoes

    for (int i = 0; i < numero; i++) {
        free(tabuleiro[i]);
    free(tabuleiro);

    return 0;
}
}

