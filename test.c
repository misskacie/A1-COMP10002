#include <stdio.h>
#include <stdlib.h>
#define DEBUG 1
#define INTSIZE 500
#define VARSEQUAL 0
#define VAR1GREATER 1
#define VAR2GREATER 2

#define LONG_ZERO {.num = {3}, .length = 1}

typedef struct{
    int length;
    int num[500];
} longint_t;



int
is_greater_than(longint_t var1, longint_t var2){
	if (var1.length == var2.length){
		for (int i = var1.length - 1 ; i >= 0; i--){
			if (var1.num[i] == var2.num[i]){
				continue;
			} else if (var1.num[i] > var2.num[i]) {
				return VAR1GREATER;
			} else {
				return VAR2GREATER;
			}
		}
		return VARSEQUAL;
	}
	if (var1.length > var2.length){
		return VAR1GREATER;
	} else {
		return VAR2GREATER;
	}
}


void
do_subtraction(longint_t *var1, longint_t var2){
	longint_t result = {.num = {0}, .length = 1};
    int out;
    int carryover = 0;
	int i;
	int status = is_greater_than(*var1,var2);

	if(status == VAR2GREATER){
		//print_error("Operation leads to negative integer");
		exit(EXIT_FAILURE);
	} else if (status == VARSEQUAL){
		*var1 = result;
		return;
	}

    //printf("var length %d\n",var1->length);
    for(i = 0; i < var1->length; i++){
        if (var1->num[i] < var2.num[i]){
			var1->num[i] += 10;
			var1->num[i+1] -= 1;
		}

		if (i < var1->length && i < var2.length){
			out = var1->num[i] - var2.num[i];
		} else {
			out = var1->num[i];
		}
        //printf("i: %d\n",i);
        result.num[i] = out;
		result.length = i + 1;
    }
    //remove leading zeros from the result
    while(result.length > 1 && result.num[result.length - 1] == 0){
        result.length -=1;
    }
	
	*var1 = result;
}



int main(int argv, char *argc[]){
    longint_t var3 = LONG_ZERO;
  
    for (int i = var3.length - 1; i >= 0;i--){
        printf("%d",var3.num[i]);
    }
	printf("\n%d",var3.length);
    



}