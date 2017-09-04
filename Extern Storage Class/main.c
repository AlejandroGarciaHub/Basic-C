//
//  main.c
//  Extern Storage Class
//
//  Created by Miguel Garcia on 8/29/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#include <stdio.h>

extern void message();

int main(int argc, const char * argv[]) {
    
    printf("Hello\n");
    message();
    return 0;
}
