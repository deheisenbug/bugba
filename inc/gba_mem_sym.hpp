/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * Memory regsiter declarations
 * Symbols are currently defined/set in a linker script
 */

#include "gba_mem_regs.hpp"
#include "gba_reg_irq.hpp"

#pragma once

extern gba::hw::color_palette mem_pal;
extern gba::hw::video_memory mem_vram;
extern gba::hw::oam_memory mem_oam;
extern gba::hw::disp_control reg_disp;
extern gba::hw::snd_control reg_snd;
extern gba::hw::key_control reg_key;
extern gba::hw::irq_control reg_irq;
extern gba::hw::pwr_control reg_pwr;
extern gba::dtypes::reg_irq_flags_t bios_irqflags;
extern volatile fn_ptr irq_vector;
