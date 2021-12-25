#if !defined(VECTOR3_D)
#define VECTOR3_D

typedef struct {
	float x;
	float y;
	float z;
} v3;

v3 *v3AddV3(v3 *a, v3 *b) {
	v3 *newV3 = malloc(sizeof(v3));
	newV3->x = a->x + b->x;
	newV3->y = a->y + b->y;
	newV3->z = a->z + b->z;
	return newV3;
}

void printV3(v3 *vector) {
	printf("[x: %f, y: %f, z: %f]\n", vector->x, vector->y, vector->z);
}

#endif // VECTOR3_D
