//LARISSA GABRIELA - M2
//escolhi dj38.tsp para testar
// não tive tempo de estressar o algoritmo com a china e japão, mas no relatorio fiz analise teorica de consumo de memoria

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_VERTICES 2500
#define MAX_COMPRIMENTO 256 //para linha de arquivo

//cidade
typedef struct {
    int id;
    double x,y;
} Cidade;

//grafo e matriz adjacencia
typedef struct {
    int numVertices;
    double **matrizAdj;
} GrafoMatriz;

//lista de nós de ajdacencia
typedef struct ListaNode{
    int destino;
    double peso;
    struct ListaNode* next; //ponteiro proximo
} ListaNode;

//grafo e LISTA
typedef struct {
    int numVertices;
    ListaNode** listaArray;
} GrafoLista;

//PROTOTIPOS
GrafoMatriz* criarGrafoMatriz(int n);
void inserirVerticeMatriz(GrafoMatriz* grafo);
void removerVerticeMatriz(GrafoMatriz* grafo, int vertice);
double calcularDistancia(Cidade a, Cidade b);
int existeArestaMatriz(GrafoMatriz* grafo, int origem, int dest);
void editarArestaMatriz(GrafoMatriz* grafo, int origem, int
    dest, double novoPeso);
GrafoLista* criarGrafoLista(int n);
void inserirVerticeLista(GrafoLista* grafo);
void removerVerticeLista(GrafoLista* grafo, int vertice);
int existeArestaLista(GrafoLista* grafo, int origem, int dest);
void editarArestaLista(GrafoLista* grafo, int origem, int dest, double novoPeso);
void inserirArestaLista(GrafoLista* grafo, int origem, int dest,
    double peso);
void popularGrafos(GrafoMatriz* gm, GrafoLista* gl, Cidade*
    cidades, int n);
void gerarRelatorio();
void lerArquivoTSP(const char* nomeArquivo, Cidade** cidades,
    int* numCidades);


//funçoes da matriz
GrafoMatriz* criarGrafoMatriz(int n){
    GrafoMatriz* grafo = (GrafoMatriz*)malloc(sizeof(GrafoMatriz));
    grafo->numVertices = n;
    grafo->matrizAdj = (double**)malloc(n * sizeof(double*));
    for(int i = 0; i < n; i++){
        grafo->matrizAdj[i] = (double*)calloc(n, sizeof(double));
    }
    return grafo;
}

void inserirVerticeMatriz(GrafoMatriz* grafo){
    int novoN = grafo->numVertices + 1;
    grafo->matrizAdj = (double**)realloc(grafo->matrizAdj, novoN * sizeof(double*));
    grafo->matrizAdj[novoN-1] = (double*)calloc(novoN, sizeof(double));
    
    for(int i = 0; i < novoN-1; i++){
        grafo->matrizAdj[i] = (double*)realloc(grafo->matrizAdj[i], novoN * sizeof(double));
        grafo->matrizAdj[i][novoN-1] = 0; //inicializa nova coluna
    }
    grafo->numVertices = novoN;
}

void removerVerticeMatriz(GrafoMatriz* grafo, int vertice){
    int n = grafo->numVertices;
    if(vertice < 0 || vertice >= n) return;

    free(grafo->matrizAdj[vertice]);

    // Move linhas e colunas
    for(int i = vertice; i < n-1; i++){
       for (int j = 0; j < n; j++){
            grafo->matrizAdj[i][j] = grafo->matrizAdj[i+1][j];
        }
        grafo->matrizAdj[i] = realloc(grafo->matrizAdj[i], n * sizeof(double));
    }

    for(int i = 0; i < n-1; i++){
        for(int j = vertice; j < n-1; j++){
            grafo->matrizAdj[i][j] = grafo->matrizAdj[i][j+1];
        }
        grafo->matrizAdj[i] = (double*)realloc(grafo->matrizAdj[i], (n-1) * sizeof(double));
    }

    free(grafo->matrizAdj[n-1]);

    grafo->matrizAdj = (double**)realloc(grafo->matrizAdj, (n-1) * sizeof(double*));
    grafo->numVertices = n-1;
}

int existeArestaMatriz(GrafoMatriz* grafo, int origem, int dest){
    return grafo->matrizAdj[origem][dest] != 0;
}

void editarArestaMatriz(GrafoMatriz* grafo, int origem, int dest, double novoPeso){
    grafo->matrizAdj[origem][dest] = novoPeso;
    grafo->matrizAdj[dest][origem] = novoPeso; //grafo não direcionado
}


