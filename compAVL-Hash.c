//Guilherme de Souza Cerdeira			RA: 2474085
//Arthur Romão Barreto 					RA: 2474050

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura para o nodo da árvore AVL
typedef struct No {
    int chave;
    int altDireita;
    int altEsquerda;
    struct No* direita;
    struct No* esquerda;
} Nodo;

// Estrutura para a tabela hash
typedef struct {
    int* tabela;
    int m;
    int tamanho;
    int capacidade;
} TabelaHash;

// Estrutura para o nodo da lista encadeada
typedef struct NodoLista {
    int chave;
    struct NodoLista* proximo;
} NodoLista;

// Estrutura para a tabela hash com lista encadeada
typedef struct {
    NodoLista** tabela;
    int m;
    int tamanho;
    int capacidade;
} TabelaHashComLista;

// Função para criar uma nova tabela hash com lista encadeada
TabelaHashComLista criarTabelaHashComLista(int capacidade) {
    TabelaHashComLista tabela;
    tabela.tabela = (NodoLista**)malloc(sizeof(NodoLista*) * capacidade);
    tabela.capacidade = capacidade;
    tabela.tamanho = 0;
    // Inicialize a tabela com valores nulos
    for (int i = 0; i < capacidade; i++) {
        tabela.tabela[i] = NULL;
    }
    return tabela;
}

// Função para adicionar um elemento na lista encadeada
void adicionarNaLista(NodoLista** lista, int chave) {
    NodoLista* novoNodo = (NodoLista*)malloc(sizeof(NodoLista));
    novoNodo->chave = chave;
    novoNodo->proximo = *lista;
    *lista = novoNodo;
}

// Função de hash pelo método da divisão
int hashDivisaoComLista(int chave, int capacidade) {
    return chave % capacidade;
}

// Função para inserir uma chave na tabela hash com lista encadeada
void inserirHashComLista(TabelaHashComLista* tabela, int chave) {
    int indice = hashDivisaoComLista(chave, tabela->capacidade);

    // Se houver colisão, adicione na lista encadeada
    adicionarNaLista(&(tabela->tabela[indice]), chave);

    // Atualize o tamanho da tabela
    tabela->tamanho++;
}

// Função para buscar uma chave na tabela hash com lista encadeada
int buscarHashComLista(TabelaHashComLista* tabela, int chave, int* comparacoes) {
    int indice = hashDivisaoComLista(chave, tabela->capacidade);
    NodoLista* lista = tabela->tabela[indice];

    // Percorra a lista encadeada para buscar a chave
    while (lista != NULL) {
        (*comparacoes)++;
        if (lista->chave == chave) {
            return 1; // Chave encontrada
        }
        lista = lista->proximo;
    }
    
    return 0; // Chave não encontrada
}

// Função para liberar a memória da tabela hash com lista encadeada
void liberarTabelaHashComLista(TabelaHashComLista* tabela) {
    for (int i = 0; i < tabela->capacidade; i++) {
        NodoLista* lista = tabela->tabela[i];
        while (lista != NULL) {
            NodoLista* proximo = lista->proximo;
            free(lista);
            lista = proximo;
        }
    }
    free(tabela->tabela);
    tabela->tabela = NULL;
    tabela->capacidade = 0;
    tabela->tamanho = 0;
}

// Função para criar um novo nodo AVL
Nodo* criarNodo(int chave) {
    Nodo* novoNodo = (Nodo*)malloc(sizeof(Nodo));
    novoNodo->chave = chave;
    novoNodo->altDireita = 0;
    novoNodo->altEsquerda = 0;
    novoNodo->direita = NULL;
    novoNodo->esquerda = NULL;
    return novoNodo;
}

