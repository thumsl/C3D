#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

float get_number(int* i, char* buffer, float* x) {
    // reads characters from buffer until space, \n or \0 and gen number from them
    char str[16];
    int j;

    for (j = 0; buffer[*i] != ' ' && buffer[*i] != '\n' && buffer[*i] != '\0'; (*i)++)
        str[j++] = buffer[(*i)];

    str[j] = 0;
    if (buffer[*i] == ' ')
        (*i)++;

    *x = atof(str);

    if (buffer[*i] == '\n' || buffer[*i] == '\0')
        return 0;
    else
        return 1;
}

int loadOBJ(const char* filename, GLfloat* vertices, GLuint* indices, int* vertexCount, int* indexCount) {
    char* buffer;

    if (!readfile(&buffer, filename)) {
        fprintf(stderr, "There was an error reading file '%s'.\n", filename);
        return 0;
    }

    int i = 0, vn = 0, vt = 0, f = 0, v = 0;
    GLfloat v_temp[50000], vt_temp[50000];
    GLuint f_temp[50000];

    while (buffer[i] != 0) {
        if (buffer[i] != 'v' && buffer[i] != 'f')
            for (i++; buffer[i] != '\n' && buffer[i] != '\0'; i++);
        else if (buffer[i+1] != ' ' && buffer[i+1] != 't' && buffer[i+1] != 'n')
            for (i++; buffer[i] != '\n' && buffer[i] != '\0'; i++);
        else if (buffer[i+1] == ' ') {
            if (buffer[i] == 'v') {
                i += 2; float x;
                while (get_number(&i, buffer, &x))
                    v_temp[v++] = x;
                v_temp[v++] = x;
            }
            else {
                i += 2; float x;
                while (get_number(&i, buffer, &x))
                    f_temp[f++] = (int)(x - 1);
                f_temp[f++] = (int)(x - 1);
            }
        }
        else if (buffer[i+1] == 'n') {
            i += 3; float x;
            while (get_number(&i, buffer, &x))
                (1 == 1); // TODO
        }
        else if (buffer[i+1] == 't') {
            i += 3; float x;
            while (get_number(&i, buffer, &x))
                vt_temp[vt++] = x;
            vt_temp[vt++] = x;
        }
        i++;
    }

    for (i = 0; i < v; i++)
        vertices[i] = v_temp[i];
    for (i = 0; i < vt; i++)
        vertices[v+i] = vt_temp[i];

    for (i = 0; i < f; i++)
        indices[i] = f_temp[i];

    *vertexCount = v/3;
    *indexCount = f;
    printf("vertex count %d indices %d\n", *vertexCount, *indexCount);

    putchar('\n');
    return 1;
}