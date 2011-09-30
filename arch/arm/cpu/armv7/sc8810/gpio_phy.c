#include <common.h>
#include <asm/io.h>
//#include <linux/kernel.h>

//#include <mach/regs_gpio.h>
#include <asm/arch/sc8810_reg_global.h>
#include <asm/arch/sc8810_reg_base.h>
#include <asm/arch/gpio_reg_v0.h>
#include <asm/arch/gpio_phy.h>

const static struct gpio_section  s_gpio_section_table[] = {
    {   (GPIO_BASE + 0*0x80),    0x8,    GPIO_SECTION_GPI    },
    {   (GPIO_BASE + 1*0x80),    0x10,    GPIO_SECTION_GPIO    },
    {   (GPIO_BASE + 2*0x80),    0x10,    GPIO_SECTION_GPIO    },
    {   (GPIO_BASE + 3*0x80),    0x10,    GPIO_SECTION_GPIO    },
    {   (GPIO_BASE + 4*0x80),    0x10,    GPIO_SECTION_GPIO   },
    {   (GPIO_BASE + 5*0x80),    0x10,    GPIO_SECTION_GPIO   },
    {   (GPIO_BASE + 6*0x80),    0x10,    GPIO_SECTION_GPIO   },
    {   (GPIO_BASE + 7*0x80),    0x10,    GPIO_SECTION_GPIO   },
    {   (GPIO_BASE + 8*0x80),    0x10,    GPIO_SECTION_GPIO    },
    {   (GPIO_BASE + 9*0x80),    0x10,    GPIO_SECTION_GPIO   },
    {   (ANA_GPIO_BASE + 0*0x80),   0x8,    GPIO_SECTION_GPI    },
    {   (ANA_GPIO_BASE + 1*0x80),   0x10,    GPIO_SECTION_GPIO  },
    {   (ANA_GPIO_BASE + 2*0x80),   0x10,    GPIO_SECTION_GPIO  },
};

struct gpio_section * gpio_get_section_table (u32 *table_size)
{
    *table_size = ARRAY_SIZE(s_gpio_section_table);

    return (struct gpio_section *) s_gpio_section_table;
}

