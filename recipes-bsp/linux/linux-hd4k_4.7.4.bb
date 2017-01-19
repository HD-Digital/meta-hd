DESCRIPTION = "Linux kernel for ${MACHINE}"
SECTION = "kernel"
LICENSE = "GPLv2"

KERNEL_RELEASE = "4.7.4"
COMPATIBLE_MACHINE = "hd+|vs+"
MACHINE_KERNEL_PR_append = ".0"

SRC_URI[md5sum] = "ab37f1c0c601a6bfd2d35dc356b40f0e"
SRC_URI[sha256sum] = "1433e9983866903cb25a2a4d846c84b3e420b3410d56dde4c2b2bf92a8dcdba9"

LIC_FILES_CHKSUM = "file://${WORKDIR}/linux-${PV}/COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

# By default, kernel.bbclass modifies package names to allow multiple kernels
# to be installed in parallel. We revert this change and rprovide the versioned
# package names instead, to allow only one kernel to be installed.
PKG_kernel-base = "kernel-base"
PKG_kernel-image = "kernel-image"
RPROVIDES_kernel-base = "kernel-${KERNEL_VERSION}"
RPROVIDES_kernel-image = "kernel-image-${KERNEL_VERSION}"

SRC_URI += "http://downloads.mutant-digital.net/linux-${PV}-${ARCH}.tar.gz \
	file://reserve_dvb_adapter_0.patch \
	file://blacklist_mmc0.patch \
	file://defconfig \
	file://findkerneldevice.py \
	"

inherit kernel machine_kernel_pr

S = "${WORKDIR}/linux-${PV}"

export OS = "Linux"

FILES_kernel-image = "/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE} /${KERNEL_IMAGEDEST}/findkerneldevice.py"

KERNEL_IMAGETYPE = "zImage"
KERNEL_OUTPUT = "arch/${ARCH}/boot/${KERNEL_IMAGETYPE}"
KERNEL_IMAGEDEST = "tmp"
KERNEL_OBJECT_SUFFIX = "ko"
KERNEL_CONSOLE = "null"
SERIAL_CONSOLE ?= ""

kernel_do_install_append() {
        install -d ${D}/${KERNEL_IMAGEDEST}
        install -m 0755 ${KERNEL_OUTPUT} ${D}/${KERNEL_IMAGEDEST}
	install -m 0755 ${WORKDIR}/findkerneldevice.py ${D}/${KERNEL_IMAGEDEST}
}

pkg_postinst_kernel-image() {
	if [ "x$D" == "x" ]; then
		if [ -f /${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE} ] ; then
			python /${KERNEL_IMAGEDEST}/findkerneldevice.py
			dd if=/${KERNEL_IMAGEDEST}/${KERNEL_IMAGETYPE} of=/dev/kernel
		fi
	fi
    true
}

do_rm_work() {
}