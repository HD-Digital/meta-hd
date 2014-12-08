MACHINE_KERNEL_PR = "r1"

KERNEL_MODULE_AUTOLOAD += "xfs"

# We know about rtc
MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"

IMAGE_CMD_ubi_prepend = " \
	rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
	"

IMAGE_CMD_tar_prepend = " \
	rm -Rf ${DEPLOY_DIR_IMAGE}/boot; \
	mkdir ${DEPLOY_DIR_IMAGE}/boot; \
	gzip -9c ${DEPLOY_DIR_IMAGE}/vmlinux-${MACHINE}.bin > ${DEPLOY_DIR_IMAGE}/boot/vmlinux.gz; \
	echo "LABEL=sdcard" > ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	echo "KERNEL=/boot/vmlinux.gz" >> ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	echo "PRIORITY=10" >> ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
	"

IMAGE_CMD_tar_append = "; \
	tar -rf ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.rootfs.tar -C ${DEPLOY_DIR_IMAGE} boot; \
	rm -Rf ${DEPLOY_DIR_IMAGE}/boot; \
	"

MACHINE_ESSENTIAL_EXTRA_RDEPENDS = "\
	hd-dvb-modules-${MACHINE} \
	"

MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS = "\
	kernel-module-cdfs \
	"

MACHINE_EXTRA_RRECOMMENDS = " \
	gst-plugin-dvbmediasink \
	ntfs-3g \
	"

DVBMEDIASINK_CONFIG = "--with-wma --with-wmv --with-pcm --with-dts --with-eac3"

TARGET_ARCH = "mipsel"
DEFAULTTUNE ?= "mips32el"

PREFERRED_PROVIDER_virtual/kernel = "linux-hd"

KERNEL_IMAGETYPE = "vmlinux"
KERNEL_OUTPUT = "${KERNEL_IMAGETYPE}"
KERNEL_CONSOLE = "null"
SERIAL_CONSOLE ?= ""

IMAGE_FSTYPES ?= "ubi"

UBI_VOLNAME = "rootfs"

IMAGEDIR ?= "${MACHINE}"

IMAGE_CMD_ubi_append = " \
	mkdir -p ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}; \
	cp ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.rootfs.ubi ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/rootfs.bin; \
	gzip -9c ${DEPLOY_DIR_IMAGE}/vmlinux-${MACHINE}.bin > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/kernel.bin; \
	echo ${DISTRO_NAME}-${DISTRO_VERSION}-${DATE} > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/imageversion; \
	echo "rename this file to 'force' to force an update without confirmation" > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/noforce; \
	cd ${DEPLOY_DIR_IMAGE}; \
	zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${IMAGEDIR}/*; \
	rm -Rf ${IMAGEDIR}; \
	"

MACHINE_FEATURES += "alsa usbhost wlan kernelwifi extrakernelwifi 3dtv switchoff osdposition hdmicec"

require conf/machine/include/tune-mips32.inc
