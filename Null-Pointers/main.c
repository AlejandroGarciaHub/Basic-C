//
//  main.c
//  Null-Pointers
//
//  Created by Miguel Garcia on 8/28/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    
    int *ptr=NULL;
    int i=0;
    ptr=&i;
    
    printf("The value of ptr is : %x\n",ptr);
    
    if (ptr) {
        printf("Pointer is not a null pointer\n");
    }
    if (!ptr) {
        printf("Pointer is a null pointer\n");
    }
    return 0;
}
