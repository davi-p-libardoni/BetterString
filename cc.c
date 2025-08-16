#include "cc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MIN_ALLOC 8

// Helpers
void realocarMem(cc *pcad, int newlen){
    if(pcad->cap <= newlen){
        int newsize = 8;
        while(newsize < newlen+1) newsize = newsize*2;
        pcad->mem = realloc(pcad->mem, newsize);
        pcad->cap = newsize;
    }
}

void shiftChars(cc *pcad, int pos, int offset){
    if(offset==0) return;
    realocarMem(pcad,pcad->tam+offset);
    if(offset<0){
        for(int i=pos;i<pcad->tam;i++)
            pcad->mem[i+offset] = pcad->mem[i];
    }else{
        for(int i=pcad->tam-1;i>=pos;i--)
            pcad->mem[i+offset] = pcad->mem[i];
    }
}


static void cc_ok(cc cad){
    assert(cad.tam >= 0);
    assert(cad.cap >= 0);
    assert(cad.tam == 0 || cad.mem != NULL);
    if(cad.cap > 0){
        assert(cad.cap > cad.tam);
        assert(cad.cap >= MIN_ALLOC);
        assert(cad.cap == (cad.cap & -cad.cap));
    }
}

cc cc_cria(char *str){
    return (cc){.tam = strlen(str),.cap = 0,.mem = str};
}

cc cc_copia(cc cad){
    int newTam = 0,cap = MIN_ALLOC;
    while(newTam==0){
        if(cap > cad.tam) newTam = cap;
        cap = cap*2;
    }
    cc copy = (cc){.tam=cad.tam,.mem=malloc(newTam),.cap=newTam};
    for(int i=0;i<=cad.tam;i++)
        copy.mem[i] = cad.mem[i];
    return copy;
}

void cc_destroi(cc cad){
    cc_ok(cad);
    if(cad.cap>0) free(cad.mem);   
}

int cc_tam(cc cad){
    cc_ok(cad);
    return cad.tam;
}

char *cc_mem(cc cad){
    cc_ok(cad);
    return cad.mem;
}

char cc_ch(cc cad, int pos){
    cc_ok(cad);
    if(pos >= cad.tam || pos < 0) return '\0';
    return cad.mem[pos];
}

cc cc_sub(cc cad, int pos, int tam){
    cc_ok(cad);
    if(pos>cad.tam) pos = cad.tam;
    if(pos+cad.tam < 0) { tam = tam + pos + cad.tam; pos = 0; }
    if(pos<0) pos = cad.tam + pos;
    if(cad.tam < pos+tam) tam = cad.tam - pos;
    
    return (cc){.tam=tam,.mem=cad.mem+pos,.cap=0};
}

int cc_busca_c(cc cad, int pos, cc chs){
    cc_ok(cad);
    cc_ok(chs);
    for(int i = pos;i<cad.tam;i++)
        for(int j = 0;j<chs.tam;j++)
            if(cad.mem[i]==chs.mem[j]) return i;
    return -1;
}

int cc_busca_nc(cc cad, int pos, cc chs){
    cc_ok(cad);
    cc_ok(chs);
    for(int i = pos;i<cad.tam;i++){
        int found = 0;
        for(int j = 0;j<chs.tam;j++)
            if(cad.mem[i]==chs.mem[j]) found = 1;
        if(!found) return i;
    }
    return -1;
}

int cc_busca_rc(cc cad, int pos, cc chs){
    cc_ok(cad);
    cc_ok(chs);
    for(int i = cad.tam;i>=pos;i--)
        for(int j = 0;j<chs.tam;j++)
            if(cad.mem[i]==chs.mem[j]) return i;
    return -1;
}

int cc_busca_rnc(cc cad, int pos, cc chs){
    cc_ok(cad);
    cc_ok(chs);
    for(int i = cad.tam;i>=pos;i--){
        int found = 0;
        for(int j = 0;j<chs.tam;j++)
            if(cad.mem[i]==chs.mem[j]) found = 1;
        if(!found) return i;
    }
}

int cc_busca_s(cc cad, int pos, cc buscada){
    cc_ok(cad);
    cc_ok(buscada);
    if(pos<0) pos = 0;
    for (int i = pos;i<cad.tam;i++){
        if(cad.mem[i]==buscada.mem[0]){
            if(cc_igual(cc_sub(cad,i,buscada.tam),buscada)) return i;
        }
    }
    return -1;
}

bool cc_igual(cc cad, cc cadb){
    cc_ok(cad);
    cc_ok(cadb);
    if(cad.tam != cadb.tam) return false;
    for(int i=0;i<cad.tam;i++)
        if(cad.mem[i]!=cadb.mem[i]) return false;
    return true;
}

void cc_imprime(cc cad){
    cc_ok(cad);
    printf("%.*s",cad.tam,cad.mem);
}

