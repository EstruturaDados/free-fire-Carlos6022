#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

/**
 * ============================================================================
 * @file desafio_mochila_aventureiro.c
 * @brief Sistema de Inventário: Análise Comparativa de Estruturas de Dados.
 * @author [Seu Nome/Estudante de ADS]
 * * @details DOCUMENTAÇÃO TÉCNICA (NÍVEL AVENTUREIRO):
 * Este software implementa um CRUD de itens utilizando duas estruturas distintas
 * para fins de análise de desempenho e gerenciamento de memória:
 * * 1. VETOR (Array Estático):
 * - Vantagens: Acesso aleatório O(1), permite Busca Binária O(log n).
 * - Desvantagens: Tamanho fixo, remoção custosa O(n) devido ao deslocamento.
 * * 2. LISTA ENCADEADA (Alocação Dinâmica):
 * - Vantagens: Tamanho ilimitado (depende da RAM), inserção O(1) imediata.
 * - Desvantagens: Acesso sequencial obrigatório O(n), não permite Busca Binária.
 * ============================================================================
 */

#define TAM_MAX 10       // Capacidade máxima para a implementação estática
#define MAX_STR_LEN 30   // Tamanho do buffer de strings

// Variáveis globais para telemetria e análise de complexidade
int comparacoesVetor = 0;
int comparacoesLista = 0;

/**
 * @struct Item
 * @brief Payload de dados. Representa o objeto de negócio armazenado.
 */
typedef struct {
    char nome[MAX_STR_LEN];
    char tipo[20];
    int quantidade;
} Item;

/**
 * @struct No
 * @brief Elemento base da Lista Encadeada.
 * @note Diferença Estrutural: Ao contrário do vetor, o 'Nó' carrega um ponteiro
 * (*proximo) para conectar dados dispersos na memória RAM.
 */
typedef struct No {
    Item dados;
    struct No *proximo;
} No;

/**
 * @struct MochilaVetor
 * @brief Gerenciador da implementação estática.
 * @note Utiliza memória contígua. A variável 'quantidadeAtual' serve como
 * cursor para controlar o fim lógico do vetor.
 */
typedef struct {
    Item itens[TAM_MAX];
    int quantidadeAtual;
} MochilaVetor;

// --- Protótipos das Funções ---
void limparBuffer();
void inicializarMochila(MochilaVetor *m);
void liberarLista(No *cabeca);

// Operações em Vetor (Static Array)
void inserirItemVetor(MochilaVetor *m, Item item);
void removerItemVetor(MochilaVetor *m, const char *nome);
void listarItensVetor(const MochilaVetor *m);
void ordenarVetor(MochilaVetor *m);
int buscarSequencialVetor(MochilaVetor *m, const char *nome);
int buscarBinariaVetor(MochilaVetor *m, const char *nome);

