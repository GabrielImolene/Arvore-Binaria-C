#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

// Estrutura do nó da árvore binária
typedef struct No {
    char palavra[100];
    struct No* esquerda;
    struct No* direito;
} No;

// Cria um novo nó
No* criaNo(char* palavra) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo) {
        strcpy(novo->palavra, palavra);
        novo->esquerda = NULL;
        novo->direito = NULL;
    }
    return novo;
}

// Insere uma palavra na árvore
No* inseriNo(No* raiz, char* palavra) {
    if (raiz == NULL) {
        return criaNo(palavra);
    }
    if (strcmp(palavra, raiz->palavra) < 0) {
        raiz->esquerda = inseriNo(raiz->esquerda, palavra);
    } else if (strcmp(palavra, raiz->palavra) > 0) {
        raiz->direito = inseriNo(raiz->direito, palavra);
    }
    return raiz;
}

// Insere apenas palavras únicas
int inserirUnico(No** raiz, char* palavra) {
    if (*raiz == NULL) {
        *raiz = criaNo(palavra);
        return 1;
    }

    int cmp = strcmp(palavra, (*raiz)->palavra);
    if (cmp < 0) {
        return inserirUnico(&(*raiz)->esquerda, palavra);
    } else if (cmp > 0) {
        return inserirUnico(&(*raiz)->direito, palavra);
    }

    return 0; // Palavra duplicada
}

// Busca uma palavra na árvore
bool busca(No* raiz, char* palavra) {
    if (raiz == NULL) {
        return false;
    }
    int cmp = strcmp(palavra, raiz->palavra);
    if (cmp == 0) {
        return true;
    } else if (cmp < 0) {
        return busca(raiz->esquerda, palavra);
    } else {
        return busca(raiz->direito, palavra);
    }
}

// Busca especializada (por trecho)
void buscaEspecializada(No* raiz, char* trecho) {
    if (raiz != NULL) {
        buscaEspecializada(raiz->esquerda, trecho);
        if (strstr(raiz->palavra, trecho) != NULL) {
            printf("- %s\n", raiz->palavra);
        }
        buscaEspecializada(raiz->direito, trecho);
    }
}

// Remove uma palavra da árvore
void removeNo(No** raiz, char* palavra) {
    if (*raiz == NULL) return;

    int cmp = strcmp(palavra, (*raiz)->palavra);
    if (cmp < 0) {
        removeNo(&(*raiz)->esquerda, palavra);
    } else if (cmp > 0) {
        removeNo(&(*raiz)->direito, palavra);
    } else {
        if ((*raiz)->esquerda == NULL) {
            No* temp = (*raiz)->direito;
            free(*raiz);
            *raiz = temp;
        } else if ((*raiz)->direito == NULL) {
            No* temp = (*raiz)->esquerda;
            free(*raiz);
            *raiz = temp;
        } else {
            No* temp = (*raiz)->direito;
            while (temp->esquerda != NULL) {
                temp = temp->esquerda;
            }
            strcpy((*raiz)->palavra, temp->palavra);
            removeNo(&(*raiz)->direito, temp->palavra);
        }
    }
}

// Imprime as palavras em ordem
void imprimeResultado(No* raiz) {
    if (raiz != NULL) {
        imprimeResultado(raiz->esquerda);
        printf("- %s\n", raiz->palavra);
        imprimeResultado(raiz->direito);
    }
}

// Libera a memória da árvore
void liberaNo(No* raiz) {
    if (raiz != NULL) {
        liberaNo(raiz->esquerda);
        liberaNo(raiz->direito);
        free(raiz);
    }
}

// Conta o número total de palavras na árvore
int contarNos(No* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarNos(raiz->esquerda) + contarNos(raiz->direito);
}

