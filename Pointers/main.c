//
//  main.c
//  Pointers
//
//  Created by Miguel Garcia on 8/24/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    
    int i=10;
    double d=10.123456;
    char c='a';
    
    //Pointers
    int *ip=&i;
    double *dp=&d;
    char *cp=&c;
    
    printf("Value of i: %d\n\n",i);
    printf("Value of d: %f\n\n",d);
    printf("Value of c: %c\n\n\n\n",c);
    
    printf("Address of i: %d\n\n",i);
    printf("Address of d: %f\n\n",d);
    printf("Address of c: %c\n\n\n\n",c);

    
}
