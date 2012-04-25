LOCAL_TOOLCHAIN := $(ANDROID_BUILD_TOP)/prebuilt/linux-x86/toolchain/arm-eabi-4.3.1/bin/arm-eabi-
#UBOOT_OUT := $(TARGET_OUT_INTERMEDIATES)/u-boot
UBOOT_OUT := u-boot
UBOOT_BUILT_SPL := $(UBOOT_OUT)/nand_spl/u-boot-spl-16k.bin
UBOOT_BUILT_BIN := $(UBOOT_OUT)/u-boot.bin
UBOOT_BUILT_FDL := $(UBOOT_OUT)/nand_fdl/fdl2.bin
UBOOT_CONFIG := $(UBOOT_OUT)/include/config.h

$(UBOOT_OUT):
	@echo "Start U-Boot build"

$(UBOOT_CONFIG): u-boot/include/configs/$(addsuffix .h,$(UBOOT_DEFCONFIG))
	mkdir -p $(UBOOT_OUT)
	$(MAKE) -C u-boot CROSS_COMPILE=$(LOCAL_TOOLCHAIN) distclean
	$(MAKE) -C u-boot CROSS_COMPILE=$(LOCAL_TOOLCHAIN) $(UBOOT_DEFCONFIG)_config

$(INSTALLED_UBOOT_TARGET) : $(UBOOT_CONFIG) $(UBOOT_OUT)
	$(MAKE) -C u-boot CROSS_COMPILE=$(LOCAL_TOOLCHAIN)
	$(MAKE) -C u-boot CROSS_COMPILE=$(LOCAL_TOOLCHAIN) fdl2
	@cp $(UBOOT_BUILT_SPL) $(PRODUCT_OUT)
	@cp $(UBOOT_BUILT_BIN) $(PRODUCT_OUT)
	@cp $(UBOOT_BUILT_FDL) $(PRODUCT_OUT)
	@echo "Install U-Boot target done"

