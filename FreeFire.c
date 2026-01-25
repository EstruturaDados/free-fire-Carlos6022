#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file desafio_mochila.c
 * @brief Sistema de inventário de sobrevivência para gerenciamento de itens (loot).
 * @details Este programa utiliza listas sequenciais (vetores de structs) para permitir
 * que um jogador cadastre, remova, liste e busque itens em sua mochila.
 */

#define TAM_MAX 10
#define MAX_STR_LEN 30

/**
 * @struct Item
 * @brief Estrutura que define as propriedades de um item coletado.
 * @var Item::nome Nome descritivo do item.
 * @var Item::tipo Categoria do item (ex: arma, munição, cura).
 * @var Item::quantidade Quantidade de unidades daquele item.
 */
typedef struct {
    char nome[MAX_STR_LEN];
    char tipo[20];
    int quantidade;
} Item;

/**
 * @struct Mochila
 * @brief Estrutura que representa o inventário (mochila) do jogador.
 * @var Mochila::itens Vetor estático que armazena os objetos do tipo Item.
 * @var Mochila::quantidadeAtual Contador que controla quantos slots estão ocupados.
 */
typedef struct {
    Item itens[TAM_MAX];
    int quantidadeAtual;
} Mochila;

// --- Protótipos das Funções ---
void inicializarMochila(Mochila *mochila);
void inserirItem(Mochila *mochila, Item novoItem);
void removerItem(Mochila *mochila, const char *nome);
void listarItens(const Mochila *mochila);
void buscarItem(const Mochila *mochila, const char *nome);
void limparBuffer();

