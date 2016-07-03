PREFERRED_VERSION_linux-hd = "4.0.1"

IMAGE_FSTYPES += "ubi"
UBI_VOLNAME = "rootfs"

TARGET_ARCH = "mipsel"
DEFAULTTUNE ?= "mips32el"

IMAGE_CMD_ubi_append = " \
	mkdir -p ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}; \
	cp ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.rootfs.ubi ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/rootfs.bin; \
	gzip -9c ${DEPLOY_DIR_IMAGE}/vmlinux-${MACHINE}.bin > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/kernel.bin; \
	echo ${DISTRO_NAME}-${DISTRO_VERSION}-${DATE} > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/imageversion; \
	echo "rename this file to 'force' to force an update without confirmation" > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/noforce; \
	cd ${DEPLOY_DIR_IMAGE}; \
	zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${IMAGEDIR}/*; \
	rm -Rf ${IMAGEDIR}; \
	rm -Rf ${DEPLOY_DIR_IMAGE}/boot; \
	"

IMAGE_CMD_ubi_prepend = " \
	rm -Rf ${DEPLOY_DIR_IMAGE}/boot; \
	mkdir ${DEPLOY_DIR_IMAGE}/boot; \
	echo "LABEL=${DISTRO_NAME}${DISTRO_VERSION}" > ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	echo "KERNEL=/boot/vmlinux.gz" >> ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	echo "PRIORITY=100" >> ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
	"

require conf/machine/include/tune-mips32.inc
require conf/machine/include/hd.inc