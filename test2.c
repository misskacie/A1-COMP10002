#include <stdio.h>
#include <stdlib.h>
#define INTSIZE 500

typedef struct{
    int length;
    int num[INTSIZE];
} longint_t;


void 
reverse_array(longint_t *var){
	longint_t tmp;
	tmp = *var;
	int i = 0;
	for (int j = var->length - 1; j >= 0; j--){
		var->num[i] = tmp.num[j];
		i++;
	}
	return;
}

void
do_plus(longint_t *var1, longint_t var2) {
    longint_t result;
    int out;
    int carryover = 0;
	int max = (var1->length > var2.length) ? var1->length : var2.length ;
	int min = (var1->length < var2.length) ? var1->length : var2.length ;
	int maxvar = var1->length > var2.length ? 1 : 2 ;
	int i;
	#if DEBUG
	printf("doplus\n");
		for (int i = 0; i <var1->length ;i++){
			printf("%d",var1->num[i]);
		}
		printf("\n");
		for (int i = 0; i <var2.length ;i++){
			printf("%d",var2.num[i]);
		}
		printf("\n");
	#endif
    for(i = 0; i < min; i++){
        out = var1->num[i] + var2.num[i];
        
        if (carryover != 0){
            out += carryover;
            carryover = 0;
        }
        if (out >= 10){
            out -=10;
            carryover = 1;

        }
        printf("out, carryover %d %d\n",out, carryover);
        result.num[i] = out;
		result.length = i + 1;
    }
    printf("i: %d\n", i);
    printf("out, carryover %d %d\n",out, carryover);
    result.num[i] = carryover;
    result.length = i + 1;
    reverse_array(&result);
	*var1 = result;
}
#define CH_A 'a'
void
do_print(int varnum, longint_t var) {
	printf("register %c: ", varnum+CH_A);
	int ref = var.length % 3;
    ref = ref == 0 ? 3 : 0;
    for (int i = var.length - 1; i >= 0; i--){
		printf("%d", var.num[i]);
        if (i != 0 && (ref + i) % 3 == 0){
            printf(",");
        }
	}
	printf("\n");
}

void
do_multiplication(longint_t *var1, longint_t var2){
	longint_t result;
	int out;
    int carryover = 0;
	int max = var1->length > var2.length ? var1->length : var2.length;
	int i, j;
	
	for(i=0; i<max; i++){
		for(j=0; j<max;j++){
			out = var1->num[i] * var2.num[j];
			if (carryover != 0){
            	out += carryover;
            	carryover = 0;
        	}
        	while (out >= 10){
            	out -=10;
            	carryover += 1;
        	}
		}
	}

	*var1 = result;
}

int main(int argv, char *argc[]){;
    longint_t var[2];
    var[0].length = 3;
    
    for(int i = 0; i <3 ; i++){
        var[0].num[i] = i ;
        var[0].length = i + 1;
    }
    longint_t var1;
    var1.length = 3;
    var1.num[0] = 1;
    var1.num[1] = 2;
    var1.num[2] = 3;

    longint_t var2;
    var2.length = 3;
    var2.num[0] = 1;
    var2.num[1] = 2;
    var2.num[2] = 3;

    do_multiplication(&var1 ,var2);
    printf("%d %d %d %d\n",var1.num[0], var1.num[1],var1.num[2],var1.num[3]);

    // printf("%d %d %d\n",var.num[0], var.num[1],var.num[2]);
    // printf("%d %d %d\n",var2.num[0], var2.num[1],var2.num[2]);

    // do_plus(&var,var2);

    // printf("%d %d %d %d\n",var.num[0], var.num[1],var.num[2],var.num[3]);
   
}