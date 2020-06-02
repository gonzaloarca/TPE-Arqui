#include <std_io.h>

int putchar(char c){
	return write( 1, &c, 1 );
}

int strcmp(char *str1, char *str2){
	int i = 0, answer;
	char flag = 1;
	while(flag != 0)
    {
        if (str1[i] != str2[i])
        {
           	answer = (str1[i] < str2[i])? -1 : 1;
           	flag = 0;
        }

        if (str1[i] == 0)
        {
            answer = 0;
            flag = 0;
        }
        i++;
    }

    return answer;
}

int strlen(char *str){
    int i = 0;
    for( ; str[i] != '\0' ; i++);
    return i;
}

int puts( char * s ){
    while( *s != 0 ){
        putchar( *s);
        s++;
    }

    return 0;
}

void newLine(){
    write( 1,"\n", 1);
}

static void numToString( int num, char * str ){

    if( num == 0 ){
        str[0] = '0';
        str[1] = 0;
        return;
    }

    int dig = 0;
    int aux = num;

    while( aux != 0 ){ //cuento digitos para saber desde donde arrancar a meter los caracteres
        aux /= 10;
        dig++;
    }
    
    if( num < 0 ){
        dig++;
        str[0] = '-';
        num *= -1;      //porque num % n da negativo si num < 0
    }

    int i = dig;
    str[i--] = 0;

    while( num != 0 ){
        str[i--] = (num % 10) + '0';
        num /= 10;
    }
}

int printInt( int num ){
    char buffer[32];
    numToString( num, buffer );
    puts(buffer);

    return 0;
}
