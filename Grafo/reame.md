Enunciado do Exercício

Desenvolva um programa em C que realize as seguintes tarefas:

    Leitura de Arquivo TSPLIB:

        O programa deve ser capaz de ler um arquivo no formato TSPLIB que descreve um problema de Caixeiro Viajante (TSP) onde as cidades são os vértices e as distâncias/rotas são as arestas.

        Dica: Foque na seção NODE_COORD_SECTION para obter as coordenadas das cidades (vértices) e, para simplificar, considere o grafo como completo (existe uma aresta entre todas as cidades) e não-direcionado. A distância euclidiana entre as cidades pode ser usada como peso da aresta.

    Implementação de Estruturas de Grafos:

        Crie duas estruturas de dados separadas para representar o mesmo grafo lido do arquivo:

            Usando Matriz de Adjacências.

            Usando Lista de Adjacências.

    Análise de Custo e Complexidade:

        Implemente as seguintes operações básicas em ambas as estruturas (lista e matriz):

            Inserção: Adicionar um novo vértice (cidade) e as arestas necessárias.

            Remoção: Remover um vértice e todas as arestas associadas.

            Busca/Acesso: Verificar se existe uma aresta entre dois vértices específicos (ex: cidade X e cidade Y).

            Edição: Alterar o peso de uma aresta existente (ex: mudar a distância entre duas cidades).

        Para cada uma dessas quatro operações e para as duas implementações (Matriz e Lista), determine a Complexidade de Tempo (pior caso, em notação O) e a Complexidade de Espaço (memória).

    Discussão e Comparação (Relatório):

        O programa deve incluir um print ou uma seção de saída que resuma os resultados da análise de complexidade.

        Prepare uma análise comparativa (em forma de um breve relatório/comentário no código) sobre as duas implementações, discutindo em que cenários (grafos densos vs. esparsos) cada uma é mais vantajosa em termos de tempo de execução e uso de memória.

Os arquivos  fornecidos no site  https://www.math.uwaterloo.ca/tsp/world/countries.html contém vários exemplos de arquivos no formato TSPLIB (.tsp). Para o nosso exercício, sugiro escolher um arquivo pequeno a médio (ex: a280.tsp ou menor) para facilitar a depuração.
