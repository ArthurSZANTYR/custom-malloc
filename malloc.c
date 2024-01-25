#include <unistd.h> // Pour sbrk()
#include <stdio.h> // Pour printf()

void *malloc(size_t size) {
    void *block;

    // Allocation de mémoire en utilisant sbrk.
    block = sbrk(size);

    // Vérifie si sbrk a échoué
    if (block == (void*) -1) {
        return NULL; // Échec de l'allocation
    }

    // Retourne le pointeur vers le nouveau bloc de mémoire
    return block;
}

int main() {
    // Allouer de la mémoire pour un tableau de 10 entiers
    int *array = malloc(10 * sizeof(int));
    
    // Vérifier si l'allocation a réussi
    if (array == NULL) {
        printf("Échec de l'allocation de mémoire.\n");
    }

    for (int i = 0; i < 10; i++) {
        array[i] = i;
    }
    
    printf("Valeurs du tableau : ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    
    return 0;
}

