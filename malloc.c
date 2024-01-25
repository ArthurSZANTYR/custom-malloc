#include <unistd.h> // Pour sbrk()
#include <stdio.h>  // Pour printf()
#include <stdbool.h> 

typedef struct BlockHeader {
    size_t size;
    bool isFree;
    struct BlockHeader* next;  //déclare un pointeur appelé next vers une structure BlockHeader //le block suivant
} BlockHeader;

BlockHeader* head = NULL; //début de la liste chaînée des blocs alloués

void* malloc(size_t size) {   //void*   pointeur -- qui peut pointer vers n'importe quel type de données
    size_t totalSize;
    void* block;
    BlockHeader* header;

    totalSize = size + sizeof(BlockHeader); //taille totale : taille demandée + taille de l'en-tête

    block = sbrk(totalSize); //allocation mémoire  //adresse de debut de la memoire allouée 

    if (block == NULL) { // Vérifie si sbrk a échoué
    return NULL;
    }

    // Configuration de l'en-tête du bloc
    header = block;      //adresse de debut du bloc 
    header->size = size;
    header->isFree = false;
    header->next = NULL;

    if (head == NULL) {  //pour le premier bloc
        head = header;  
    } else {  // sinon ajout du nouveau bloc à la fin de la liste chaînée
        BlockHeader* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = header;
    }

    return (void*)(header + 1); // Retourne le pointeur vers le début de la zone mémoire utilisable, après l'en-tête
                                 //deplace le pointeur de sizeof(BlockHeader) + 1 octets vers l'avant  //donc espace juste aprés l'entete
}

void free(void* block) {  //arg > pointeur vers le bloc a libérer
    BlockHeader* header;
    void* blockStart;

    header = (BlockHeader*)block - 1; //pour retrouver l'en-tête du bloc

    header->isFree = true;

    // Fusion simple avec le bloc suivant si c'est libre - pour défragmenter la heap 
    if (header->next && header->next->isFree) {
        header->size += header->next->size + sizeof(BlockHeader);
        header->next = header->next->next;  //next changé pour le i+2 //le prochain bloc  //NULL car dernier bloc de liste chainée forcement 
    }
}

int main() {
    int *array = malloc(10 * sizeof(int));
    
    if (array == NULL) {
        printf("Échec de l'allocation de mémoire.\n");
    } else {
        for (int i = 0; i < 10; i++) {
            array[i] = i;
        }
        
        printf("Valeurs du tableau : ");
        for (int i = 0; i < 10; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        free(array);
    }
    
    return 0;
}


