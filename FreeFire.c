/**
 * @file desafioMestre.c
 * @brief Sistema avançado de gerenciamento de componentes com análise de desempenho
 * @details Implementa um jogo educacional onde o jogador deve montar uma torre de fuga
 *          escolhendo diferentes estratégias de ordenação (Bubble Sort, Insertion Sort,
 *          Selection Sort) e aplicando busca binária para localizar componentes-chave.
 *          O programa mede desempenho através de comparações e tempo de execução.
 * @author Disciplina: Estruturas de Dados - Nível Mestre
 * @version 2.0
 * @date Janeiro 2026
 */

#include <stdio.h>  // Entrada/Saída padrão (printf, scanf, fgets)
#include <stdlib.h> // Funções de alocação de memória (malloc, free)
#include <string.h> // Funções de manipulação de strings (strcmp, strcpy, strcspn)
#include <time.h>   // Medição de tempo (clock, CLOCKS_PER_SEC)

/** @def MAX_COMPONENTES Capacidade máxima de componentes na mochila */
#define MAX_COMPONENTES 20

/** @def MAX_STR_LEN Tamanho máximo para strings (nome e tipo) */
#define MAX_STR_LEN 30

/**
 * @brief Variável global para contar comparações em operações com vetores
 * @details Incrementada durante algoritmos de ordenação e busca
 *          Utilizada para análise de complexidade temporal O(n)
 */
int comparacoesVetor = 0;

/**
 * @struct Componente
 * @brief Estrutura que representa um componente da torre de fuga
 * @param nome Nome único do componente (ex: "chip central", "antena", "bateria")
 * @param tipo Categoria do componente (ex: "estrutural", "eletrônico", "energia")
 * @param quantidade Número de unidades deste componente disponível
 * @param prioridade Prioridade de montagem (1-10, onde 10 é máxima urgência)
 */
typedef struct {
    char nome[30];  /**< Nome do componente */
    char tipo[20];  /**< Tipo/categoria do componente */
    int quantidade; /**< Quantidade disponível */
    int prioridade; /**< Prioridade de montagem (1-10) */
} Componente;

// ============================================================================
// PROTÓTIPOS DAS FUNÇÕES OBRIGATÓRIAS - DESAFIO MESTRE
// ============================================================================

/**
 * @brief Ordena componentes por prioridade (decrescente) - Selection Sort
 * @param arr Array de componentes a ordenar (MODIFICADO in-place)
 * @param tamanho Número de elementos no array
 * @details Algoritmo: Selection Sort
 *          Complexidade: O(n²) comparações
 *          Estratégia: Seleciona o maior valor não ordenado a cada iteração
 *          Ideal para: Dados com poucos elementos ou análise educacional
 */
void selectionSortPrioridade(Componente arr[], int tamanho);

/**
 * @brief Ordena componentes por tipo (alfabético) - Insertion Sort
 * @param arr Array de componentes a ordenar (MODIFICADO in-place)
 * @param tamanho Número de elementos no array
 * @details Algoritmo: Insertion Sort
 *          Complexidade: O(n²) comparações no pior caso
 *          Estratégia: Insere cada elemento na posição correta
 *          Ideal para: Dados parcialmente ordenados ou listas pequenas
 */
void insertionSortTipo(Componente arr[], int tamanho);

/**
 * @brief Ordena componentes por nome (alfabético) - Bubble Sort
 * @param arr Array de componentes a ordenar (MODIFICADO in-place)
 * @param tamanho Número de elementos no array
 * @details Algoritmo: Bubble Sort
 *          Complexidade: O(n²) comparações no pior caso
 *          Estratégia: Compara elementos adjacentes, trocando se necessário
 *          Necessário: Ordenação por NOME para usar buscaBinariaPorNome()
 */
void bubbleSortNome(Componente arr[], int tamanho);

/**
 * @brief Realiza busca binária por nome de componente
 * @param arr Array de componentes ORDENADO POR NOME (bubbleSortNome)
 * @param tamanho Número de elementos no array
 * @param nome Nome do componente a buscar (string terminada em '\0')
 * @return Índice do componente encontrado, ou -1 se não encontrado
 * @details Algoritmo: Busca Binária
 *          Complexidade: O(log n) comparações
 *          Pré-condição: Array DEVE estar ordenado alfabeticamente por nome
 *          Telemetria: Incrementa comparacoesVetor para análise de desempenho
 */
