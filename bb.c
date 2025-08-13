#include "bb.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

static void bb_ok(bb cad){
    assert(cad.tam >= 0);
    assert(cad.tam == 0 || cad.mem != NULL);
}

bb bb_cria(char *str){
    return (bb){.tam = strlen(str),.mem = str};
}

//void bb_destroi(bb cad){
//    
//}

int bb_tam(bb cad){
    return cad.tam;
}

char *bb_mem(bb cad){
    return cad.mem;
}

char bb_ch(bb cad, int pos){
    return cad.mem[pos];
}

bb bb_sub(bb cad, int pos, int tam){
    if(pos>cad.tam) return bb_("");
    char* last = cad.mem+cad.tam;
    if(pos<0){
        if(cad.tam+pos+tam>cad.tam) tam = cad.tam;
        if(cad.tam+pos<0) pos = -1*(cad.tam);
        return (bb){.tam=tam,.mem=last+pos};
    }
    return (bb){.tam=tam,.mem=(cad.mem)+pos};
}

int bb_busca_c(bb cad, int pos, bb chs){
    for(int i = pos;i<cad.tam;i++)
        for(int j = 0;j<chs.tam;j++)
            if(cad.mem[i]==chs.mem[j]) return i;
    return -1;
}

int bb_busca_nc(bb cad, int pos, bb chs){
    for(int i = pos;i<cad.tam;i++){
        int found = 0;
        for(int j = 0;j<chs.tam;j++)
            if(cad.mem[i]==chs.mem[j]) found = 1;
        if(!found) return i;
    }
    return -1;
}

int bb_busca_rc(bb cad, int pos, bb chs){
    for(int i = cad.tam;i>=pos;i--)
        for(int j = 0;j<chs.tam;j++)
            if(cad.mem[i]==chs.mem[j]) return i;
    return -1;
}

int bb_busca_rnc(bb cad, int pos, bb chs){
    for(int i = cad.tam;i>=pos;i--){
        int found = 0;
        for(int j = 0;j<chs.tam;j++)
            if(cad.mem[i]==chs.mem[j]) found = 1;
        if(!found) return i;
    }
}

int bb_busca_s(bb cad, int pos, bb buscada){
    
}

bool bb_igual(bb cad, bb cadb){
    if(cad.tam != cadb.tam) return false;
    for(int i=0;i<cad.tam;i++){
        if(cad.mem[i]!=cadb.mem[i]) return false;
    }
    return true;
}

void bb_imprime(bb cad){
    printf("%.*s",cad.tam,cad.mem);
}

int main(){
    bb s = bb_("abcdearecd");
    printf("1o - %d\nUltimo - %d\n",bb_busca_nc(s,0,bb_("abcde")),bb_busca_rnc(s,0,bb_("abcde")));
    //bb sub = bb_sub(s,-8,2);
    //bb_imprime(sub);
}