//Função de rotação Esquerda da avl
Nodo* rotacaoEsquerda(Nodo* nodo){
   //Realizando a rotação
    Nodo *aux1, *aux2;
    aux1 = nodo->direita;
    aux2 = aux1->esquerda;
    nodo->direita = aux2;
    aux1->esquerda = nodo;

    //Ajustando a altura do nodo
    if(nodo->direita == NULL)
        nodo->altDireita = 0;
    else if(nodo->direita->altEsquerda > nodo->direita->altDireita){
        nodo->altDireita = nodo->direita->altEsquerda + 1;
    }
    else{
        nodo->altDireita = nodo->direita->altDireita + 1;
    }
    if(aux1->esquerda->altEsquerda > aux1->esquerda->altDireita){
        aux1->altEsquerda = aux1->esquerda->altEsquerda + 1;
    }else{
        aux1->altEsquerda = aux1->esquerda->altDireita + 1;
    }
    return aux1;
}

//Função de rotação direita da avl
Nodo* rotacaoDireita(Nodo* nodo){
    // Realizando a rotação
    Nodo *aux1, *aux2;
    aux1 = nodo->esquerda;
    aux2 = aux1->direita;
    nodo->esquerda = aux2;
    aux1->direita = nodo;

    // Ajustando a altura do nodo
    if (nodo->esquerda == NULL)
        nodo->altEsquerda = 0;
    else if (nodo->esquerda->altEsquerda > nodo->esquerda->altDireita){
        nodo->altEsquerda = nodo->esquerda->altEsquerda + 1;
    } else {
        nodo->altEsquerda = nodo->esquerda->altDireita + 1;
    }
    if (aux1->direita->altEsquerda > aux1->direita->altDireita){
        aux1->altDireita = aux1->direita->altEsquerda + 1;
    } else {
        aux1->altDireita = aux1->direita->altDireita + 1;
    }
    return aux1;
}


//Função para balancear a avl
Nodo* balanceamento(Nodo *nodo){
    if(nodo == NULL){
        return NULL;
    }else{
        int fdX = nodo->altDireita - nodo->altEsquerda;

        if(fdX == 2){
            int fdY = nodo->direita->altDireita - nodo->direita->altEsquerda;

            if(fdY >= 0){
                nodo = rotacaoEsquerda(nodo);
            }else{
                nodo->direita = rotacaoDireita(nodo->direita);
                nodo = rotacaoEsquerda(nodo);
            }
        }else if(fdX == -2){
            int fdY = nodo->esquerda->altDireita - nodo->esquerda->altEsquerda;

            if(fdY <= 0){
                nodo = rotacaoDireita(nodo);
            }else{
                nodo->esquerda = rotacaoEsquerda(nodo->esquerda);
                nodo = rotacaoDirkeita(nodo);
            }
        }
        return nodo;
    }
}


//Função para inserir AVL
Nodo* inserirAVL(Nodo* raiz, int chave) {
    // Implementação da inserção na árvore AVL

    if (raiz == NULL) {
        return criarNodo(chave);
    }

    if (chave < raiz->chave) {
        raiz->esquerda = inserirAVL(raiz->esquerda, chave);
    } else if (chave > raiz->chave) {
        raiz->direita = inserirAVL(raiz->direita, chave);
    } else {
        // Duplicatas não são permitidas (você pode ajustar isso conforme necessário)
        return raiz;
    }

    // Atualizar as alturas e balancear
    raiz->altEsquerda = (raiz->esquerda == NULL) ? 0 : (raiz->esquerda->altEsquerda > raiz->esquerda->altDireita) ? raiz->esquerda->altEsquerda + 1 : raiz->esquerda->altDireita + 1;
    raiz->altDireita = (raiz->direita == NULL) ? 0 : (raiz->direita->altEsquerda > raiz->direita->altDireita) ? raiz->direita->altEsquerda + 1 : raiz->direita->altDireita + 1;

    // Balancear o nodo
    return balanceamento(raiz);
}


