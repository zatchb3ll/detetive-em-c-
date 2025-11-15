/*Bibliotecas*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*Permite Abstração*/
typedef struct Sala {
    char nome[100];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

/*
 *Aloca Dinamicamente
 * 
 * Parâmetro: nome - string com o nome da sala
 * Retorno: ponteiro para a nova sala criada
 * 
 * Um novo nó é criado pela função, que aloca a memória, copia o nome da sala e inicializa seus apontadores dos filhos como NULL.
 */
Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para nova sala.\n");
        exit(EXIT_FAILURE);
    }
    
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0';
    
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/*
 * Permite o Passeio
 * 
 * Parâmetro: sala - apontador para a sala atual
 * 
 * Exibe as funções
 * 
 * A exploração continua até o jogador atingir uma sala-folha
 * (sem caminhos à esquerda nem à direita) ou escolher sair.
 */
void explorarSalas(Sala* sala) {
    if (sala == NULL) {
        printf("\nVocê chegou ao fim de um corredor. Não há mais caminhos!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("Você está em: %s\n", sala->nome);
    printf("========================================\n");
    
    if (sala->esquerda == NULL && sala->direita == NULL) {
        printf("Esta é uma sala sem saídas. Você alcançou um beco sem saída!\n");
        return;
    }
    
    char escolha;
    int validoEsquerda = (sala->esquerda != NULL);
    int validoDireita = (sala->direita != NULL);
    
    printf("\nEscolha uma direção:\n");
    if (validoEsquerda) {
        printf("  (e) Ir para a esquerda\n");
    }
    if (validoDireita) {
        printf("  (d) Ir para a direita\n");
    }
    printf("  (s) Sair da exploração\n");
    printf("\nDigite sua escolha: ");
    
    scanf(" %c", &escolha);
    
    switch (escolha) {
        case 'e':
        case 'E':
            if (validoEsquerda) {
                explorarSalas(sala->esquerda);
            } else {
                printf("\nNão há caminho à esquerda. Tente outra direção.\n");
                explorarSalas(sala);
            }
            break;
            
        case 'd':
        case 'D':
            if (validoDireita) {
                explorarSalas(sala->direita);
            } else {
                printf("\nNão há caminho à direita. Tente outra direção.\n");
                explorarSalas(sala);
            }
            break;
            
        case 's':
        case 'S':
            printf("\nVocê saiu da exploração. Até a próxima!\n");
            break;
            
        default:
            printf("\nOpção inválida! Digite 'e', 'd' ou 's'.\n");
            explorarSalas(sala);
            break;
    }
}

/*
 * liberarMansao() – Libera a memória alocada para toda a árvore
 * 
 * Parâmetro: sala - apontador para o nó raiz da árvore
 * 
 * Realiza limpeza recursiva da memória.
 */
void liberarMansao(Sala* sala) {
    if (sala != NULL) {
        liberarMansao(sala->esquerda);
        liberarMansao(sala->direita);
        free(sala);
    }
}

/*
 * main() – Monta o mapa
 * 
 * A função constrói uma árvore binária representando as salas da mansão,
 * com o Hall de Entrada como raiz. Em seguida, chama explorarSalas() para
 * iniciar a interação com o jogador. Finalmente, libera toda a memória
 * alocada através de liberarMansao().
 */
int main() {
    printf("╔════════════════════════════════════════╗\n");
    printf("║     BEM-VINDO AO DETECTIVE QUEST      ║\n");
    printf("║   Explore a Mansão e Encontre Pistas  ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    printf("Instruções: Use 'e' para ir à esquerda, 'd' para ir à direita,\n");
    printf("ou 's' para sair a qualquer momento.\n");
    printf("════════════════════════════════════════\n");
    
    Sala* hallEntrada = criarSala("Hall de Entrada");
    
    hallEntrada->esquerda = criarSala("Biblioteca");
    hallEntrada->direita = criarSala("Cozinha");
    
    hallEntrada->esquerda->esquerda = criarSala("Sala de Leitura");
    hallEntrada->esquerda->direita = criarSala("Arquivo Secreto");
    
    hallEntrada->direita->esquerda = criarSala("Despensa");
    hallEntrada->direita->direita = criarSala("Garagem");
    
    hallEntrada->esquerda->esquerda->esquerda = NULL;
    hallEntrada->esquerda->esquerda->direita = NULL;
    
    hallEntrada->esquerda->direita->esquerda = NULL;
    hallEntrada->esquerda->direita->direita = NULL;
    
    hallEntrada->direita->esquerda->esquerda = NULL;
    hallEntrada->direita->esquerda->direita = NULL;
    
    hallEntrada->direita->direita->esquerda = NULL;
    hallEntrada->direita->direita->direita = NULL;
    
    explorarSalas(hallEntrada);
    
    liberarMansao(hallEntrada);
    
    printf("\nPrograma finalizado. Memória liberada com sucesso.\n");
    return 0;
}
