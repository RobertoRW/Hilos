#include<stdio.h>
#include<string.h>

void get_line(char *linea, int maximo);
char arreglo[10];   /* NO ESTA INICIALIZADA */
void strcy(char *s1, char *s2);

main(){
    char str[100]; /* Donde se guarda los comandos */
    get_line(str, 100);
    
    cosas(str);
}

cosas(char *str)
{
    str[4] = '\0';
    strcpy(arreglo+2, str); /* va a copiar lo que recibe a partir de la pos 2 */
    int i;
    for (i = 0; i < 10; i++)
        if(arreglo[i] == '\0')
            printf("'\\0'"); 
        else
            putchar(arreglo[i]);
    
    putchar('\n');
}

void get_line(char s[], int lim)
{
    int c, i;

    for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
        s[i] = c;
    if (c == '\n') {
        s[i++] = c;
    }
    s[i] = '\0';
    return;
}

