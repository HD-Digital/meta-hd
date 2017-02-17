PREFERRED_VERSION_linux-hd = "4.7.4"

IMAGE_CLASSES += "hd-emmc_image_multi"
IMAGE_FSTYPES += "hd-emmc"
IMAGE_ROOTFS_SIZE ?= "294912"

TARGET_ARCH = "arm"
DEFAULTTUNE ?= "armv7ahf-neon"

require conf/machine/include/arm/arch-armv7a.inc
require conf/machine/include/hd.inc