#include <std_io.h>
#include <std_num.h>

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

int printInt( int num ){
    char buffer[32];
    intToString( num, buffer );
    puts(buffer);

    return 0;
}
