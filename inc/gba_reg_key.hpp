/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * keypad registers
 */

#include <cstdint>

#include "gba_types.hpp"
#include "gba_reg_common.hpp"

#pragma once

namespace gba::dtypes {
  /**
   * keypad input register
   */
  enum class key_state_t : uint16_t {
    down = 0,
    up = 1,
  };
  class key_inp_t {
    public:
      key_state_t btn_a : 1 = key_state_t::up;
      key_state_t btn_b : 1 = key_state_t::up;
      key_state_t btn_sel : 1 = key_state_t::up;
      key_state_t btn_sta : 1 = key_state_t::up;
      key_state_t btn_r : 1 = key_state_t::up;
      key_state_t btn_l : 1 = key_state_t::up;
      key_state_t btn_u : 1 = key_state_t::up;
      key_state_t btn_d : 1 = key_state_t::up;
      key_state_t btn_rs : 1 = key_state_t::up;
      key_state_t btn_ls : 1 = key_state_t::up;
      uint16_t : 6;
  };
  using reg_key_inp_t = reg_t<key_inp_t,reg16_t>;
  /**
   * Keypad interrupt control register
   */
  class key_ctrl_t {
    enum class key_irq_t : uint16_t {
      logic_or = 0,
      logic_and = 1,
    };
    public:
      uint16_t btn_a : 1 = 0;
      uint16_t btn_b : 1 = 0;
      uint16_t btn_sel : 1 = 0;
      uint16_t btn_sta : 1 = 0;
      uint16_t btn_r : 1 = 0;
      uint16_t btn_l : 1 = 0;
      uint16_t btn_u : 1 = 0;
      uint16_t btn_d : 1 = 0;
      uint16_t btn_rs : 1 = 0;
      uint16_t btn_ls : 1 = 0;
      uint16_t : 4;
      uint16_t irq_en : 1 = 0;
      key_irq_t irq_ctrl : 1 = key_irq_t::logic_or;
  };
  using reg_key_ctrl_t = reg_t<key_ctrl_t,reg16_t>;
}
