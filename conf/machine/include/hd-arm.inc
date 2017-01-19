DVBMEDIASINK_CONFIG += "--with-h265 --with-vb8 --with-vb9 --with-opus"

IMAGE_CLASSES += "hd-emmc_image_multi"
IMAGE_FSTYPES += "hd-emmc"
IMAGE_ROOTFS_SIZE ?= "196608"

TARGET_ARCH = "arm"
DEFAULTTUNE ?= "armv7ahf-neon"

PREFERRED_PROVIDER_virtual/kernel = "linux-hd4k"

IMAGEVERSION := "${DISTRO_NAME}-${DISTRO_VERSION}-${DATE}"
IMAGEVERSION[vardepsexclude] = "DATE"

IMAGE_CMD_hd-emmc_append = "\
	mkdir -p ${IMGDEPLOYDIR}/${IMAGEDIR}; \
	cp ${IMGDEPLOYDIR}/${IMAGE_NAME}.emmc.img ${IMGDEPLOYDIR}/${IMAGEDIR}/disk.img; \
	echo "${IMAGEVERSION}" > ${IMGDEPLOYDIR}/${IMAGEDIR}/imageversion; \
	cd ${IMGDEPLOYDIR}; \
	zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${IMAGEDIR}/*; \
	rm -Rf ${IMAGEDIR}; \
	rm -Rf ${IMGDEPLOYDIR}/boot; \
	"

IMAGE_CMD_hd_emmc_prepend = " \
	rm -Rf ${IMGDEPLOYDIR}/boot; \
	mkdir ${IMGDEPLOYDIR}/boot; \
	echo "LABEL=${DISTRO_NAME}${DISTRO_VERSION}" > ${IMGDEPLOYDIR}/boot/boot.cfg; \
	echo "KERNEL=/boot/kernel.bin" >> ${IMGDEPLOYDIR}/boot/boot.cfg; \
	echo "PRIORITY=100" >> ${IMGDEPLOYDIR}/boot/boot.cfg; \
	rm -Rf ${IMAGE_ROOTFS}/tmp/*; \
	"

require conf/machine/include/arm/arch-armv7a.inc
require conf/machine/include/hd.inc
