DESCRIPTION = "Linux kernel for ${MACHINE}"
SECTION = "kernel"
LICENSE = "GPLv2"

KERNEL_RELEASE = "4.4.0"
COMPATIBLE_MACHINE = "hd+"
MACHINE_KERNEL_PR_append = ".0"

SRC_URI[md5sum] = "277b70cbe8674cb8c460bb4c18d025c4"
SRC_URI[sha256sum] = "263cd23032db323f7d7c4bb7f1ac0ff5787f0ceaa3fdfc8305148c86f286a6b1"

LIC_FILES_CHKSUM = "file://${WORKDIR}/linux-${PV}/COPYING;md5=d7810fab7487fb0aad327b76f1be7cd7"

# By default, kernel.bbclass modifies package names to allow multiple kernels
# to be installed in parallel. We revert this change and rprovide the versioned
# package names instead, to allow only one kernel to be installed.
PKG_kernel-base = "kernel-base"
PKG_kernel-image = "kernel-image"
RPROVIDES_kernel-base = "kernel-${KERNEL_VERSION}"
RPROVIDES_kernel-image = "kernel-image-${KERNEL_VERSION}"

SRC_URI += "http://downloads.mutant-digital.net/linux-${PV}.tar.gz \
	file://reserve_dvb_adapter_0.patch \
	file://defconfig \
	file://findkerneldevice.py \ 
	"

inherit kernel machine_kernel_pr

S = "${WORKDIR}/linux-${PV}"

export OS = "Linux"

require linux-hd-${ARCH}.inc
