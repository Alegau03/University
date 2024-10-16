#include <stdio.h>

int main(){
    char stringa[20];
    printf("Inserisci una stringa: \n");
    scanf("%s",&stringa);
    int i=0;

    while(stringa[i]!= '\0 '){
        if (( stringa[i]>='0') && (stringa[i]<='9')){
            stringa[i]='*';
        }
        i++;
    }
    printf("Nuova stringa: \n %s \n",stringa);

    return 0;
}


