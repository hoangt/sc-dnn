#include <stdio.h>
#include "zsim_hooks.h"


static void roi_test()
{
    printf("Begin - ROI Test\n");
    zsim_roi_begin();
    zsim_heartbeat();
    zsim_roi_end();
    printf("End  - ROI Test\n");
}

int main() {
    printf("C test\n");

    roi_test();

    printf("C test done\n");
    return 0;
}
