//
//  main.c
//  Pointers Arithmetic
//
//  Created by Miguel Garcia on 8/29/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    
    int arr[5]={1,3,5,7,9};
    int *ip=&arr;
    
    for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++) {
        printf("%d\n",*ip);
        ip++;
    }
    
    return 0;
}
