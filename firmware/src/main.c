#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{
    printk("DAO board firmware started\n");

    while (1) {
        printk("alive\n");
        k_sleep(K_SECONDS(1));
    }

    return 0;
}