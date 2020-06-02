#include <std_io.h>

int putchar(char c){
	return write( 1, &c, DEFAULT_CHAR_COLOR );
}

int putcharColor(char c, int rgb){
	return write( 1, &c, rgb );
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

        if (str1[i] == '\0')
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