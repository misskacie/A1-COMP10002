#include <stdio.h>
#include <stdlib.h>

int main(int argv, char *argc[]){
    int nums[4] = {1,2,3};
    int nums2[4] = {2,3,6};
    int result[4];
    int out;
    int carryover = 0;
    for(int i = 0; i < 5; i++){
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
    for(int i = 3; i >= 0 ;i--){
        printf("%d",result[i]);


    }






}