MACHINE_KERNEL_PR = "r1"

KERNEL_MODULE_AUTOLOAD += "xfs"

# We know about rtc
MACHINE_FEATURES_BACKFILL_CONSIDERED = "rtc"
PREFERRED_PROVIDER_virtual/egl = "v3d-libgles-${MACHINE}"
PREFERRED_PROVIDER_virtual/libgles2 = "v3d-libgles-${MACHINE}"
PREFERRED_PROVIDER_virtual/kernel_mipsel = "linux-hd"
PREFERRED_PROVIDER_virtual/kernel_arm = "linux-hd4k"

PACKAGECONFIG_GL_pn-qtbase_arm = "gles2 eglfs"

IMAGEVERSION := "${DISTRO_NAME}-${DISTRO_VERSION}-${DATE}"
IMAGEVERSION[vardepsexclude] = "DATE"

MACHINE_ESSENTIAL_EXTRA_RDEPENDS = " \
	hd-dvb-modules-${MACHINE} \
	"

MACHINE_ESSENTIAL_EXTRA_RRECOMMENDS = " \
	kernel-module-cdfs \
	"

MACHINE_EXTRA_RRECOMMENDS = " \
	gst-plugin-dvbmediasink \
	ntfs-3g \
	"

MACHINE_FEATURES += "alsa usbhost wlan 3dtv switchoff osdposition hdmicec"

DVBMEDIASINK_CONFIG_arm = "--with-h265 --with-vb8 --with-vb9 --with-opus --with-wma --with-wmv --with-pcm --with-dts --with-eac3"
DVBMEDIASINK_CONFIG_mipsel = "--with-wma --with-wmv --with-pcm --with-dts --with-eac3 \
	${@bb.utils.contains('MACHINE', 'hd11', '--with-h265 --with-vb6 --with-vb8 --with-spark' , '', d)} \
	${@bb.utils.contains('MACHINE', 'hd1265', '--with-h265 --with-vb6 --with-vb8 --with-spark' , '', d)} \
	${@bb.utils.contains('MACHINE', 'hd1500', '--with-h265 --with-vb6 --with-vb8 --with-spark' , '', d)} \
	${@bb.utils.contains('MACHINE', 'hd530c', '--with-h265 --with-vb6 --with-vb8 --with-spark' , '', d)} \
	${@bb.utils.contains('MACHINE', 'vs1000', '--with-h265 --with-vb6 --with-vb8 --with-spark' , '', d)} \
	"

IMAGEDIR ?= "${MACHINE}"
IMGDEPLOYDIR ?= "${DEPLOY_DIR_IMAGE}"

# ARM

IMAGE_CMD_hd-emmc_append_arm = "\
	mkdir -p ${IMGDEPLOYDIR}/${IMAGEDIR}; \
	cp ${IMGDEPLOYDIR}/${IMAGE_NAME}.emmc.img ${IMGDEPLOYDIR}/${IMAGEDIR}/disk.img; \
	echo "${IMAGEVERSION}" > ${IMGDEPLOYDIR}/${IMAGEDIR}/imageversion; \
	cd ${IMGDEPLOYDIR}; \
	zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${IMAGEDIR}/*; \
	rm -Rf ${IMAGEDIR}; \
	"

# MIPSEL

IMAGE_CMD_ubi_append_mipsel = " \
	mkdir -p ${IMGDEPLOYDIR}/${IMAGEDIR}; \
	cp ${IMGDEPLOYDIR}/${IMAGE_NAME}.rootfs.ubi ${IMGDEPLOYDIR}/${IMAGEDIR}/rootfs.bin; \
	gzip -9c ${DEPLOY_DIR_IMAGE}/vmlinux-${MACHINE}.bin > ${IMGDEPLOYDIR}/${IMAGEDIR}/kernel.bin; \
	echo "${IMAGEVERSION}" > ${IMGDEPLOYDIR}/${IMAGEDIR}/imageversion; \
	echo "rename this file to 'force' to force an update without confirmation" > ${IMGDEPLOYDIR}/${IMAGEDIR}/noforce; \
	cd ${IMGDEPLOYDIR}; \
	zip ${DISTRO_NAME}-${DISTRO_VERSION}-${MACHINE}_usb.zip ${IMAGEDIR}/*; \
	rm -Rf ${IMAGEDIR}; \
	rm -Rf ${IMGDEPLOYDIR}/boot; \
	"

require conf/machine/include/hd-wifi.inc