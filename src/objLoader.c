#include "../include/engine.h"
#include "../include/objLoader.h"
#include "../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

/* 
    DISCLAIMER:
        It's not pretty.
*/

static float get_number(int* i, char* buffer, float* x) {
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

void get_face_info(int* i, char* buffer, GLuint* f_temp, int *f, struct vertex_info* info) { 
    int k; char str[16];
    while (buffer[*i] != '\n' && buffer[*i] != '\0') {
        // check max_size; realloc
        for (k = 0; buffer[*i] != '/' && buffer[*i] != ' ' && buffer[*i] != '\0' && buffer[*i] != '\n'; (*i)++)
            str[k++] = buffer[*i];
        str[k] = 0;
        f_temp[*f] = atoi(str);

        if (buffer[(*i)] == '/') {
            if (buffer[(*i)+1] == '/') {
                info[f_temp[*f] - 1].textureIndex = 0;
                for ((*i) += 2, k = 0; buffer[*i] != ' ' && buffer[*i] != '\n' && buffer[*i] != '\0'; (*i)++)
                    str[k++] = buffer[*i];
                str[k] = 0;
                info[f_temp[*f] - 1].normalIndex = atoi(str);
            }
            else {
                for ((*i) += 1, k = 0; buffer[*i] != '/' && buffer[*i] != ' '; (*i)++)
                    str[k++] = buffer[*i];
                str[k] = 0;
                info[f_temp[*f] - 1].textureIndex = atoi(str);

                if (buffer[*i] == '/') {
                    for (*i += 1, k = 0; buffer[*i] != ' ' && buffer[*i] != '\n' && buffer[*i] != '\0'; (*i)++)
                        str[k++] = buffer[*i];
                    str[k] = 0;
                    info[f_temp[*f] - 1].normalIndex = atoi(str);
                }
                else
                    info[f_temp[*f] - 1].normalIndex = 0;
            }
        }
        (*f)++;
        for (; buffer[*i] == ' '; (*i)++);
    }
}

int OBJ_load(OBJ_data** data, const char* filename) {
    char* buffer;

    if (!readfile(&buffer, filename)) {
        fprintf(stderr, "There was an error reading file '%s'.\n", filename);
        return 0;
    }

    float x;
    int i = 0, j = 0, k = 0, w = 0, vn = 0, vt = 0, f = 0, v = 0, counter = 0;

    struct vertex_info* info = (struct vertex_info*)malloc(50000 * sizeof(struct vertex_info));
    GLfloat* v_temp = (GLfloat*)malloc(50000 * sizeof(GLfloat));
    GLfloat* vt_temp = (GLfloat*)malloc(50000 * sizeof(GLfloat));
    GLfloat* vn_temp = (GLfloat*)malloc(50000 * sizeof(GLfloat)); vt_temp[50000], vn_temp[5000];
    GLuint* f_temp = (GLuint*)malloc(50000 * sizeof(GLuint));

    (*data) = (OBJ_data*)malloc(sizeof(OBJ_data));

    (*data)->hitbox.min[0] =  INFINITY; (*data)->hitbox.min[1] =  INFINITY; (*data)->hitbox.min[2] =  INFINITY;
    (*data)->hitbox.max[0] = -INFINITY; (*data)->hitbox.max[1] = -INFINITY; (*data)->hitbox.max[2] = -INFINITY;

    while (buffer[i] != 0) {
        counter = 0;
        if (buffer[i] != 'v' && buffer[i] != 'f') // comment or other meaningless line; skip to next line
            for (; buffer[i] != '\n' && buffer[i] != '\0'; i++);
        else if (buffer[i+1] != ' ' && buffer[i+1] != 't' && buffer[i+1] != 'n') // could be position, texture, normal or face
            for (i++; buffer[i] != '\n' && buffer[i] != '\0'; i++);
        else if (buffer[i+1] == ' ') { // it is either position or face
            if (buffer[i] == 'v') { // it was position coord
                for (i++; buffer[i] == ' '; i++);
                while (get_number(&i, buffer, &x)) {
                    v_temp[v++] = x;
                    if (x < (*data)->hitbox.min[counter])
                        (*data)->hitbox.min[counter] = x;
                    else if (x > (*data)->hitbox.max[counter])
                        (*data)->hitbox.max[counter] = x;
                    counter++;
                }
                v_temp[v++] = x;
                if (x < (*data)->hitbox.min[counter])
                    (*data)->hitbox.min[counter] = x;
                else if (x > (*data)->hitbox.max[counter])
                    (*data)->hitbox.max[counter] = x;                
            }
            else { // it was a face
                for (i++; buffer[i] == ' '; i++);
                get_face_info(&i, buffer, f_temp, &f, info);
            }
        }
        else if (buffer[i+1] == 'n') { // it was a normal coord
            for (i+=2; buffer[i] == ' '; i++);
            while (get_number(&i, buffer, &x))
                vn_temp[vn++] = x;
            vn_temp[vn++] = x;
        }
        else if (buffer[i+1] == 't') { // it was a texture coord
            for (i+=2; buffer[i] == ' '; i++);
            while (get_number(&i, buffer, &x))
                vt_temp[vt++] = x;
            vt_temp[vt++] = x;
        }
        i++;
    }

    (*data)->vertexCount = v/3;
    (*data)->indexCount = f;

    (*data)->vertices = (GLfloat*)malloc(8 * (*data)->vertexCount * sizeof(GLfloat)); // 8: 3 position, 2 texture, 3 normal
    (*data)->indices = (GLuint*)malloc((*data)->indexCount * sizeof(GLuint));

    for (i = 0, j = 0; i < ((*data)->vertexCount * 8); i += 8, j+=3) {
        (*data)->vertices[i] = v_temp[j];
        (*data)->vertices[i+1] = v_temp[j+1];
        (*data)->vertices[i+2] = v_temp[j+2];
    }

    for (i = 0; i < f; i++)
        (*data)->indices[i] = f_temp[i] - 1;

    for (i = 0, j = 3; i < (*data)->vertexCount; i++, j+=8) {
        int tex_start = (info[i].textureIndex - 1) * 2;
        int normal_start = (info[i].normalIndex - 1) * 3;
        (*data)->vertices[j] = vt_temp[tex_start];
        (*data)->vertices[j+1] = vt_temp[tex_start+1];
        (*data)->vertices[j+2] = vn_temp[normal_start];
        (*data)->vertices[j+3] = vn_temp[normal_start+1];
        (*data)->vertices[j+4] = vn_temp[normal_start+2];
    }
    
    return 1;
}
