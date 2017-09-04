//
//  main.c
//  Static Storage Class
//
//  Created by Miguel Garcia on 8/29/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#include <stdio.h>

void func1(void){
    static int i=5;
    i++;
    printf("func1 - Value of i is: %d\n",i);
}
void func2(void){
    int i=5;
    i++;
    printf("func2 - Value of i is : %d\n",i);
}


int main(int argc, const char * argv[]) {
    // insert code here...
    
    int count;
    count=5;
    
    while (count--) {
        func1();
    }
    printf("\n\n");
    count=5;
    while (count--) {
        func2();
    }
    return 0;
}