int buscaBinariaPorNome(Componente arr[], int tamanho, char nome[]);

/**
 * @brief Exibe tabela formatada com todos os componentes
 * @param arr Array de componentes a exibir
 * @param tamanho Número de elementos a mostrar
 * @details Formato: Tabela com 4 colunas (NOME | TIPO | QUANTIDADE | PRIORIDADE)
 *          Largura fixa para melhor legibilidade
 *          Inclui separadores visuais de linha
 */
void mostrarComponentes(Componente arr[], int tamanho);

/**
 * @brief Mede o tempo de CPU gasto na execução de um algoritmo
 * @param algoritmo Ponteiro para função de ordenação (bubbleSortNome, insertionSortTipo ou selectionSortPrioridade)
 * @param arr Array de componentes
 * @param tamanho Número de elementos
 * @param descricao String descritiva do algoritmo (para exibição)
 * @details Usa clock() para medir tempo em ticks do processador
 *          Converte para segundos usando CLOCKS_PER_SEC
 *          Exibe resultado em formato legível (ex: 0.000123 segundos)
 *          Integra com contagem de comparações para análise educacional
 */
void medirTempo(void (*algoritmo)(Componente[], int), Componente arr[], int tamanho, const char descricao[]);

// ============================================================================
// FUNÇÃO PRINCIPAL - SISTEMA DE ORGANIZAÇÃO DA TORRE DE FUGA
// ============================================================================
/**
 * @brief Menu interativo para gerenciar componentes da torre de fuga
 * @details Permite adicionar, remover, listar, ordenar (3 estratégias) e buscar componentes
 *          Integra análise de desempenho com contagem de comparações e medição de tempo
 * @return 0 ao encerrar o programa
 */