//funçoes da lista
GrafoLista* criarGrafoLista(int n) {
    GrafoLista* grafo = (GrafoLista*)malloc(sizeof(GrafoLista));
    grafo->numVertices = n;
    grafo->listaArray = (ListaNode**)malloc(n * sizeof(ListaNode*));
    for(int i = 0; i < n; i++) {
        grafo->listaArray[i] = NULL;
    }
    return grafo;
}
void inserirVerticeLista(GrafoLista* grafo) {
    int novoN = grafo->numVertices + 1;
    grafo->listaArray = (ListaNode**)realloc(grafo->listaArray, novoN * sizeof(ListaNode*));
    grafo->listaArray[novoN-1] = NULL; // Inicializa nova lista
    grafo->numVertices = novoN;
}

void removerVerticeLista(GrafoLista* grafo, int vertice) {
    // Remove arestas de outros vértices
    for(int i = 0; i < grafo->numVertices; i++) {
        if(i == vertice) continue;
        
        ListaNode* prev = NULL;
        ListaNode* atual = grafo->listaArray[i];
        while(atual != NULL) {
            if(atual->destino == vertice) {
                if(prev == NULL) {
                    grafo->listaArray[i] = atual->next;
                } else {
                    prev->next = atual->next;
                }
                free(atual);
                break;
            }
            prev = atual;
            atual = atual->next;
        }
    }
    
    // Libera a lista do vértice
    ListaNode* atual = grafo->listaArray[vertice];
    while(atual != NULL) {
        ListaNode* temp = atual;
        atual = atual->next;
        free(temp);
    }
    
    // Move os vértices
    for(int i = vertice; i < grafo->numVertices-1; i++) {
        grafo->listaArray[i] = grafo->listaArray[i+1];
    }
    grafo->listaArray = (ListaNode**)realloc(grafo->listaArray, (grafo->numVertices-1) * sizeof(ListaNode*));
    grafo->numVertices--;
}
int existeArestaLista(GrafoLista* grafo, int origem, int dest) {
    ListaNode* atual = grafo->listaArray[origem];
    while(atual != NULL) {
        if(atual->destino == dest) return 1;
        atual = atual->next;
    }
    return 0;
}
void editarArestaLista(GrafoLista* grafo, int origem, int dest, double novoPeso) {
    ListaNode* atual = grafo->listaArray[origem];
    while(atual != NULL) {
        if(atual->destino == dest) {
            atual->peso = novoPeso;
            break;
        }
        atual = atual->next;
    }
    
    atual = grafo->listaArray[dest];
    while(atual != NULL) {
        if(atual->destino == origem) {
            atual->peso = novoPeso;
            break;
        }
        atual = atual->next;
    }
}

// Função para inserir aresta na lista
void inserirArestaLista(GrafoLista* grafo, int origem, int dest, double peso) {
    ListaNode* novoNo = (ListaNode*)malloc(sizeof(ListaNode));
    novoNo->destino = dest;
    novoNo->peso = peso;
    novoNo->next = grafo->listaArray[origem];
    grafo->listaArray[origem] = novoNo;

    // Como o grafo é não direcionado, insere a aresta no sentido oposto também
    novoNo = (ListaNode*)malloc(sizeof(ListaNode));
    novoNo->destino = origem;
    novoNo->peso = peso;
    novoNo->next = grafo->listaArray[dest];
    grafo->listaArray[dest] = novoNo;
}

// Função para calcular distância euclidiana
double calcularDistancia(Cidade a, Cidade b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
// Função para popular grafos: descobri que existe isso agora

void popularGrafos(GrafoMatriz* gm, GrafoLista* gl, Cidade* cidades, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = i+1; j < n; j++) {
            double distancia = calcularDistancia(cidades[i], cidades[j]);
            gm->matrizAdj[i][j] = distancia;
            gm->matrizAdj[j][i] = distancia; //grafo não direcionado
            inserirArestaLista(gl, i, j, distancia);
        }
    }
}
// Função para gerar relatório de complexidade
//matri n x n , onde n é numero de vertice
//lista n + e, onde e é numero de arestas
//cada vertice tem uma lista encadeada de arestas
//na lista de adjacencia, a busca e ediçao de aresta depende do grau do vertice
// e mantem uma lista de nós que representam as arestas saidno dele
// logo, grau (v) é numero de arestas saindo do vertice v

