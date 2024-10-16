#include <stdio.h>


int main(){
    char stringa[10];

    char *c;
    c= &stringa[0];

    printf("Inserisci una stringa: \n");
    scanf("%s",stringa);

    while (*c!='\0'){
        if (( *c>='0') && (*c<='9')){
            *c='*';
        }
        c++;
    }
    printf("Stringa cambiata: \n %s", stringa);
    return 0;
}