#!/bin/sh

DIR_PROTECT="arch/arm/cpu/armv7/sc8810 arch/arm/cpu/arm926ejs/sc8800g arch/arm/cpu/arm926ejs/sc8800x property nand_fdl nand_spl"
PRODUCTS="sc8810_openphone sp8810 sp8805ga sp6810a openphone native_openphone"

argu=$1
if [ -z $argu ]; then
argu="clean"
elif [ $argu != "clean" ] && [ $argu != "restore" ]; then
echo " you should type as $0 clean or restore"
exit 0
fi

if [ $argu = "clean" ]; then
    for dir in $DIR_PROTECT; do
    find $dir -name *.c -o -name *.S | xargs rm -f
    done

    echo "CONFIG_IDH_BUILD = 1" > ./include/idh_config.mk
fi 

if [ $argu = "restore" ]; then
    for dir in $DIR_PROTECT; do
    git checkout -- $dir
    done
    rm ./include/idh_config.mk 2>/dev/null
fi

