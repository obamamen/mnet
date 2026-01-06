#include <stdio.h>
#include "mnet.h"

int main(void)
{
    if (mnet_initialize() != 0)
    {
        fprintf(stderr,"mnet_initialize() failed\n");
    }

    mnet_cleanup();
}