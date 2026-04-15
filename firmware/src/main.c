#include "fuel_app.h"
#include "fuel_platform.h"

int main(void)
{
    fuel_platform_init();
    fuel_app_init();

    while (1) {
        fuel_app_process();
        fuel_platform_idle();
    }
}