int main() {
    // ======= Inicialização de Variáveis =======
    Componente componentes[MAX_COMPONENTES]; // Array de estruturas para armazenar componentes
    int quantidade = 0;                      // Contador de componentes adicionados (0-10)
    int opcao;                               // Opção do menu escolhida pelo jogador
    int mochilaOrdenada = 0;                 // Flag: 0 = não ordenado, 1 = ordenado por nome (necessário para busca binária)
    char nomeBusca[MAX_STR_LEN];             // Buffer para nome a ser buscado

    // ======= Loop Principal do Menu =======
    do {
        // Exibe cabeçalho com status atual
        printf("\n========================================================\n");
        printf("    PLANO DE FUGA - CODIGO DA ILHA (NIVEL MESTRE)\n");
        printf("========================================================\n");
        printf("Itens na Mochila: %d/10\n", quantidade);
        printf("Status da Ordenacao por Nome: %s\n", mochilaOrdenada ? "ORDENADO" : "NAO ORDENADO");
        printf("\n");

        // Exibe opções disponíveis
        printf("1. Adicionar Componente\n");
        printf("2. Descartar Componente\n");
        printf("3. Listar Componentes (Inventario)\n");
        printf("4. Organizar Mochila (Ordenar Componentes)\n");
        printf("5. Busca Binaria por Componente-Chave (por nome)\n");
        printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
        printf("--------------------------------------------------------\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        while(getchar() != '\n')
            ; // Limpa buffer de entrada

        switch(opcao) {
        // ============== CASO 1: ADICIONAR COMPONENTE ==============
        case 1:
            // Verifica limite de capacidade (máximo 10 componentes)
            if(quantidade >= 10) {
                printf("\n[ERRO] Mochila cheia! Capacidade maxima: 10 componentes.\n");
            } else {
                printf("\n--- Coletando Novo Componente ---\n");

                // Entrada 1: Nome do componente (max 30 caracteres)
                printf("Nome: ");
                fgets(componentes[quantidade].nome, 30, stdin);
                componentes[quantidade].nome[strcspn(componentes[quantidade].nome, "\n")] = 0;

                // Entrada 2: Tipo/Categoria do componente
                printf("Tipo (Estrutural, Eletronico, Energia): ");
                fgets(componentes[quantidade].tipo, 20, stdin);
                componentes[quantidade].tipo[strcspn(componentes[quantidade].tipo, "\n")] = 0;

                // Entrada 3: Quantidade de unidades (inteiro)
                printf("Quantidade: ");
                scanf("%d", &componentes[quantidade].quantidade);
                while(getchar() != '\n')
                    ;

                // Entrada 4: Prioridade (1-10, onde 10 é máxima urgência)
                printf("Prioridade de Montagem (1-10): ");
                scanf("%d", &componentes[quantidade].prioridade);
                while(getchar() != '\n')
                    ;

                // Incrementa contador e marca como não ordenado
                quantidade++;
                mochilaOrdenada = 0; // Novo componente desordeniza a lista
                printf("\nComponente '%s' adicionado!\n", componentes[quantidade - 1].nome);

                // Exibe tabela atualizada
                mostrarComponentes(componentes, quantidade);
            }
            break;

        case 2: // Descartar Componente
            if(quantidade == 0) {
                printf("\n[AVISO] Mochila vazia. Nada para descartar.\n");
            } else {
                printf("\n--- Descartando Componente ---\n");
                printf("Nome do componente a descartar: ");
                fgets(nomeBusca, MAX_STR_LEN, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;

                // Busca sequencial (O(n)) pelo nome do componente
                int encontrado = -1;
                for(int i = 0; i < quantidade; i++) {
                    if(strcmp(componentes[i].nome, nomeBusca) == 0) {
                        encontrado = i;
                        break;
                    }
                }

                if(encontrado == -1) {
                    printf("\n[ERRO] Componente '%s' nao encontrado na mochila.\n", nomeBusca);
                } else {
                    // Remove o componente deslocando os elementos posteriores (O(n))
                    for(int i = encontrado; i < quantidade - 1; i++) {
                        componentes[i] = componentes[i + 1];
                    }
                    quantidade--;
                    mochilaOrdenada = 0; // Marca como não ordenado
                    printf("\n[SUCESSO] Componente descartado da mochila.\n");
                }
            }
            break;

        case 3: // Listar Componentes (Inventário)
            // Exibe tabela formatada com todos os componentes
            mostrarComponentes(componentes, quantidade);
            break;

        case 4: // Organizar Mochila (Ordenar)
            // Submenu com 3 estratégias de ordenação diferentes
            if(quantidade == 0) {
                printf("\n[AVISO] Mochila vazia. Nada para organizar.\n");
            } else {
                printf("\n--- Estrategia de Organizacao ---\n");
                printf("Como deseja ordenar os componentes?\n");
                printf("1. Por Nome (Ordem Alfabetica)\n");
                printf("2. Por Tipo\n");
                printf("3. Por Prioridade de Montagem\n");
                printf("4. Cancelar\n");
                printf("Escolha o criterio: ");
                int subOpcao;
                scanf("%d", &subOpcao);
                while(getchar() != '\n')
                    ;

                switch(subOpcao) {
                // -------- Subopcão 1: Bubble Sort por Nome --------
                case 1:
                    printf("\n[PROCESSANDO] Organizando mochila por NOME...\n");
                    // Usa medirTempo para executar e medir bubbleSortNome O(n²)
                    medirTempo(bubbleSortNome, componentes, quantidade, "BUBBLE SORT - Por Nome");
                    mochilaOrdenada = 1; // IMPORTANTE: Marca como ordenado (necessário para busca binária)
                    mostrarComponentes(componentes, quantidade);
                    break;

                // -------- Subopcão 2: Insertion Sort por Tipo --------
                case 2:
                    printf("\n[PROCESSANDO] Organizando mochila por TIPO...\n");
                    // Usa medirTempo para executar e medir insertionSortTipo O(n²)
                    medirTempo(insertionSortTipo, componentes, quantidade, "INSERTION SORT - Por Tipo");
                    mochilaOrdenada = 0; // Não está ordenado por nome (busca binária não é válida)
                    mostrarComponentes(componentes, quantidade);
                    break;

                // -------- Subopcão 3: Selection Sort por Prioridade --------
                case 3:
                    printf("\n[PROCESSANDO] Organizando mochila por PRIORIDADE...\n");
                    // Usa medirTempo para executar e medir selectionSortPrioridade O(n²)
                    medirTempo(selectionSortPrioridade, componentes, quantidade, "SELECTION SORT - Por Prioridade");
                    mochilaOrdenada = 0; // Não está ordenado por nome (busca binária não é válida)
                    mostrarComponentes(componentes, quantidade);
                    break;

                // -------- Subopcão 4: Cancelar operação --------
                case 4:
                    printf("\nOperacao cancelada.\n");
                    break;

                default:
                    printf("Opcao invalida!\n");
                }
            }
            break;

        // ============== CASO 5: BUSCA BINÁRIA ==============
        case 5:
            // Requisitos: Array deve estar ordenado por nome (bubbleSortNome)
            if(quantidade == 0) {
                printf("\n[AVISO] Mochila vazia. Nada para buscar.\n");
            } else if(!mochilaOrdenada) {
                // Validação: mochilaOrdenada = 1 só depois de bubbleSortNome
                printf("\n");
                printf("ALERTA: A busca binaria requer que a mochila esteja ordenada por NOME.\n");
                printf("Use a Opcao 4 para organizar a mochila primeiro.\n");
            } else {
                // Entrada: Nome do componente a buscar
                printf("\n--- Busca Binaria por Componente-Chave ---\n");
                printf("Nome do componente a buscar: ");
                fgets(nomeBusca, MAX_STR_LEN, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;

                // Executa busca binária O(log n) - retorna índice ou -1
                int pos = buscaBinariaPorNome(componentes, quantidade, nomeBusca);

                if(pos == -1) {
                    // Componente não encontrado
                    printf("\n[FALHA] Componente-chave '%s' nao encontrado na mochila.\n", nomeBusca);
                } else {
                    // Componente encontrado - exibe dados
                    printf("\n--- Componente-Chave Encontrado! ---\n");
                    printf("Nome: %s, Tipo: %s, Qtd: %d, Prio: %d\n",
                           componentes[pos].nome, componentes[pos].tipo,
                           componentes[pos].quantidade, componentes[pos].prioridade);
                }
                // Telemetria: exibe número de comparações realizadas
                printf("-> Comparacoes realizadas: %d (Complexidade O(log n))\n", comparacoesVetor);
            }
            break;

        // ============== CASO 0: ENCERRAR PROGRAMA ==============
        case 0:
            printf("\n");
            printf("========================================================\n");
            printf("  TORRE DE FUGA ATIVADA! Preparando para evacuacao...\n");
            printf("========================================================\n");
            printf("Sistema encerrado com sucesso.\n");
            break;

        // ============== CASO PADRÃO: OPÇÃO INVÁLIDA ==============
        default:
            printf("\n[ERRO] Opcao invalida! Escolha entre 0-5.\n");
        }

        // Pausa para leitura do resultado (exceto ao sair)
        if(opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
    } while(opcao != 0);

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES OBRIGATÓRIAS - DESAFIO MESTRE
// ============================================================================

/**
 * @brief Exibe tabela formatada com todos os componentes da mochila
 * @param arr Array de componentes a exibir
 * @param tamanho Número de elementos a mostrar
 *
 * Formato da Tabela:
 * | NOME | TIPO | QUANTIDADE | PRIORIDADE |
 * Colunas com largura fixa para melhor legibilidade
 * Inclui separadores visuais (linhas de traço)
 */
void mostrarComponentes(Componente arr[], int tamanho) {
    // Validação: se a mochila está vazia
    if(tamanho == 0) {
        printf("\n[RELATORIO] Nenhum componente cadastrado.\n");
        return;
    }

    // Cabeçalho da tabela
    printf("\n--- INVENTARIO ATUAL (%d/10) ---\n", tamanho);
    printf("-------------------------------------------------------------------\n");
    printf("%-20s | %-15s | %-11s | %-10s\n", "NOME", "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("-------------------------------------------------------------------\n");

    // Linha para cada componente (iteração O(n))
    for(int i = 0; i < tamanho; i++)
        printf("%-20s | %-15s | %-11d | %-10d\n", arr[i].nome, arr[i].tipo, arr[i].quantidade, arr[i].prioridade);

    printf("-------------------------------------------------------------------\n");
}

/**
 * @brief Bubble Sort - Ordena componentes alfabeticamente por nome
 * @param arr Array de componentes a ordenar (MODIFICADO in-place)
 * @param tamanho Número de elementos
 *
 * Algoritmo:
 * - Compara elementos adjacentes
 * - Toca se forem maiores (ordem alfabética)
 * - Repete até que nenhuma troca seja necessária
 * - Após ordenação: elemento maior "flutua" para o final
 *
 * Complexidade:
 * - Melhor caso: O(n) - array já ordenado (com otimização)
 * - Pior caso: O(n²) - array em ordem reversa
 * - Comparações: sempre ~n(n-1)/2
 *
 * Necessário para: Busca Binária (requer array ordenado)
 */
void bubbleSortNome(Componente arr[], int tamanho) {
    if(tamanho == 0)
        return;

    Componente temp;
    int trocas = 0;
    comparacoesVetor = 0;

    // Loop externo: n-1 iterações
    for(int i = 0; i < tamanho - 1; i++) {
        // Loop interno: compara elementos adjacentes (reduz a cada iteração)
        for(int j = 0; j < tamanho - i - 1; j++) {
            comparacoesVetor++;
            // Comparação alfabética: strcmp retorna > 0 se arr[j] > arr[j+1]
            if(strcmp(arr[j].nome, arr[j + 1].nome) > 0) {
                // Troca: arr[j] <-> arr[j+1]
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                trocas++;
            }
        }
    }
    printf("\n[BUBBLE SORT] Ordenacao por NOME concluida.\n");
    printf("-> Comparacoes: %d | Trocas: %d\n", comparacoesVetor, trocas);
}

/**
 * @brief Insertion Sort - Ordena componentes alfabeticamente por tipo
 * @param arr Array de componentes a ordenar (MODIFICADO in-place)
 * @param tamanho Número de elementos
 *
 * Algoritmo:
 * - Considera primeiro elemento já "ordenado"
 * - Para cada elemento subsequente, insere na posição correta
 * - Desloca elementos maiores para a direita
 *
 * Complexidade:
 * - Melhor caso: O(n) - array já ordenado
 * - Pior caso: O(n²) - array em ordem reversa
 * - Comparações: ~n(n-1)/4 em média
 *
 * Vantagem: Eficiente para dados parcialmente ordenados
 */
void insertionSortTipo(Componente arr[], int tamanho) {
    if(tamanho == 0)
        return;

    Componente chave;      // Elemento sendo inserido
    int deslocamentos = 0; // Contador de elementos movidos
    comparacoesVetor = 0;

    // Começa do segundo elemento (índice 1)
    for(int i = 1; i < tamanho; i++) {
        chave = arr[i]; // Elemento a inserir
        int j = i - 1;  // Índice para verificar elementos anteriores

        // Move elementos maiores para a direita
        while(j >= 0) {
            comparacoesVetor++;
            // Comparação alfabética por tipo (strcmp)
            if(strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j + 1] = arr[j]; // Desloca para direita
                j--;
                deslocamentos++;
            } else {
                break; // Encontrou posição correta
            }
        }
        arr[j + 1] = chave; // Insere elemento na posição correta
    }
    printf("\n[INSERTION SORT] Ordenacao por TIPO concluida.\n");
    printf("-> Comparacoes: %d | Deslocamentos: %d\n", comparacoesVetor, deslocamentos);
}

/**
 * @brief Selection Sort - Ordena componentes por prioridade (decrescente)
 * @param arr Array de componentes a ordenar (MODIFICADO in-place)
 * @param tamanho Número de elementos
 *
 * Algoritmo:
 * - Encontra o elemento máximo na porção não-ordenada
 * - Coloca na posição correta
 * - Repete para a porção restante (reduz a cada iteração)
 *
 * Complexidade:
 * - Todas as variações: O(n²)
 * - Comparações: sempre n(n-1)/2
 * - Trocas: no máximo n-1
 *
 * Características:
 * - Não é "estável" (pode mudar ordem relativa de iguais)
 * - Simples de implementar
 * - Bom para dados pequenos ou análise educacional
 *
 * Nota: Ordena em ORDEM DECRESCENTE (maior prioridade primeiro)
 */
void selectionSortPrioridade(Componente arr[], int tamanho) {
    if(tamanho == 0)
        return;

    int minIdx; // Índice do elemento máximo na porção não-ordenada
    Componente temp;
    int trocas = 0;
    comparacoesVetor = 0;

    // Percorre array selecionando máximo em cada iteração
    for(int i = 0; i < tamanho - 1; i++) {
        minIdx = i; // Assume primeiro da porção não-ordenada como máximo

        // Busca o máximo na porção não-ordenada
        for(int j = i + 1; j < tamanho; j++) {
            comparacoesVetor++;
            // Busca prioridade MAIOR (ordem DECRESCENTE)
            if(arr[j].prioridade > arr[minIdx].prioridade) {
                minIdx = j; // Atualiza índice do máximo
            }
        }

        // Troca se encontrou máximo diferente
        if(minIdx != i) {
            temp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = temp;
            trocas++;
        }
    }
    printf("\n[SELECTION SORT] Ordenacao por PRIORIDADE concluida.\n");
    printf("-> Comparacoes: %d | Trocas: %d\n", comparacoesVetor, trocas);
}

/**
 * @brief Busca Binária - Localiza componente por nome em O(log n)
 * @param arr Array ORDENADO alfabeticamente por nome (bubbleSortNome)
 * @param tamanho Quantidade de elementos
 * @param nome Nome do componente a buscar (string terminada em '\0')
 * @return Índice do elemento encontrado, ou -1 se não encontrado
 *
 * PRÉ-CONDIÇÃO: Array DEVE estar ordenado por nome!
 *               Use bubbleSortNome() antes de chamar esta função
 *
 * Algoritmo:
 * - Mantém intervalo [inicio, fim] onde elemento pode estar
 * - Calcula meio = (inicio + fim) / 2
 * - Compara arr[meio] com nome buscado
 * - Reduz intervalo pela metade a cada iteração
 *
 * Complexidade:
 * - Tempo: O(log n) - máximo log₂(n) iterações
 * - Comparações: máximo ~log₂(n) + 1
 * - Espaço: O(1) - não usa recursão (iterativo)
 *
 * Exemplo: Para 1024 elementos, máximo 10 comparações
 *
 * Telemetria: Incrementa comparacoesVetor para análise
 */
int buscaBinariaPorNome(Componente arr[], int tamanho, char nome[]) {
    if(tamanho == 0) {
        return -1; // Array vazio
    }

    int inicio = 0, fim = tamanho - 1;
    comparacoesVetor = 0; // Reset contador de comparações

    // Enquanto houver intervalo válido
    while(inicio <= fim) {
        // Calcula índice do meio (evita overflow)
        int meio = inicio + (fim - inicio) / 2;
        comparacoesVetor++;

        // Compara nome no meio com nome buscado
        int comp = strcmp(arr[meio].nome, nome);

        if(comp == 0) {
            // ENCONTRADO: retorna índice
            return meio;
        }
        if(comp < 0)
            // Nome buscado é maior: busca na metade direita
            inicio = meio + 1;
        else
            // Nome buscado é menor: busca na metade esquerda
            fim = meio - 1;
    }

    // NÃO ENCONTRADO
    return -1;
}

/**
 * @brief Mede o tempo de CPU gasto em um algoritmo de ordenação
 * @param algoritmo Ponteiro para função de ordenação (bubbleSortNome, insertionSortTipo ou selectionSortPrioridade)
 * @param arr Array de componentes (será ordenado)
 * @param tamanho Número de elementos
 * @param descricao String descritiva do algoritmo (para exibição: "BUBBLE SORT - Por Nome")
 *
 * Objetivo:
 * - Executar algoritmo de ordenação
 * - Medir tempo decorrido usando clock() do <time.h>
 * - Converter de ticks para segundos
 * - Exibir resultado formatado
 *
 * Detalhes:
 * - Usa ponteiro para função (function pointer) para flexibilidade
 * - clock() retorna ticks desde início do programa
 * - CLOCKS_PER_SEC converte ticks para segundos
 * - Tempo é em CPU (não real), útil para comparação
 *
 * Formato de Saída:
 * === INICIANDO: ALGORITMO ===
 * [Executa: bubbleSortNome() imprime própria telemetria]
 * Tempo de CPU: X.XXXXXX segundos
 * =================================
 */
void medirTempo(void (*algoritmo)(Componente[], int), Componente arr[], int tamanho, const char descricao[]) {
    clock_t inicio, fim; // Tipo para medir ticks do relógio
    double tempo_cpu;    // Tempo em segundos (float/double)

    // Exibe cabeçalho
    printf("\n=== INICIANDO: %s ===\n", descricao);

    // Marca tempo ANTES de executar
    inicio = clock();

    // EXECUTA O ALGORITMO (função apontada em algoritmo)
    // O algoritmo imprime sua própria telemetria (comparações, trocas)
    algoritmo(arr, tamanho);

    // Marca tempo DEPOIS de executar
    fim = clock();

    // Calcula tempo decorrido em segundos
    // (fim - inicio) = número de ticks
    // Divide por CLOCKS_PER_SEC para converter em segundos
    tempo_cpu = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    // Exibe resultado (6 casas decimais para precisão)
    printf("Tempo de CPU: %.6f segundos\n", tempo_cpu);
    printf("=================================\n");
}