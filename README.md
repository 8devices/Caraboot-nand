Caraboot-nand
========

U-Boot for Rambutan based boards


Build
-------
----

1) Build buildroot toolchain (http://buildroot.org)

Download:
```
cd your_work_dir
git clone git://git.buildroot.net/buildroot
```
 
Configure:
```
cd buildroot
make menuconfig
```
In Target Options select Target Architecture as MIPS (big endian) and Target Architecture Variant as mips 32r2.
In Toolchain select GCC Compiler version as gcc 4.7.x.
Save and exit.

Build:
```
make
```
----
2) Build Caraboot image

Download:
```
cd your_work_dir
git clone https://github.com/8devices/Caraboot-nand.git
cd Caraboot-nand/
git checkout caraboot
```


Build:
```
make rambutan_defconfig
CROSS_COMPILE=your_work_dir/buildroot/output/host/usr/bin/mips-linux- make
```

The bootloader binary will be saved to ```u-boot.bin``` file.
You can now use this file to upgrade your bootloader on Rambutan board.


