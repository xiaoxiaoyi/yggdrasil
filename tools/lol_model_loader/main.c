#include <stdio.h>
#include <stdlib.h>

typedef struct Vertex{
    float position[3];
    char boneldx[4];
    float weights[4];
    float normals[3];
    float texcoords[2];
}Vertex;

void saveToOBJ(Vertex * v, size_t vSize, short * ind, size_t indSize){
    printf("Open save.obj\n");
    FILE * f = fopen("save.obj", "w");
    
    fprintf(f, "o model\n\n");
    
    // Vertex
    size_t i;
    for(i=0; i<vSize; ++i ){
        fprintf(f, "v %f %f %f\n", v[i].position[0], v[i].position[1], v[i].position[2]);
    }
    fprintf(f,"\n");
    
    // Normal
    for(i=0; i<vSize; ++i ){
        fprintf(f, "vn %f %f %f\n", v[i].normals[0], v[i].normals[1], v[i].normals[2]);
    }
    fprintf(f,"\n");
    
    // Texture
    for(i=0; i<vSize; ++i ){
        fprintf(f, "vt %f %f\n", v[i].texcoords[0], v[i].texcoords[1]);
    }
    fprintf(f,"\n");
    
    for(i=0; i<indSize/3; ++i ){
        fprintf(f, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", ind[i*3] + 1, ind[i*3] + 1, ind[i*3] + 1,
                                                     ind[i*3 + 1] + 1, ind[i*3 + 1] + 1, ind[i*3 + 1] + 1,
                                                     ind[i*3 + 2] + 1, ind[i*3 + 2] + 1, ind[i*3 + 2] + 1);
    }
    
    
    fprintf(f,"\n");
    
    fclose(f);
}

int main(void){
    FILE * f = fopen("Olaf.skn", "r");
    printf("#### HEADER ####\n");
    int magicNumber;
    fread (&magicNumber,4,1,f);
    printf("MagicNumber %d\n", magicNumber);
    
    short numObjects;
    fread (&numObjects,2,1,f);
    printf("numObjects %d\n", numObjects);
    
    short matHeader;
    fread (&matHeader,2,1,f);
    printf("matHeader %d\n", matHeader);
    printf("\n");
    
    if(matHeader == 1) {
        printf("#### Material Block ####\n");
        int numMaterials;
        fread (&numMaterials,4,1,f);
        printf("numMaterials %d\n", numMaterials);
        
        if(numMaterials > 0){
            char name[64];
            fread (&name,1,64,f);
            printf("name %s\n", name);
            
            int startVertex;
            fread (&startVertex,4,1,f);
            printf("startVertex %d\n", startVertex);
            
            int numVertices;
            fread (&numVertices,4,1,f);
            printf("numVertices %d\n", numVertices);
            
            int startIndex;
            fread (&startIndex,4,1,f);
            printf("startIndex %d\n", startIndex);
            
            int numIndices;
            fread (&numIndices,4,1,f);
            printf("numIndices %d\n", numIndices);
        }
        printf("\n");
    }
    
    printf("#### COUNT BLOCK ####\n");
    int numIndicies;
    fread (&numIndicies,4,1,f);
    printf("numIndicies %d\n", numIndicies);
    printf("\n");
    int numVertices;
    fread (&numVertices,4,1,f);
    printf("numVertices %d\n", numVertices);
    printf("\n");
    
    printf("#### INDEX BLOCK ####\n");
    short * indices = (short *) malloc(sizeof(short) * numIndicies);
    fread (indices,sizeof(short),numIndicies,f);
    printf("Read Index\n\n");
    
    printf("#### VERTEX BLOCK ####\n");
    Vertex * vertex = (Vertex *) malloc(sizeof(Vertex) * numVertices);
    fread (vertex,sizeof(Vertex),numVertices,f);
    printf("Read Block\n\n");
    
    if(fgetc(f)==0)
        printf("FINAL EQUAL EOF => true\n");
    else
        printf("PROBLEM NO EOF\n");
    
    //SAVE
    saveToOBJ(vertex, numVertices, indices, numIndicies);
    
    free(indices);
    free(vertex);
    fclose(f);
    return 0;
}
