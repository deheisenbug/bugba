/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * irq registers
 */

#include <cstdint>
#include "gba_types.hpp"
#include "gba_reg_common.hpp"

#pragma once

namespace gba::dtypes {
  /**
   * Intterupt source enable register
   */
  class irq_en_t {
    public:
      uint16_t lcd_vbl : 1 = 0;
      uint16_t lcd_hbl : 1 = 0;
      uint16_t lcd_vcnt : 1 = 0;
      uint16_t timer0_ovfl : 1 = 0;
      uint16_t timer1_ovfl : 1 = 0;
      uint16_t timer2_ovfl : 1 = 0;
      uint16_t timer3_ovfl : 1 = 0;
      uint16_t serial_com : 1 = 0;
      uint16_t dma0 : 1 = 0;
      uint16_t dma1 : 1 = 0;
      uint16_t dma2 : 1 = 0;
      uint16_t dma3 : 1 = 0;
      uint16_t key_inp : 1 = 0;
      uint16_t gpak_ext : 1 = 0;
      uint16_t : 2;
  };
  using reg_irq_en_t = reg_t<irq_en_t,reg16_t>;
  /**
   * Interrupt flag and acknowledgement register
   */
  class irq_flags_t {
    public:
      uint16_t lcd_vbl : 1 = 0;
      uint16_t lcd_hbl : 1 = 0;
      uint16_t lcd_vcnt : 1 = 0;
      uint16_t timer0_ovfl : 1 = 0;
      uint16_t timer1_ovfl : 1 = 0;
      uint16_t timer2_ovfl : 1 = 0;
      uint16_t timer3_ovfl : 1 = 0;
      uint16_t serial_com : 1 = 0;
      uint16_t dma0 : 1 = 0;
      uint16_t dma1 : 1 = 0;
      uint16_t dma2 : 1 = 0;
      uint16_t dma3 : 1 = 0;
      uint16_t key_inp : 1 = 0;
      uint16_t gpak_ext : 1 = 0;
      uint16_t : 2;
  };
  using reg_irq_flags_t = reg_t<irq_flags_t,reg16_t>;
  /**
   * Interrupt master enable register
   */
  class irq_men_t {
  public:
    uint16_t enable : 1 = 0;
    uint16_t : 15 ;
  };
  using reg_irq_men_t = reg_t<irq_men_t,reg16_t>;
}