// Operações em Lista Encadeada (Linked List)
No *inserirItemLista(No *cabeca, Item item);
No *removerItemLista(No *cabeca, const char *nome);
void listarItensLista(No *cabeca);
void buscarSequencialLista(No *cabeca, const char *nome);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================
int main() {
    MochilaVetor mv;
    inicializarMochila(&mv);
    No *ml = NULL; // Ponteiro inicial da lista (Head)

    int opcao, estrutura;
    Item tempItem;
    char nomeBusca[MAX_STR_LEN];
    clock_t t; // Variável para medição de tempo de CPU (Profiling)

    printf("--- Configuração Inicial do Sistema ---\n");
    printf("\n[1] Vetor (Sequencial - Memoria Contigua)\n[2] Lista Encadeada (Dinamica - Memoria Dispersa)\n\nOpcao: ");
    scanf("%d", &estrutura);
    limparBuffer();

    do {
        printf("\n==============================================================");
        printf("\n  MOCHILA DE SOBREVIVENCIA - PAINEL DE CONTROLE [%s]   \n", (estrutura == 1 ? "VETOR" : "LISTA"));
        printf("==============================================================\n\n");

        // Exibe a capacidade atual baseada na estrutura escolhida
        if(estrutura == 1) {
            printf("Capacidade (Vetor): %d/%d slots ocupados\n\n", mv.quantidadeAtual, TAM_MAX);
        } else {
            printf("Capacidade (Lista): Alocacao Dinamica (Limitada apenas pela RAM)\n\n");
        }

        printf("1. Adicionar Item (Loot)\n2. Remover Item\n3. Listar Itens\n4. Busca Sequencial\n");
        if(estrutura == 1) {
            printf("5. Ordenar e Busca Binaria (Exclusivo Vetor)\n");
        }
        printf("0. Encerrar Sistema\n\nEscolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
        case 1: // Inserção
            printf("\n--- REGISTRO DE NOVO ITEM ---\n");
            printf("Nome: "); fgets(tempItem.nome, MAX_STR_LEN, stdin);
            tempItem.nome[strcspn(tempItem.nome, "\n")] = 0;
            printf("Tipo: "); fgets(tempItem.tipo, 20, stdin);
            tempItem.tipo[strcspn(tempItem.tipo, "\n")] = 0;
            printf("Quantidade: "); scanf("%d", &tempItem.quantidade);
            limparBuffer();

            if(estrutura == 1) {
                inserirItemVetor(&mv, tempItem);
                listarItensVetor(&mv);
            } else {
                ml = inserirItemLista(ml, tempItem);
                listarItensLista(ml);
            }
            break;

        case 2: // Remoção
            // Validação de Pré-condição: Não tentar remover de estrutura vazia
            if ((estrutura == 1 && mv.quantidadeAtual == 0) || (estrutura == 2 && ml == NULL)) {
                 printf("\n[ALERTA SISTEMA]: A mochila esta vazia. Operacao cancelada.\n");
            } else {
                printf("Digite o nome do item a remover: ");
                fgets(nomeBusca, MAX_STR_LEN, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                
                if(estrutura == 1) {
                    removerItemVetor(&mv, nomeBusca);
                } else {
                    ml = removerItemLista(ml, nomeBusca);
                }
            }
            break;

        case 3: // Listagem
            if(estrutura == 1) listarItensVetor(&mv);
            else listarItensLista(ml);
            break;

        case 4: // Busca Linear
            if ((estrutura == 1 && mv.quantidadeAtual == 0) || (estrutura == 2 && ml == NULL)) {
                 printf("\n[ALERTA SISTEMA]: A mochila esta vazia. Busca cancelada.\n");
            } else {
                printf("Termo de busca (Sequencial): ");
                fgets(nomeBusca, MAX_STR_LEN, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                
                t = clock(); // Início da medição de tempo
                if(estrutura == 1) {
                    buscarSequencialVetor(&mv, nomeBusca);
                    printf("-> Métricas: %d comparacoes realizadas (Vetor)\n", comparacoesVetor);
                } else {
                    buscarSequencialLista(ml, nomeBusca);
                    printf("-> Métricas: %d comparacoes realizadas (Lista)\n", comparacoesLista);
                }
                t = clock() - t; // Fim da medição
                printf("Tempo de CPU: %f segundos\n", ((double)t) / CLOCKS_PER_SEC);
            }
            break;

        case 5: // Ordenação + Busca Binária (Apenas Vetor)
            if(estrutura == 1) {
                if(mv.quantidadeAtual == 0) {
                     printf("\n[ALERTA SISTEMA]: Mochila vazia.\n");
                } else {
                    ordenarVetor(&mv); // Pré-requisito para busca binária
                    printf("Termo de busca (Binaria): ");
                    fgets(nomeBusca, MAX_STR_LEN, stdin);
                    nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                    
                    t = clock();
                    int res = buscarBinariaVetor(&mv, nomeBusca);
                    t = clock() - t;
                    
                    if(res != -1) printf("\n[SUCESSO] Item localizado no indice %d.\n", res);
                    else printf("\n[FALHA] Item nao encontrado.\n");
                    
                    printf("-> Métricas: %d comparacoes na Busca Binaria (O(log n))\n", comparacoesVetor);
                    printf("Tempo de CPU: %f segundos\n", ((double)t) / CLOCKS_PER_SEC);
                }
            }
            break;

        case 0:
            printf("Encerrando sessao...\n");
            break;

        default:
            printf("Opcao invalida!\n");
        }

        if(opcao != 0) {
            printf("\n[SISTEMA]: Pressione [ENTER] para continuar...");
            getchar();
        }
    } while(opcao != 0);

    // Garbage Collection Manual (Obrigatório para Listas em C)
    liberarLista(ml); 
    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO: VETOR (LISTA SEQUENCIAL)
// ============================================================================

void inicializarMochila(MochilaVetor *m) { m->quantidadeAtual = 0; }

/**
 * @brief Adiciona um item ao final do vetor.
 * @note Complexidade: O(1).
 * @details Muito rápido pois acessa diretamente o índice via 'quantidadeAtual',
 * mas limitado pelo tamanho fixo TAM_MAX definido em tempo de compilação.
 */
void inserirItemVetor(MochilaVetor *m, Item item) {
    if(m->quantidadeAtual >= TAM_MAX) {
        printf("\n[ERRO CRITICO] Overflow: Capacidade maxima do vetor atingida!\n");
        return;
    }
    m->itens[m->quantidadeAtual++] = item;
    printf("\n[Vetor] Item '%s' persistido na memoria estatica.\n", item.nome);
}

/**
 * @brief Algoritmo Bubble Sort para ordenação alfabética.
 * @note Complexidade: O(n^2).
 * @details Necessário para habilitar a Busca Binária. Ineficiente para grandes
 * volumes de dados, mas aceitável para inventários pequenos.
 */
void ordenarVetor(MochilaVetor *m) {
    if(m->quantidadeAtual == 0) return;
    Item temp;
    for(int i = 0; i < m->quantidadeAtual - 1; i++) {
        for(int j = 0; j < m->quantidadeAtual - i - 1; j++) {
            if(strcmp(m->itens[j].nome, m->itens[j+1].nome) > 0) {
                temp = m->itens[j];
                m->itens[j] = m->itens[j+1];
                m->itens[j+1] = temp;
            }
        }
    }
    printf("\n[SISTEMA] Vetor ordenado (Bubble Sort) para otimizar buscas.\n");
}

/**
 * @brief Busca Linear/Sequencial no vetor.
 * @note Complexidade: O(n).
 * @details Percorre o array índice por índice. Ineficiente se o item estiver
 * no final de um vetor grande.
 */
int buscarSequencialVetor(MochilaVetor *m, const char *nome) {
    if(m->quantidadeAtual == 0) {
        printf("\n[AVISO]: Vetor vazio.\n");
        return -1;
    }
    comparacoesVetor = 0;
    for(int i = 0; i < m->quantidadeAtual; i++) {
        comparacoesVetor++;
        if(strcmp(m->itens[i].nome, nome) == 0) return i;
    }
    printf("\nItem nao encontrado (Busca Sequencial).\n");
    return -1;
}

/**
 * @brief Busca Binária (Divide and Conquer).
 * @note Complexidade: O(log n).
 * @details A grande vantagem do Vetor. Divide o espaço de busca pela metade
 * a cada iteração. Exige que o vetor esteja previamente ordenado.
 */
int buscarBinariaVetor(MochilaVetor *m, const char *nome) {
    if(m->quantidadeAtual == 0) {
        printf("\n[AVISO]: Vetor vazio.\n");
        return -1;
    }
    int inicio = 0, fim = m->quantidadeAtual - 1;
    comparacoesVetor = 0;
    while(inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        comparacoesVetor++;
        int comp = strcmp(m->itens[meio].nome, nome);
        if(comp == 0) return meio;
        if(comp < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    return -1;
}

/**
 * @brief Remove item e reorganiza a memória.
 * @note Complexidade: O(n) no pior caso.
 * @details O "Calcanhar de Aquiles" do vetor. Ao remover um item do meio,
 * exige o deslocamento (shift) de todos os itens subsequentes para evitar
 * buracos na memória.
 */
void removerItemVetor(MochilaVetor *m, const char *nome) {
    if(m->quantidadeAtual == 0) {
        printf("\n[AVISO]: Vetor vazio. Operacao abortada.\n");
        return;
    }

    int pos = -1;
    for(int i = 0; i < m->quantidadeAtual; i++) {
        if(strcmp(m->itens[i].nome, nome) == 0) { pos = i; break; }
    }
    
    if(pos == -1) {
        printf("\n[ERRO]: Item '%s' nao localizado no vetor.\n", nome);
        return;
    }

    // Realiza o Shift (deslocamento) para a esquerda
    for(int i = pos; i < m->quantidadeAtual - 1; i++) m->itens[i] = m->itens[i+1];
    m->quantidadeAtual--;
    
    printf("\n[SUCESSO] Item '%s' removido e vetor reorganizado.\n", nome);
    listarItensVetor(m);
}

/**
 * @brief Renderiza a tabela de itens do vetor.
 */
void listarItensVetor(const MochilaVetor *m) {
    if(m->quantidadeAtual == 0) {
        printf("\n[RELATORIO] A mochila (Vetor) esta vazia.\n");
        return;
    }
    printf("\n--- RELATORIO DE INVENTARIO (VETOR: %d/%d) ---\n", m->quantidadeAtual, TAM_MAX);
    printf("-------------------------------------------------------------------\n");
    printf("%-25s | %-20s | %-10s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("-------------------------------------------------------------------\n");
    for(int i = 0; i < m->quantidadeAtual; i++)
        printf("%-25s | %-20s | %-10d\n", m->itens[i].nome, m->itens[i].tipo, m->itens[i].quantidade);
    printf("-------------------------------------------------------------------\n");
}

// ============================================================================
// IMPLEMENTAÇÃO: LISTA ENCADEADA (LISTA DINÂMICA)
// ============================================================================

/**
 * @brief Insere um novo nó no INÍCIO da lista.
 * @note Complexidade: O(1).
 * @details Grande vantagem da lista: não precisa verificar limites de tamanho
 * nem deslocar elementos. Apenas aloca memória (malloc) e ajusta ponteiros.
 */
No *inserirItemLista(No *cabeca, Item item) {
    No *novo = (No *)malloc(sizeof(No));
    if(!novo) {
        printf("\n[ERRO CRITICO] Falha de alocacao de memoria (RAM cheia?)\n");
        return cabeca;
    }
    novo->dados = item;
    novo->proximo = cabeca; // O novo nó aponta para a antiga cabeça
    return novo; // O novo nó se torna a nova cabeça
}

/**
 * @brief Busca Sequencial na lista (única possível).
 * @note Complexidade: O(n).
 * @details Diferença Crítica: Como os nós estão espalhados na memória, não
 * podemos calcular o "meio" da lista instantaneamente, inviabilizando a 
 * Busca Binária eficiente.
 */
void buscarSequencialLista(No *cabeca, const char *nome) {
    if(cabeca == NULL) {
        printf("\n[AVISO]: Lista vazia.\n");
        return;
    }
    comparacoesLista = 0;
    No *atual = cabeca;
    while(atual) {
        comparacoesLista++;
        if(strcmp(atual->dados.nome, nome) == 0) {
            printf("\n[SUCESSO] Item encontrado na Lista Dinamica!\n");
            return;
        }
        atual = atual->proximo;
    }
    printf("\n[FALHA] Item nao encontrado na lista.\n");
}

/**
 * @brief Remove um nó da lista manipulando ponteiros.
 * @note Complexidade: O(n) para achar, O(1) para deletar.
 * @details Diferença Crítica: Ao remover, não precisamos deslocar dados.
 * Apenas fazemos o nó anterior apontar para o nó seguinte (bypass) e damos
 * free() na memória do nó removido.
 */
No* removerItemLista(No *cabeca, const char *nome) {
    if(cabeca == NULL) {
        printf("\n[AVISO]: Lista vazia. Nada a remover.\n");
        return NULL;
    }

    No *atual = cabeca, *ant = NULL;
    
    // Percorre a lista até achar o item ou chegar ao fim
    while(atual && strcmp(atual->dados.nome, nome) != 0) {
        ant = atual; 
        atual = atual->proximo;
    }

    if(atual == NULL) {
        printf("\n[ERRO]: Item '%s' nao encontrado na lista.\n", nome);
        return cabeca;
    }

    // Lógica de "Costura" dos ponteiros
    if(!ant) cabeca = atual->proximo; // Era o primeiro item
    else ant->proximo = atual->proximo; // Era um item do meio/fim
    
    free(atual); // Libera memória para o SO evitar Memory Leak
    printf("\n[SUCESSO] Item '%s' removido e memoria liberada.\n", nome);
    
    listarItensLista(cabeca);
    return cabeca;
}

/**
 * @brief Renderiza a tabela percorrendo os nós.
 * @details Necessário percorrer a lista inteira (O(n)) apenas para contar
 * os elementos antes de exibir.
 */
void listarItensLista(No *cabeca) {
    if (cabeca == NULL) {
        printf("\n[RELATORIO] A mochila (Lista) esta vazia.\n");
        return;
    }

    // Contagem manual (Overhead da lista encadeada simples)
    int contador = 0;
    No *temp = cabeca;
    while (temp != NULL) {
        contador++;
        temp = temp->proximo;
    }

    printf("\n--- RELATORIO DE INVENTARIO (LISTA: %d itens alocados) ---\n", contador);
    printf("-------------------------------------------------------------------\n");
    printf("%-25s | %-20s | %-10s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("-------------------------------------------------------------------\n");

    No *atual = cabeca;
    while (atual != NULL) {
        printf("%-25s | %-20s | %-10d\n", 
               atual->dados.nome, 
               atual->dados.tipo, 
               atual->dados.quantidade);
        atual = atual->proximo;
    }
    printf("-------------------------------------------------------------------\n");
}

/**
 * @brief Garbage Collector Manual.
 * @details Percorre a lista liberando cada nó alocado via malloc.
 * Essencial em C para evitar vazamento de memória (Memory Leak) ao fechar o programa.
 */
void liberarLista(No *cabeca) {
    while(cabeca) {
        No *t = cabeca; 
        cabeca = cabeca->proximo; 
        free(t); 
    }
}

/**
 * @brief Utilitário para limpeza de buffer de entrada.
 * @details Substitui o uso inseguro de fflush(stdin). Consome caracteres
 * residuais até encontrar uma quebra de linha ou fim de arquivo.
 */
void limparBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF)
        ;
}