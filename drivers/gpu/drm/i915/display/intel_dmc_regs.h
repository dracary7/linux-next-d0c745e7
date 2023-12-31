/* SPDX-License-Identifier: MIT */
/*
 * Copyright © 2022 Intel Corporation
 */

#ifndef __INTEL_DMC_REGS_H__
#define __INTEL_DMC_REGS_H__

#include "i915_reg_defs.h"

#define DMC_PROGRAM(addr, i)	_MMIO((addr) + (i) * 4)
#define DMC_SSP_BASE_ADDR_GEN9	0x00002FC0
#define DMC_HTP_ADDR_SKL	0x00500034
#define DMC_SSP_BASE		_MMIO(0x8F074)
#define DMC_HTP_SKL		_MMIO(0x8F004)
#define DMC_LAST_WRITE		_MMIO(0x8F034)
#define DMC_LAST_WRITE_VALUE	0xc003b400
#define DMC_MMIO_START_RANGE	0x80000
#define DMC_MMIO_END_RANGE	0x8FFFF
#define SKL_DMC_DC3_DC5_COUNT	_MMIO(0x80030)
#define SKL_DMC_DC5_DC6_COUNT	_MMIO(0x8002C)
#define BXT_DMC_DC3_DC5_COUNT	_MMIO(0x80038)
#define TGL_DMC_DEBUG_DC5_COUNT	_MMIO(0x101084)
#define TGL_DMC_DEBUG_DC6_COUNT	_MMIO(0x101088)
#define DG1_DMC_DEBUG_DC5_COUNT	_MMIO(0x134154)

#define TGL_DMC_DEBUG3		_MMIO(0x101090)
#define DG1_DMC_DEBUG3		_MMIO(0x13415c)

#endif /* __INTEL_DMC_REGS_H__ */
