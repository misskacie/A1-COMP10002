#include <stdio.h>
#include <stdlib.h>
#define INTSIZE 500

typedef struct{
    int length;
    int num[3];
} longint_t;


longint_t
reverse_array(longint_t var){
	longint_t tmp;
	tmp = var;
	int i = 0;
	for (int j = var.length - 1; j >= 0; j--){
		var.num[i] = tmp.num[j];
	}
	return var;
}

int main(int argv, char *argc[]){;
    longint_t var;
    var.length = 3;
    var.num[0] = 1;
    var.num[1] = 2;
    var.num[2] = 3;

    var = reverse_array(var);

    printf("%d %d %d",var.length, var.num[0],var.num[1]);

}