int main() {
    Mochila minhaMochila;
    inicializarMochila(&minhaMochila);

    int opcao;
    char nomeBusca[MAX_STR_LEN];
    Item tempItem;

    do {
        printf("==================================================");
        printf("\n    MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA   \n");
        printf("==================================================\n");
        printf("Itens na mochila: %d/%d\n\n", minhaMochila.quantidadeAtual, TAM_MAX);
        printf("1. Adicionar Item (Loot)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens na Mochila\n");
        printf("4. Buscar Item por Nome\n");
        printf("0. Sair\n");
        printf("--------------------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
        case 1:
            printf("\n--- ADICIONAR NOVO ITEM ---\n");
            printf("nome do item: ");
            fgets(tempItem.nome, MAX_STR_LEN, stdin);
            if(tempItem.nome[strlen(tempItem.nome) - 1] == '\n') {
                tempItem.nome[strlen(tempItem.nome) - 1] = '\0';
            }

            printf("tipo do item(arma, munição, cura, etc.): ");
            fgets(tempItem.tipo, 20, stdin);
            if(tempItem.tipo[strlen(tempItem.tipo) - 1] == '\n') {
                tempItem.tipo[strlen(tempItem.tipo) - 1] = '\0';
            }

            printf("Quantidade: ");
            scanf("%d", &tempItem.quantidade);
            limparBuffer();

            inserirItem(&minhaMochila, tempItem);
            listarItens(&minhaMochila);
            break;

        case 2:
            printf("Digite o nome do item a remover: ");
            fgets(nomeBusca, MAX_STR_LEN, stdin);
            if(nomeBusca[strlen(nomeBusca) - 1] == '\n') {
                nomeBusca[strlen(nomeBusca) - 1] = '\0';
            }
            removerItem(&minhaMochila, nomeBusca);
            listarItens(&minhaMochila); // Listar itens após remoção
            break;

        case 3:
            listarItens(&minhaMochila);
            break;

        case 4:
            printf("Digite o nome do item a buscar: ");
            fgets(nomeBusca, MAX_STR_LEN, stdin);
            if(nomeBusca[strlen(nomeBusca) - 1] == '\n') {
                nomeBusca[strlen(nomeBusca) - 1] = '\0';
            }
            buscarItem(&minhaMochila, nomeBusca);
            break;

        case 0:
            printf("Saindo do programa...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }

        if(opcao != 0) {
            printf("\n[SISTEMA]: Pressione [ENTER] para voltar ao menu...");
            getchar();
        }

    } while(opcao != 0);

    return 0;
}

// --- IMPLEMENTAÇÕES DAS FUNÇÕES ---

/**
 * @brief Limpa o buffer de entrada do teclado (stdin).
 * @details Remove caracteres residuais (como o '\n') que podem causar erros em leituras posteriores.
 */
void limparBuffer() {
    int c;
    while((c = getchar()) != '\n' && c != EOF)
        ;
}

/**
 * @brief Inicializa o estado da mochila.
 * @param mochila Ponteiro para a estrutura que sera zerada.
 */
void inicializarMochila(Mochila *mochila) {
    mochila->quantidadeAtual = 0;
}

/**
 * @brief Insere um novo item na mochila.
 * @param mochila Ponteiro para a mochila onde o item sera inserido.
 * @param novoItem Estrutura contendo os dados do item coletado.
 * @note Caso a mochila esteja cheia (TAM_MAX), a insercao e abortada.
 */
void inserirItem(Mochila *mochila, Item novoItem) {

    if(mochila->quantidadeAtual == TAM_MAX) {
        printf("Mochila cheia! Não é possível inserir mais itens.\n");
        return;
    }

    // Copia a struct inteira para a posição do vetor
    mochila->itens[mochila->quantidadeAtual] = novoItem;
    mochila->quantidadeAtual++;
    printf("\nItem '%s' inserido na mochila com sucesso.\n", novoItem.nome);
}

/**
 * @brief Realiza uma busca sequencial por um item pelo nome.
 * @param mochila Ponteiro constante para a mochila (apenas leitura).
 * @param nome String contendo o nome do item a ser localizado.
 */
void buscarItem(const Mochila *mochila, const char *nome) {
    for(int i = 0; i < mochila->quantidadeAtual; i++) {
        if(strcmp(mochila->itens[i].nome, nome) == 0) {
            printf("\n[ITEM ENCONTRADO]\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila->itens[i].nome, mochila->itens[i].tipo, mochila->itens[i].quantidade);
            return;
        }
    }
    printf("\n[AVISO] Item '%s' nao encontrado.\n", nome);
}

/**
 * @brief Remove um item da mochila e reorganiza o vetor.
 * @param mochila Ponteiro para a mochila de onde o item sera removido.
 * @param nome Nome do item que deve ser excluido.
 * @details Localiza o item e desloca os itens subsequentes para a esquerda para manter a lista contigua.
 */
void removerItem(Mochila *mochila, const char *nome) {
    int i, pos = -1;

    for(i = 0; i < mochila->quantidadeAtual; i++) {
        if(strcmp(mochila->itens[i].nome, nome) == 0) {
            pos = i;
            break;
        }
    }

    if(pos == -1) {
        printf("Erro: Item '%s' não encontrado na mochila.\n", nome);
        return;
    }

    for(i = pos; i < mochila->quantidadeAtual - 1; i++) {
        mochila->itens[i] = mochila->itens[i + 1];
    }

    mochila->quantidadeAtual--;
    printf("\nItem '%s' removido da mochila com sucesso.\n", nome);
}

/**
 * @brief Exibe todos os itens armazenados na mochila em formato de tabela.
 * @param mochila Ponteiro constante para a mochila (apenas leitura).
 */
void listarItens(const Mochila *mochila) {
    if(mochila->quantidadeAtual == 0) {
        printf("\nA mochila esta vazia.\n");
        return;
    }

    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", mochila->quantidadeAtual, TAM_MAX);
    printf("-------------------------------------------------------------------\n");
    printf("%-25s | %-20s | %-10s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("-------------------------------------------------------------------\n");

    for(int i = 0; i < mochila->quantidadeAtual; i++) {
        printf("%-25s | %-20s | %-10d\n",
               mochila->itens[i].nome,
               mochila->itens[i].tipo,
               mochila->itens[i].quantidade);
    }
    printf("-------------------------------------------------------------------\n");
}