void gerarRelatorio() {
    printf("RELATORIO DE COMPLEXIDADES:\n\n");
    printf("Busca de aresta: O(1)\n"); //acesso direto
    printf("Edicao de aresta: O(1)\n"); //acesso direto
    printf("Matriz de Adjacencias:\n");
   
    printf("Espaco: O(n^2)\n\n"); //precisa de memoria para tosas as celulas da matriz

    printf("Lista de Adjacencias:\n");
    printf("Busca de aresta: O(grau(v))\n"); //precisa percorrer a lista do vertice v
    printf("Edicao de aresta: O(grau(v))\n"); //precisa percorrer a lista do vertice v
    printf("Insercao de vertice: O(1)\n"); //so cria lista vazia
    printf("Remocao de vertice: O(n + grau(v))\n");   //percorre todas as lstas, de todos os vertices O(n) e libera a lista do vertice v O(grau(v)) para remover aquele vertice e limapr a lista
    printf("Espaco: O(n + e)\n"); // n listas + e nós de arestas

    printf("Matriz vs Lista:\n");
    printf("Matriz de adjacencias e mais eficiente para busca e edicao de arestas, porem consome mais espaco. E boa para grafos densos e de acesso rapido\n");
    printf("Lista de adjacencias e mais eficiente em termos de economia de memoria/espaco, boa para grafos esparsos, mas busca e edicao de arestas podem ser mais lentas dependendo do grau do vertice.\n");
}

// Função para ler cidades de arquivo
void lerArquivoTSP(const char* nomeArquivo, Cidade** cidades, int* numCidades) {
    FILE* file = fopen(nomeArquivo, "r");
    if(!file) {
        printf("Erro ao abrir arquivo!\n");
        exit(1);
    }

    char line[MAX_COMPRIMENTO];
    *numCidades = 0;
    int relendoCoordenadas = 0;

    // Primeira passada para contar cidades
    //usando mesmo nome do .tsp

    while(fgets(line, sizeof(line), file)) {
        if(strstr(line, "DIMENSION")) {
            sscanf(line, "DIMENSION : %d", numCidades);
        //sscanf(line, "%*s %*c %d", numCidades); // ignora DIMENSION 

        }
//poderia usar strtok tambem, para ler qualquer formato

        if(strstr(line, "NODE_COORD_SECTION")) {
            relendoCoordenadas = 1;
            break;
        }
    }

    if(*numCidades == 0) {
        printf("Dimensao nao encontrada!\n");
        exit(1);
    }
    *cidades = (Cidade*)malloc(*numCidades * sizeof(Cidade));
    int index = 0;

    // Segunda passada para ler coordenadas
    if(relendoCoordenadas) {
        while(fgets(line, sizeof(line), file) && strstr(line, "EOF") == NULL) {
            sscanf(line, "%d %lf %lf", &(*cidades)[index].id, &(*cidades)[index].x, &(*cidades)[index].y);
            index++;
        }
    }
    fclose(file);
}


int main(){

    Cidade* cidades;
    int numCidades;

    lerArquivoTSP("C:/Users/lg_11/Downloads/aed2_grafo/dj38.tsp", &cidades, &numCidades);
    
    lerArquivoTSP("C:/Users/lg_11/Downloads/aed2_grafo/ch71009.tsp", &cidades, &numCidades);
    
    lerArquivoTSP("C:/Users/lg_11/Downloads/aed2_grafo/ja9847.tsp", &cidades, &numCidades);

    //lerArquivoTSP("dj38.tsp", &cidades, &numCidades);

    GrafoMatriz* gm = criarGrafoMatriz(numCidades);
    GrafoLista* gl = criarGrafoLista(numCidades);

    popularGrafos(gm, gl, cidades, numCidades);

    // Testar as operações
    printf("Operacoes feitas:.\n");

    // Exemplo de uso 
    inserirVerticeMatriz(gm);
    inserirVerticeLista(gl);

     editarArestaMatriz(gm, 0, 1, 5.0); //grafo matriz
     editarArestaLista(gl, 0, 1, 5.0);   // grafo da lista = deu conflito de dados em usar grafolista e grafomatriz

    removerVerticeMatriz(gm, 0);
    removerVerticeLista(gl, 0);

    printf("Existe aresta entre 0 e 1? Matriz: %d, Lista: %d\n",
           existeArestaMatriz(gm, 0, 1),
           existeArestaLista(gl, 0, 1));

   

    gerarRelatorio();

    free(cidades);
    // Liberar memória dos grafos
    for(int i = 0; i < gm->numVertices; i++) free(gm->matrizAdj[i]);
    free(gm->matrizAdj); 
    free(gm);
//libera grafo por encadeada, de cada vertice
    for(int i = 0; i < gl->numVertices; i++) {
        ListaNode* atual = gl->listaArray[i];
        while(atual != NULL) {
            ListaNode* temp = atual;
            atual = atual->next;
            free(temp);
        }
    }
    free(gl->listaArray);
    free(gl);

    return 0;
}