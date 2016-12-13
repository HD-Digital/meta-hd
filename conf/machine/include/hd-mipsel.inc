IMAGE_FSTYPES += "ubi"
UBI_VOLNAME = "rootfs"

TARGET_ARCH = "mipsel"
DEFAULTTUNE ?= "mips32el"

PREFERRED_PROVIDER_virtual/kernel = "linux-hd"

IMAGEVERSION := "${DISTRO_NAME}-${DISTRO_VERSION}-${DATE}"
IMAGEVERSION[vardepsexclude] = "DATE"

IMAGE_CMD_ubi_append = " \
	mkdir -p ${IMGDEPLOYDIR}/${IMAGEDIR}; \
	cp ${IMGDEPLOYDIR}/${IMAGE_NAME}.rootfs.ubi ${IMGDEPLOYDIR}/${IMAGEDIR}/rootfs.bin; \
	gzip -9c ${IMGDEPLOYDIR}/vmlinux-${MACHINE}.bin > ${IMGDEPLOYDIR}/${IMAGEDIR}/kernel.bin; \
	echo "${IMAGEVERSION}" > ${IMGDEPLOYDIR}/${IMAGEDIR}/imageversion; \
	echo "rename this file to 'force' to force an update without confirmation" > ${IMGDEPLOYDIR}/${IMAGEDIR}/noforce; \
	cd ${IMGDEPLOYDIR}; \
	zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${IMAGEDIR}/*; \
	rm -Rf ${IMAGEDIR}; \
	rm -Rf ${IMGDEPLOYDIR}/boot; \
	"

IMAGE_CMD_ubi_prepend = " \
	rm -Rf ${IMGDEPLOYDIR}/boot; \
	mkdir ${IMGDEPLOYDIR}/boot; \
	echo "LABEL=${DISTRO_NAME}${DISTRO_VERSION}" > ${IMGDEPLOYDIR}/boot/boot.cfg; \
	echo "KERNEL=/boot/vmlinux.gz" >> ${IMGDEPLOYDIR}/boot/boot.cfg; \
	echo "PRIORITY=100" >> ${IMGDEPLOYDIR}/boot/boot.cfg; \
	rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
	"

require conf/machine/include/tune-mips32.inc
require conf/machine/include/hd.inc