// Função para buscar uma chave na árvore AVL
int buscarAVL(Nodo* raiz, int chave, int* comparacoes) {
    // Caso base: a chave não está presente ou chegou a uma folha
    if (raiz == NULL) {
        *comparacoes += 1; // Incrementa o contador de comparações
        return 0; // Chave não encontrada
    }

    // Compara a chave com a chave do nodo atual
    if (chave == raiz->chave) {
        *comparacoes += 1; // Incrementa o contador de comparações
        return 1; // Chave encontrada
    } else if (chave < raiz->chave) {
        // Se a chave desejada é menor, buscar na subárvore esquerda
        *comparacoes += 1; // Incrementa o contador de comparações
        return buscarAVL(raiz->esquerda, chave, comparacoes);
    } else {
        // Se a chave desejada é maior, buscar na subárvore direita
        *comparacoes += 1; // Incrementa o contador de comparações
        return buscarAVL(raiz->direita, chave, comparacoes);
    }
}

void liberarArvore(Nodo* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

/// Função para criar uma nova tabela hash
TabelaHash criarTabelaHash(int capacidade) {
    TabelaHash tabela;
    tabela.tabela = (int*)malloc(sizeof(int) * capacidade);
    tabela.capacidade = capacidade;
    tabela.tamanho = 0;
    // Inicialize a tabela com valores nulos
    for (int i = 0; i < capacidade; i++) {
        tabela.tabela[i] = -1;
    }
    return tabela;
}

// Função de hash pelo método da divisão
int hashDivisao(int chave, int capacidade) {
    return chave % capacidade;
}

// Função para redimensionar a tabela hash
void redimensionarTabela(TabelaHash* tabela) {
    int novaCapacidade = tabela->capacidade * 2;
    int* novaTabela = (int*)malloc(sizeof(int) * novaCapacidade);

    // Inicialize a nova tabela com valores nulos
    for (int i = 0; i < novaCapacidade; i++) {
        novaTabela[i] = -1;
    }

    // Reinsira as chaves na nova tabela
    for (int i = 0; i < tabela->capacidade; i++) {
        if (tabela->tabela[i] != -1) {
            int chave = tabela->tabela[i];
            int indice = hashDivisao(chave, novaCapacidade);

            // Use sondagem linear para encontrar uma posição vazia na nova tabela
            while (novaTabela[indice] != -1) {
                indice = (indice + 1) % novaCapacidade;
            }

            novaTabela[indice] = chave;
        }
    }

    // Libere a memória da tabela antiga e atualize os dados da nova tabela
    free(tabela->tabela);
    tabela->tabela = novaTabela;
    tabela->capacidade = novaCapacidade;
}

// Função para inserir uma chave na tabela hash
void inserirHash(TabelaHash* tabela, int chave) {
    // Verifique se é necessário redimensionar a tabela
    if ((float)tabela->tamanho / tabela->capacidade >= 0.7) {
        redimensionarTabela(tabela);
    }

    int indice = hashDivisao(chave, tabela->capacidade);

    // Use sondagem linear para encontrar uma posição vazia na tabela
    while (tabela->tabela[indice] != -1) {
        indice = (indice + 1) % tabela->capacidade;
    }

    // Insira a chave na tabela e atualize o tamanho
    tabela->tabela[indice] = chave;
    tabela->tamanho++;
}

// Função para buscar uma chave na tabela hash
int buscarHash(TabelaHash* tabela, int chave, int* comparacoes) {
    int indice = hashDivisao(chave, tabela->capacidade);

    // Use sondagem linear para encontrar a chave ou uma posição vazia
    while (tabela->tabela[indice] != -1) {
        (*comparacoes)++;
        if (tabela->tabela[indice] == chave) {
            return 1; // Chave encontrada
        }
        indice = (indice + 1) % tabela->capacidade;
    }

    return 0; // Chave não encontrada
}

int main() {
    // Leitura do arquivo e execução das operações
    Nodo* raiz = NULL;
    FILE* inFile = fopen("in3.txt", "r"); // Abre o arquivo de entrada
    
    if (inFile == NULL) {
        printf("Erro ao abrir arquivos.");
        return 1;
    }

    char operacao;
    int numero;
    int comparacoes = 0;
    int op = 0;
    clock_t tempoInicial, tempoFinal;
    
    while (op != 1 && op != 2 && op != 3) {
        printf("Selecione a estrutura de dados desejada:\n1-Arvore AVL\n2-Tabela Hash com sondagem linear\n3-Tabela Hash com lista encadeada\n4-Sair\n");
        scanf("%d", &op);

        if (op == 1) {
            // Inicializa o relógio antes das operações da árvore AVL
            tempoInicial = clock();

            while (fscanf(inFile, " %c %d", &operacao, &numero) == 2) {
                if (operacao == 'i') {
                    // Inserir na árvore AVL
                    raiz = inserirAVL(raiz, numero);
                } else if (operacao == 'b') {
                    // Buscar na árvore AVL
                    int resultado = buscarAVL(raiz, numero, &comparacoes);
                    if (resultado) {
                        printf("Chave %d encontrada na arvore AVL, com %d comparacoes.\n", numero, comparacoes);
                    } else {
                        printf("Chave %d nao encontrada na arvore AVL.\n", numero);
                    }
                }
            }

            // Finaliza o relógio após as operações da árvore AVL
            tempoFinal = clock();
        } else if (op == 2) {
            // Inicializa o relógio antes das operações da tabela hash com sondagem linear
            tempoInicial = clock();

            TabelaHash tabela = criarTabelaHash(100);
            while (fscanf(inFile, " %c %d", &operacao, &numero) == 2) {
                if (operacao == 'i') {
                    // Inserir na tabela hash com sondagem linear
                    inserirHash(&tabela, numero);
                } else if (operacao == 'b') {
                    // Buscar na tabela hash com sondagem linear
                    int resultado = buscarHash(&tabela, numero, &comparacoes);
                    if (resultado) {
                        printf("Chave %d encontrada na tabela hash com sondagem linear, com %d comparacoes.\n", numero, comparacoes);
                    } else {
                        printf("Chave %d nao encontrada na tabela hash com sondagem linear.\n", numero);
                    }
                }
            }

            // Finaliza o relógio após as operações da tabela hash com sondagem linear
            tempoFinal = clock();
            
            free(tabela.tabela); // Liberar a tabela hash
        } else if (op == 3) {
            // Inicializa o relógio antes das operações da tabela hash com lista encadeada
            tempoInicial = clock();

            TabelaHashComLista tabelaComLista = criarTabelaHashComLista(100);
            while (fscanf(inFile, " %c %d", &operacao, &numero) == 2) {
                if (operacao == 'i') {
                    // Inserir na tabela hash com lista encadeada
                    inserirHashComLista(&tabelaComLista, numero);
                } else if (operacao == 'b') {
                    // Buscar na tabela hash com lista encadeada
                    int resultado = buscarHashComLista(&tabelaComLista, numero, &comparacoes);
                    if (resultado) {
                        printf("Chave %d encontrada na tabela hash com lista encadeada, com %d comparacoes.\n", numero, comparacoes);
                    } else {
                        printf("Chave %d nao encontrada na tabela hash com lista encadeada.\n", numero);
                    }
                } 
            }

            // Finaliza o relógio após as operações da tabela hash com lista encadeada
            tempoFinal = clock();
            
            liberarTabelaHashComLista(&tabelaComLista); // Liberar a tabela hash com lista encadeada
        }
        else if(op == 4)
            break;

        // Calcula e imprime o tempo total de execução
        double tempoTotal = ((double)(tempoFinal - tempoInicial)) / CLOCKS_PER_SEC;
        printf("Tempo total de execucao: %.4f segundos\n", tempoTotal);

        fseek(inFile, 0, SEEK_SET); // Reposiciona o ponteiro do arquivo para o início
    }
    
    liberarArvore(raiz);
    fclose(inFile);

    return 0;
}