void cc_cat(cc *pcad, cc cadb){
    cc_ok(*pcad);
    cc_ok(cadb);
    if(pcad->cap==0) return;
    int newlen = pcad->tam+cadb.tam;
    realocarMem(pcad,newlen);
    int end = newlen;
    for(int i = pcad->tam, j = 0;i<end;i++,j++){
        pcad->mem[i] = cadb.mem[j];
    }
    pcad->mem[end] = '\0';
    pcad->tam = end;
}

void cc_catc(cc *pcad, char c){
    cc_ok(*pcad);
    if(pcad->cap==0) return;
    realocarMem(pcad,pcad->tam+1);
    pcad->mem[pcad->tam] = c;
    pcad->mem[pcad->tam+1] = '\0';
    pcad->tam = pcad->tam+1;
}

void cc_insere(cc *pcad, int pos, cc cadb){
    cc_ok(*pcad);
    cc_ok(cadb);
    if(pcad->cap==0) return;
    if(pos > pcad->tam) pos = pcad->tam;
    if(pos < 0) pos = 0;
    realocarMem(pcad,pcad->tam+cadb.tam);
    int offset = cadb.tam;
    for(int i = pcad->tam-1;i>=pos;i--)
        pcad->mem[i+offset] = pcad->mem[i];
    for(int i = 0;i<cadb.tam;i++)
        pcad->mem[pos+i] = cadb.mem[i];
    pcad->tam = pcad->tam+cadb.tam;
    pcad->mem[pcad->tam] = '\0';
}

void cc_remove(cc *pcad, int pos, int tam){
    cc_ok(*pcad);
    if(pcad->cap==0 || pos>=pcad->tam) return;
    if(pos<0) pos = pcad->tam+pos;
    if(pos+tam > pcad->tam) tam = pcad->tam - pos;
    for(int i = pos+tam;i < pcad->tam;i++){
        pcad->mem[i-tam] = pcad->mem[i];
    }
    pcad->tam = pcad->tam - tam;
    pcad->mem[pcad->tam] = '\0';
}

void cc_preenche(cc *pcad, int tam, char c){
    cc_ok(*pcad);
    if(pcad->tam == tam || pcad->cap == 0) return;
    realocarMem(pcad,tam);
    for(int i = pcad->tam;i<tam;i++)
        pcad->mem[i] = c;
    pcad->mem[tam] = '\0';
    pcad->tam = tam;
}

void cc_subst(cc *pcad, int pos, int tam, cc cadb, char preenche){
    cc_ok(*pcad);
    cc_ok(cadb);
    if(pcad->cap == 0 || tam<0) return;
    // Tam > pcad inteira, trunca pcad na posição pos para concatenar abaixo
    if(tam > pcad->tam && pos < pcad->tam){
        pcad->tam = pos;
    }
    // Pos = logo após o último caracter, apenas concatenar
    if(pos == pcad->tam){    
        cc_cat(pcad,cadb);
        return;
    }
    
    // Posição depois do final da string
    if(pos>pcad->tam) tam = 0;

    int offset = cadb.tam - tam, fill = 0, fillpos = -1;

    // Posição negativa (dentro da string, a partir do final)
    if(pos<0) pos = pcad->tam+pos;

    // Preparar p/ preencher - Posição negativa fora da string
    if(pos<0 && pos + tam < 0){
        fill = -(pos)-tam;
        fillpos = cadb.tam;
        offset += fill + tam;
        tam = 0;
        pos = 0;
    }

    // Preparar p/ preencher - Posição positiva fora da string
    if(pos>pcad->tam){
        fill = pos - pcad->tam;
        fillpos = pcad->tam;
    }

    shiftChars(pcad,pos,offset);
    pcad->tam += offset;

    if(fill!=0){
        realocarMem(pcad,pcad->tam+fill);
        for(int i=fillpos;i<fillpos+fill;i++) pcad->mem[i] = preenche;
        if(fillpos != cadb.tam) {
            pcad->tam += fill;
        }
    }

    for(int i=pos,j=0;i<pos+cadb.tam;i++,j++){
        pcad->mem[i] = cadb.mem[j];
    }
    pcad->mem[pcad->tam] = '\0';
}

char *cc_strc(cc cad){
    char *copy = malloc(cad.tam+1);
    for(int i = 0;i<cad.tam;i++)
        copy[i] = cad.mem[i];
    copy[cad.tam] = '\0';
    return copy;
}

cc cc_le_arquivo(cc nome){

}

void cc_grava_arquivo(cc cad, cc nome){

}

// temporário, apenas para teste
// APAGAR MAIN QUANDO TERMINAR AS FUNÇÕES
int main(){
    cc s = cc_("aranha");
    cc s_alt = cc_copia(s);
    cc t = cc_("homem");

    cc_subst(&s_alt,-s_alt.tam-1,0,t,'-');
    // cc_preenche(&s_alt,20,'a');
    // cc_imprime(s_alt);
    // printf("\n%d - %d\n",s_alt.tam,s_alt.cap);
   
}