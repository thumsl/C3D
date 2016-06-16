#include "../include/engine.h"
#include "../include/objLoader.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

/* 
    DISCLAIMER:
        It's not pretty.
*/

float get_number(int* i, char* buffer, float* x) {
    // reads characters from buffer until space, \n or \0 and gen number from them
    char str[16];
    int j;

    for (j = 0; buffer[*i] != ' ' && buffer[*i] != '\n' && buffer[*i] != '\0'; (*i)++)
        str[j++] = buffer[(*i)];

    str[j] = 0;

    while (buffer[*i] == ' ')
        (*i)++;

    *x = atof(str);

    if (buffer[*i] == '\n' || buffer[*i] == '\0')
        return 0;
    else
        return 1;
}

int loadOBJ(OBJ_data** data, const char* filename) {
    char* buffer;

    if (!readfile(&buffer, filename)) {
        fprintf(stderr, "There was an error reading file '%s'.\n", filename);
        return 0;
    }

    float x;
    int i = 0, j = 0, k = 0, vn = 0, vt = 0, f = 0, v = 0;

    GLfloat v_temp[50000], vt_temp[50000];
    GLuint f_temp[50000];

    while (buffer[i] != 0) {
        if (buffer[i] != 'v' && buffer[i] != 'f')
            for (; buffer[i] != '\n' && buffer[i] != '\0'; i++);
        else if (buffer[i+1] != ' ' && buffer[i+1] != 't' && buffer[i+1] != 'n')
            for (i++; buffer[i] != '\n' && buffer[i] != '\0'; i++);
        else if (buffer[i+1] == ' ') {
            if (buffer[i] == 'v') {
                for (i++; buffer[i] == ' '; i++);
                while (get_number(&i, buffer, &x))
                    v_temp[v++] = x;
                v_temp[v++] = x;
            }
            else {
                for (i++; buffer[i] == ' '; i++);
                while (get_number(&i, buffer, &x))
                    f_temp[f++] = (int)(x - 1);
                f_temp[f++] = (int)(x - 1);
            }
        }
        else if (buffer[i+1] == 'n') {
            for (i+=2; buffer[i] == ' '; i++);
            while (get_number(&i, buffer, &x))
                (1 == 1); // TODO
        }
        else if (buffer[i+1] == 't') {
            for (i+=2; buffer[i] == ' '; i++);
            while (get_number(&i, buffer, &x))
                vt_temp[vt++] = x;
            vt_temp[vt++] = x;
        }
        i++;
    }

    (*data) = (OBJ_data*)malloc(sizeof(OBJ_data));

    (*data)->vertexCount = v/3;
    (*data)->indexCount = f;

    (*data)->vertices = (GLfloat*)malloc(5 * (*data)->vertexCount * sizeof(GLfloat));
    (*data)->indices = (GLuint*)malloc((*data)->indexCount * sizeof(GLuint));

    if (vt != 0) {
        for (k = 0, i = 0, j = 0; k < ((*data)->vertexCount * 5); i += 3, j+= 2, k += 5) {
            (*data)->vertices[k] = v_temp[i];
            (*data)->vertices[k+1] = v_temp[i+1];
            (*data)->vertices[k+2] = v_temp[i+2];
            (*data)->vertices[k+3] = vt_temp[j];
            (*data)->vertices[k+4] = vt_temp[j+1];
        }
    }
    else {
        for (k = 0, i = 0, j = 0; k < ((*data)->vertexCount * 5); i += 3, j+= 2, k += 5) {
            (*data)->vertices[k] = v_temp[i];
            (*data)->vertices[k+1] = v_temp[i+1];
            (*data)->vertices[k+2] = v_temp[i+2];
            (*data)->vertices[k+3] = 0.0f;
            (*data)->vertices[k+4] = 0.0f;
        }
    }

    DEBUG_PRINT(("K = %d, i = %d, j = %d, v = %d, vt = %d, v+vt = %d, f = %d\n", k, i, j, v, vt, v+vt, f));

    for (i = 0; i < f; i++)
        (*data)->indices[i] = f_temp[i];

    DEBUG_PRINT(("vertex count %d indices %d\n", (*data)->vertexCount, (*data)->indexCount));
    putchar('\n');

    return 1;
}