// Padroniza uma palavra
void padronizacao(char* palavra) {
    int i, j = 0;
    char temp[strlen(palavra) + 1];
    for (i = 0; palavra[i] != '\0'; i++) {
        char c = palavra[i];
        if (strchr("áàâãä", c)) c = 'a';
        else if (strchr("éèêë", c)) c = 'e';
        else if (strchr("íìîï", c)) c = 'i';
        else if (strchr("óòôõö", c)) c = 'o';
        else if (strchr("úùûü", c)) c = 'u';
        else if (c == '\xE7') c = 'c'; // Unicode para 'ç'
        if (isalnum(c)) { // Verifica se é letra ou dígito
            temp[j++] = tolower(c);
        }
    }
    temp[j] = '\0';
    strcpy(palavra, temp);
}

// Calcula o tempo de busca
void calcularTempoBusca(No* raiz) {
    char palavra[100];
    printf("Digite a palavra para medir o tempo de busca: ");
    scanf("%s", palavra);
    padronizacao(palavra);

    clock_t inicio = clock();
    bool resultado = busca(raiz, palavra);
    clock_t fim = clock();

    double tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    if (resultado) {
        printf("Palavra '%s' encontrada em %.6f segundos.\n", palavra, tempoGasto);
    } else {
        printf("Palavra '%s' não encontrada (tempo: %.6f segundos).\n", palavra, tempoGasto);
    }
}

// Exibe um menu estilizado
void exibeMenu() {
    printf("\n=========================================\n");
    printf("   🌳 Bem-vindo ao Projeto C 🌳\n");
    printf("   ⌨️ Indexador de Texto ⌨️ \n");
    printf("=========================================\n\n");
    printf("1️⃣  Inserir uma palavra\n");
    printf("2️⃣  Buscar uma palavra\n");
    printf("3️⃣  Exibir palavras\n");
    printf("4️⃣  Remover uma palavra\n");
    printf("5️⃣  Busca especializada\n");
    printf("6️⃣  Calcular tempo de busca\n");
    printf("7️⃣  Sair\n");
    printf("\n");
    printf("=========================================\n");
    printf("👉 Escolha uma opção: ");
}

// Função principal (main)
int main() {
    No* raiz = NULL;
    int opcao;
    char palavra[100];

    do {
        exibeMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\n📝 Digite a palavra: ");
                scanf("%s", palavra);
                padronizacao(palavra);
                if (strlen(palavra) == 0) {
                    printf("⚠️ Palavra inválida! Certifique-se de que ela contém apenas letras ou números.\n");
                } else if (inserirUnico(&raiz, palavra)) {
                    printf("✅ Palavra '%s' inserida com sucesso!\n", palavra);
                } else {
                    printf("⚠️ Palavra '%s' já existe na árvore!\n", palavra);
                }
                printf("📊 Total de palavras na árvore: %d\n", contarNos(raiz));
                break;
            case 2:
                printf("\n🔍 Digite a palavra que deseja buscar: ");
                scanf("%s", palavra);
                padronizacao(palavra);
                if (busca(raiz, palavra)) {
                    printf("✅ Palavra '%s' encontrada!\n", palavra);
                } else {
                    printf("❌ Palavra '%s' não encontrada!\n", palavra);
                }
                break;
            case 3:
                printf("\n📂 Palavras armazenadas (ordem alfabética):\n");
                if (raiz == NULL) {
                    printf("🌱 Árvore vazia.\n");
                } else {
                    imprimeResultado(raiz);
                }
                break;
            case 4:
                printf("\n🗑️ Digite a palavra que deseja remover: ");
                scanf("%s", palavra);
                padronizacao(palavra);
                removeNo(&raiz, palavra);
                printf("✅ Palavra '%s' removida (se existia)!\n", palavra);
                printf("📊 Total de palavras na árvore: %d\n", contarNos(raiz));
                break;
            case 5:
                printf("\n🔍 Digite o trecho que deseja buscar: ");
                scanf("%s", palavra);
                padronizacao(palavra);
                printf("\n📂 Palavras contendo o trecho '%s':\n", palavra);
                buscaEspecializada(raiz, palavra);
                break;
            case 6:
                calcularTempoBusca(raiz);
                break;
            case 7:
                printf("\n👋 Saindo... Obrigado por usar o programa!\n");
                break;
            default:
                printf("❌ Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 7);

    liberaNo(raiz);
    return 0;
}
