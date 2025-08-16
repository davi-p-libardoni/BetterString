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
    // Pos = após o último caracter, apenas concatenar
    if(pos == pcad->tam){    
        cc_cat(pcad,cadb);
        return;
    }
    
    if(pos>pcad->tam) tam = 0;
    int offset = cadb.tam - tam, fill = 0, fillpos = -1;
    if(pos<0) pos = pcad->tam+pos;
    if(pos<0 && pos + tam < 0){
        fill = -(pos)-tam;
        fillpos = cadb.tam;
        offset += fill + tam;
        pos = 0;
    }
    if(pos>pcad->tam){
        fill = pos - pcad->tam;
        fillpos = pcad->tam;
    }
    shiftChars(pcad,pos,offset);
    pcad->tam += offset;
    if(fill!=0){
        realocarMem(pcad,pcad->tam+fill);
        for(int i=fillpos;i<fillpos+fill;i++) pcad->mem[i] = preenche;
        pcad->tam += fill;
    }
    for(int i=pos,j=0;i<pos+cadb.tam;i++,j++){
        pcad->mem[i] = cadb.mem[j];
    }
    pcad->mem[pcad->tam] = '\0';
    //cc_imprime(*pcad);
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
    
    printf("\n=== CC_SUBST TESTS ===\n");
    printf("All tests start with string 'abacaxi' and fill character '%%'\n\n");
    
    // Test case 1: pos=5, tam=2, cadb="te" -> "abacate"
    cc test1 = cc_copia(cc_("abacaxi"));
    cc_subst(&test1, 5, 2, cc_("te"), '%');
    cc expected1 = cc_("abacate");
    printf("Test 1 - pos=5, tam=2, cadb=\"te\": ");
    cc_imprime(test1);
    if (cc_igual(test1, expected1)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected1);
        printf(")\n");
    }
    
    // Test case 2: pos=7, tam=0, cadb="." -> "abacaxi."
    cc test2 = cc_copia(cc_("abacaxi"));
    cc_subst(&test2, 7, 0, cc_("."), '%');
    cc expected2 = cc_("abacaxi.");
    printf("Test 2 - pos=7, tam=0, cadb=\".\": ");
    cc_imprime(test2);
    if (cc_igual(test2, expected2)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected2);
        printf(")\n");
    }
    
    // Test case 3: pos=9, tam=0, cadb="." -> "abacaxi%%."
    cc test3 = cc_copia(cc_("abacaxi"));
    cc_subst(&test3, 9, 0, cc_("."), '%');
    cc expected3 = cc_("abacaxi%%.");
    printf("Test 3 - pos=9, tam=0, cadb=\".\": ");
    cc_imprime(test3);
    if (cc_igual(test3, expected3)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected3);
        printf(")\n");
    }
    
    // Test case 4: pos=9, tam=0, cadb="" -> "abacaxi%%"
    cc test4 = cc_copia(cc_("abacaxi"));
    cc_subst(&test4, 9, 0, cc_(""), '%');
    cc expected4 = cc_("abacaxi%%");
    printf("Test 4 - pos=9, tam=0, cadb=\"\": ");
    cc_imprime(test4);
    if (cc_igual(test4, expected4)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected4);
        printf(")\n");
    }
    
    // Test case 5: pos=9, tam=10, cadb="." -> "abacaxi%%."
    cc test5 = cc_copia(cc_("abacaxi"));
    cc_subst(&test5, 9, 10, cc_("."), '%');
    cc expected5 = cc_("abacaxi%%.");
    printf("Test 5 - pos=9, tam=10, cadb=\".\": ");
    cc_imprime(test5);
    if (cc_igual(test5, expected5)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected5);
        printf(")\n");
    }
    
    // Test case 6: pos=-3, tam=0, cadb="123" -> "abac123axi"
    cc test6 = cc_copia(cc_("abacaxi"));
    cc_subst(&test6, -3, 0, cc_("123"), '%');
    cc expected6 = cc_("abac123axi");
    printf("Test 6 - pos=-3, tam=0, cadb=\"123\": ");
    cc_imprime(test6);
    if (cc_igual(test6, expected6)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected6);
        printf(")\n");
    }
    
    // Test case 7: pos=0, tam=1, cadb="123" -> "123bacaxi"
    cc test7 = cc_copia(cc_("abacaxi"));
    cc_subst(&test7, 0, 1, cc_("123"), '%');
    cc expected7 = cc_("123bacaxi");
    printf("Test 7 - pos=0, tam=1, cadb=\"123\": ");
    cc_imprime(test7);
    if (cc_igual(test7, expected7)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected7);
        printf(")\n");
    }
    
    // Test case 8: pos=-9, tam=1, cadb="123" -> "123%abacaxi"
    cc test8 = cc_copia(cc_("abacaxi"));
    cc_subst(&test8, -9, 1, cc_("123"), '%');
    cc expected8 = cc_("123%abacaxi");
    printf("Test 8 - pos=-9, tam=1, cadb=\"123\": ");
    cc_imprime(test8);
    if (cc_igual(test8, expected8)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected8);
        printf(")\n");
    }
    printf("Len Esperada: %d | Len Recebida: %d\n",expected8.tam,test8.tam);
    
    // Test case 9: pos=3, tam=200, cadb="" -> "aba"
    cc test9 = cc_copia(cc_("abacaxi"));
    cc_subst(&test9, 3, 200, cc_(""), '%');
    cc expected9 = cc_("aba");
    printf("Test 9 - pos=3, tam=200, cadb=\"\": ");
    cc_imprime(test9);
    if (cc_igual(test9, expected9)) {
        printf(" ✅ PASS\n");
    } else {
        printf(" ❌ FAIL (expected: ");
        cc_imprime(expected9);
        printf(")\n");
    }
    
    // Clean up
    cc_destroi(test1);
    cc_destroi(test2);
    cc_destroi(test3);
    cc_destroi(test4);
    cc_destroi(test5);
    cc_destroi(test6);
    cc_destroi(test7);
    cc_destroi(test8);
    cc_destroi(test9);
    
    printf("\n=== CC_SUBST TESTS COMPLETE ===\n");
}