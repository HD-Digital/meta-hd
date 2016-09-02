DESCRIPTION = "Linux kernel for ${MACHINE}"
SECTION = "kernel"
LICENSE = "GPLv2"

KERNEL_RELEASE = "4.0.1"
COMPATIBLE_MACHINE = "hd+"
MACHINE_KERNEL_PR_append = ".1"

SRC_URI[md5sum] = "c274792d088cd7bbfe7fe5a76bd798d8"
SRC_URI[sha256sum] = "6fd63aedd69b3b3b28554cabf71a9efcf05f10758db3d5b99cfb0580e3cde24c"

LIC_FILES_CHKSUM = "file://${WORKDIR}/linux-${PV}/COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

# By default, kernel.bbclass modifies package names to allow multiple kernels
# to be installed in parallel. We revert this change and rprovide the versioned
# package names instead, to allow only one kernel to be installed.
PKG_kernel-base = "kernel-base"
PKG_kernel-image = "kernel-image"
RPROVIDES_kernel-base = "kernel-${KERNEL_VERSION}"
RPROVIDES_kernel-image = "kernel-image-${KERNEL_VERSION}"

SRC_URI += "http://downloads.mutant-digital.net/linux-${PV}.tar.gz \
	file://defconfig \
	file://add-dmx-source-timecode.patch \
	file://iosched-slice_idle-1.patch \
	file://0001-bcmgenet.patch \
	file://0002-add-brcm-chips.patch \
	file://0003-fix-cpu-probe-disable-RIXIEX-check-on-BCM7584.patch \
	file://kernel-gcc6.patch \
	"

inherit kernel machine_kernel_pr

S = "${WORKDIR}/linux-${PV}"

export OS = "Linux"

require linux-hd-${ARCH}.inc
