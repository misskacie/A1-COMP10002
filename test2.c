#include <stdio.h>
#include <stdlib.h>

void
do_plus(int nums[4][2]) {
    printf("%d\n", nums[0][0]);

}


int main(int argv, char *argc[]){
    int nums[4][2] = {{1,2},{3,4},{5,6}};
    do_plus(nums);


}