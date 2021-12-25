#if !defined(Array_D)
#define Array_D

#if !defined(size_t)
#define size_t int
#endif // size_t

#include <stdio.h>
#include <stdlib.h>
#include "vector3.c"

typedef struct {
	v3 **array;
	size_t used;
	size_t size;
} v3Array;

void initV3Array(v3Array *a, size_t initialSize) {
	a->array = calloc(sizeof(v3), initialSize);
	if (!a->array) {
		perror("Error allocating memory while initing array\n");
		exit(1);
	}
	a->used = 0;
	a->size = initialSize;
}

void insertIntoV3Array(v3Array *a, v3 *item) {
	if (a->used >= a->size) {
		a->size *= 2;
		a->array = realloc(a->array, a->size * sizeof(v3));
		printf("Reallocating array [+].\n");
		if (!a->array) {
			perror("Error rellocating memory when adding item to array. Aborting.\n");
			exit(1);
		}
	}
	a->array[a->used++] = item;
}

void freeV3Array(v3Array *a) {
	for (long i = 0; i < a->used; i++) {
		free(a->array[i]);
	}
	free(a->array);
	a->size = 0;
	a->used = 0;
	free(a);
}

typedef struct {
	v3Array **array;
	size_t used;
	size_t size;
} v3Array_2D;

void initV3Array_2D(v3Array_2D *a, size_t initialSize) {
	a->array = calloc(sizeof(v3Array*), initialSize);
	if (!a->array) {
		perror("Error allocating memory while initing array\n");
		exit(1);
	}
	a->used = 0;
	a->size = initialSize;
}

void insertIntoV3Array_2D(v3Array_2D *a, v3Array *items) {
	if (a->used >= a->size) {
		a->size *= 2;
		a->array = realloc(a->array, a->size * sizeof(v3));
		printf("Reallocating array [+].\n");
		if (!a->array) {
			perror("Error rellocating memory when adding item to array. Aborting.\n");
			exit(1);
		}
	}
	a->array[a->used++] = items;
}

void addToV3Array_2DAtI(v3Array_2D *a, int index, v3Array *items) {
	while (index < a->used) {
		a->size *= 2;
		a->array = realloc(a->array, a->size * sizeof(v3Array*));
		printf("Reallocating array [+].\n");
		if (!a->array) {
			perror("Error rellocating memory when adding item to array. Aborting.\n");
			exit(1);
		}
	}
	a->array[index] = items;
}

void freeV3Array_2D(v3Array_2D *a) {
	for (long i = 0; i < a->used; i++) {
		for (int j = 0; j < a->array[i]->used; j++) {
			free(a->array[i]->array[j]);
		}
		free(a->array[i]);
	}
	free(a->array);
	a->size = 0;
	a->used = 0;
	free(a);
}


#endif // Array_D
