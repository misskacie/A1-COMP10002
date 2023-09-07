#include <stdio.h>
#include <stdlib.h>
#define DEBUG 1
void
do_plus(int varnum, int nums[], int nums2[]) {
    int result[4];
    int out;
    int carryover = 0;
    for(int i = 0; i < 2; i++){
        out = nums[i]+ nums2[i];
        if (carryover != 0){
            out += carryover;
            carryover = 0;

        }
        if (out >= 10){
            out -=10;
            carryover += 1;

        }
        result[i] = out;
    }
    #if DEBUG
    for(int i = 1; i >= 0 ;i--){
        printf("%d",result[i]);
    }
    #endif
}
int test (int *nums){
    printf("%d",*nums);


}

int main(int argv, char *argc[]){
    int nums[4][2] = {{1,2},{3,4},{5,6},{7,8}};
    int nums2[4][2] = {{1,1},{1,1},{1,1},{1,1}};
   // do_plus(3,nums[0],nums2[0]);
    int n = atoi("1");
    printf("%d",n);






}