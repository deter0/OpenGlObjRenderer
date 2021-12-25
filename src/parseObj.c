#if !defined(readFile)
#define readFile

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "v3Array.c"

typedef struct {
	const char *filePath;
	unsigned long fileSize;

	v3Array *vertices;
	v3Array *vertexNormals;
	v3Array *uvs;
	v3Array_2D *faces;

} objFile;

/* For some reason it was not defined in `string.h` for me */
char *strsep(char **restrict stringp, const char *restrict delim);

objFile* readObjFile(const char *filePath) {
	objFile *file = malloc(sizeof(objFile));
	FILE *fp = fopen(filePath, "rb");

	if (!fp) {
		perror("Something went wrong opening file.\n");
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	file->fileSize = ftell(fp);
	rewind(fp);
	
	char *fileContents = calloc(sizeof(char), file->fileSize + 1);
	if (!fileContents) {
		perror("Out of memory!");
		return NULL;
	}
	if (fread(fileContents, file->fileSize, 1, fp) != 1) {
		fclose(fp);
		free(fileContents);
		perror("Error reading file\n");
		return NULL;
	}
	fclose(fp);

	char *line = strtok(fileContents, "\n");
	char *instruction;
	
	#define INITIAL_OBJ_PARSE_SIZE 30
	
	v3Array *vertices = malloc(sizeof(v3Array));
	v3Array *vertexNormals = malloc(sizeof(v3Array));
	v3Array *uvs = malloc(sizeof(v3Array));
	v3Array_2D *faces = malloc(sizeof(v3Array_2D));
	initV3Array(vertices, INITIAL_OBJ_PARSE_SIZE);
	initV3Array(uvs, INITIAL_OBJ_PARSE_SIZE * 2);
	initV3Array(vertexNormals, INITIAL_OBJ_PARSE_SIZE / 2);
	initV3Array_2D(faces, INITIAL_OBJ_PARSE_SIZE);
	
	char *objectName = NULL;
	while (line) { // TODO(deter): More than one object;
		instruction = strsep(&line, " ");
		if (!instruction)
			goto nextLine;
		
		/* @https://en.wikipedia.org/wiki/Wavefront_.obj_file#File_format */
		if (instruction[0] == '#') {
			goto nextLine;
		} else if (strcmp(instruction, "o") == 0) {
			if (objectName != NULL) {
				printf("WARNING NO SUPPORT FOR MULTIPLE OBJECTS IN OBJ FILES, YET.\n");
				break;
			} else {
				objectName = strsep(&line, " ");
			}
		} else if (strcmp(instruction, "v") == 0 || strcmp(instruction, "vn") == 0) { /* vertices & vertex normals */
			const char *char_x = strsep(&line, " ");
			const char *char_y = strsep(&line, " ");
			const char *char_z = strsep(&line, " ");
			assert(char_x != NULL && char_y != NULL && char_z != NULL);
			float x = atof(char_x);
			float y = atof(char_y);
			float z = atof(char_z);
			v3 *newV3 = malloc(sizeof(v3));
			if (!newV3) {
				perror("Error allocating memory for Vector3. Aborting.");
				exit(1);
			}
			// printf("Created vertice: %f, %f, %f.\n", x, y, z);
			newV3->x = x;
			newV3->y = y;
			newV3->z = z;
			
			if (strcmp(instruction, "v") == 0) {
				// printf("Created vertex: ");
				// printV3(newV3);
				insertIntoV3Array(vertices, newV3); /* vertices */
			} else {
				insertIntoV3Array(vertexNormals, newV3); /* vertex normals */
			}
			goto nextLine; // no W support.
		} else if (strcmp(instruction, "f") == 0) { /* faces */
			char *faceData = strsep(&line, " ");
			v3Array *faceDataArr = malloc(sizeof(v3Array));
			initV3Array(faceDataArr, INITIAL_OBJ_PARSE_SIZE); // ! MIGHT BE UNSAFE
			
			while (faceData != NULL) {
				const char *vertexIndex_char = strsep(&faceData, "/");
				const char *textureCoordIndex_char = strsep(&faceData, "/");
				const char *normalIndex_char = strsep(&faceData, "/");
				int vertexIndex = atoi(vertexIndex_char) - 1;
				int textureCoordIndex = atoi(textureCoordIndex_char);
				int normalIndex = atoi(normalIndex_char); // TODO(deter): Check if these exist
				v3 *faceDataV3 = malloc(sizeof(v3));
				if (faceDataV3 == NULL) {
					perror("Error allocating face data. Aborting.\n");
					exit(1);
				}
				faceDataV3->x = (float)vertexIndex;
				faceDataV3->y = (float)textureCoordIndex;
				faceDataV3->z = (float)normalIndex;
				// printf("Created face and inserted into face data array. %f, %f, %f.\n", faceDataV3->x, faceDataV3->y, faceDataV3->z);
				insertIntoV3Array(faceDataArr, faceDataV3);
				faceData = strsep(&line, " ");
			}
			// printf("Inserted face data array into faces array. Size = %d\n", faceDataArr->used);
			insertIntoV3Array_2D(faces, faceDataArr);
			// printf("2D Array Size: %d\n", faces->used);
			// printf("First: %f\n", faces->array[0]->array[0]->x);
		} else if (strcmp(instruction, "vt") == 0) { /* uvs */
			const char *char_x = strsep(&line, " ");
			const char *char_y = strsep(&line, " ");
			const char *char_w_op = strsep(&line, " ");
			assert(char_x != NULL && char_y != NULL);
			float x = atof(char_x);
			float y = atof(char_y);
			float w = 0; // z in vector w in uvs since uvs are 2d? idk. default is 0
			if (char_w_op != NULL) {
				w = atof(char_w_op);
			}
			v3 *newUv = malloc(sizeof(v3));
			if (newUv == NULL) {
				perror("Error allocating memory for Vector3. Aborting.");
				exit(1);
			}
			newUv->x = x;
			newUv->y = y;
			newUv->z = w;
			insertIntoV3Array(uvs, newUv);
		}

		nextLine:
		line = strtok(NULL, "\n");
	}
	printf("LOADED %s [VERTICES: %d, FACES: %d, UVS: %d, VERTEX NORMALS: %d]\n", objectName, vertices->used, faces->used, uvs->used, vertexNormals->used);
	free(line);
	file->vertices = vertices;
	file->vertexNormals = vertexNormals;
	file->uvs = uvs;
	file->faces = faces;
	
	return file;
}


#endif // readFile
