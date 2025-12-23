/*PRECISO DE:
ler arquivos em byte ou caracter por caracter
contar frequências
construir árvore de huffman
gerar códigos binários, se usei bytes
compactar , salvar, descompactar usando a árvore/tabela
comparar .txt final com o original
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <stdint.h>
//#include <stdbool.h>





int main() {
    
    compactar("palavras.txt", "compactado.huff");
    descompactar("compactado.huff", "saida.txt");
    return 0;
}

