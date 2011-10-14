#ifndef ASM_ARCH_IO_H
#define ASM_ARCH_IO_H

static inline void __raw_bits_and(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) & v), a);
}

static inline void __raw_bits_or(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) | v), a);
}

static inline void __raw_bits_xor(unsigned int v, unsigned int a)
{
	__raw_writel((__raw_readl(a) ^ v), a);
}

#endif

