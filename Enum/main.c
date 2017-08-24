//
//  main.c
//  Enum
//
//  Created by Miguel Garcia on 8/23/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

enum boolean{NO,YES};
enum days{mon,tue,wed,thu=4,fri,sat,sun};

int main(int argc, const char * argv[]) {
    
    enum boolean flag=NO;
    printf("%d %d %d\n", flag,thu,sat);
    return 0;
}
