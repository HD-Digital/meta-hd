PREFERRED_VERSION_linux-hd = "4.4.0"

DVBMEDIASINK_CONFIG += "--with-h265 --with-vb8 --with-vb9 --with-opus"

IMAGE_CLASSES += "hd-emmc_image_single"
IMAGE_FSTYPES += "hd-emmc"
IMAGE_ROOTFS_SIZE ?= "196608"

TARGET_ARCH = "arm"
DEFAULTTUNE = "armv7ahf-neon"

IMAGE_CMD_hd-emmc_append = "\
	mkdir -p ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}; \
	cp ${DEPLOY_DIR_IMAGE}/${IMAGE_NAME}.emmc.img ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/disk.img; \
	echo ${DISTRO_NAME}-${DISTRO_VERSION}-${DATE} > ${DEPLOY_DIR_IMAGE}/${IMAGEDIR}/imageversion; \
	cd ${DEPLOY_DIR_IMAGE}; \
	zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${IMAGEDIR}/*; \
	rm -Rf ${IMAGEDIR}; \
	rm -Rf ${DEPLOY_DIR_IMAGE}/boot; \
	"

IMAGE_CMD_hd_emmc_prepend = " \
	rm -Rf ${DEPLOY_DIR_IMAGE}/boot; \
	mkdir ${DEPLOY_DIR_IMAGE}/boot; \
	echo "LABEL=${DISTRO_NAME}${DISTRO_VERSION}" > ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	echo "KERNEL=/boot/kernel.bin" >> ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	echo "PRIORITY=100" >> ${DEPLOY_DIR_IMAGE}/boot/boot.cfg; \
	rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
	"

require conf/machine/include/arm/arch-armv7a.inc
require conf/machine/include/hd.inc
