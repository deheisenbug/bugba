/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * misc registers
 */

#include <cstdint>

#include "gba_types.hpp"
#include "gba_reg_common.hpp"

#pragma once


namespace gba::dtypes {
  /**
   * Memory waitstate control regsiter
   */
  class wait_ctrl_t {
    enum class wait_cycle_def_t : uint16_t {
      cycle4 = 0,
      cycle3 = 1,
      cycle2 = 2,
      cycle8 = 3,
    };
    enum class wait_cycle0_2nd_t : uint16_t {
      cycle2 = 0,
      cycle1 = 1,
    };
    enum class wait_cycle1_2nd_t : uint16_t {
      cycle4 = 0,
      cycle1 = 1,
    };
    enum class wait_cycle2_2nd_t : uint16_t {
      cycle8 = 0,
      cycle1 = 1,
    };
    enum class phy_term_freq_t : uint16_t {
      disable = 0,
      f4_19Mhz = 1,
      f8_39Mhz = 2,
      f16_78Mhz = 3,
    };
    enum class gpak_type_t : uint16_t {
      gba = 0,
      gbc = 1,
    };
    public:
      wait_cycle_def_t sram : 2 = wait_cycle_def_t::cycle2;
      wait_cycle_def_t rom0_1st : 2 = wait_cycle_def_t::cycle2;
      wait_cycle0_2nd_t rom0_2nd : 1 = wait_cycle0_2nd_t::cycle2;
      wait_cycle_def_t rom1_1st : 2 = wait_cycle_def_t::cycle2;
      wait_cycle1_2nd_t rom1_2nd : 1 = wait_cycle1_2nd_t::cycle4;
      wait_cycle_def_t rom2_1st : 2 = wait_cycle_def_t::cycle2;
      wait_cycle2_2nd_t rom2_2nd : 1 = wait_cycle2_2nd_t::cycle8;
      phy_term_freq_t freq : 2 = phy_term_freq_t::disable;
      uint16_t : 1 ;
      uint16_t gpak_pref_en : 1 = 0;
      gpak_type_t gpak_type : 1 = gpak_type_t::gba;
  };
  using reg_wait_ctrl_t = reg_t<wait_ctrl_t,reg16_t>;
  /**
   * First or succesive boot indicator register
   */
  class post_flag_t {
    enum class post_type_t : uint8_t {
      boot_1st = 0,
      boot_later = 1,
    };
    public:
      post_type_t boot_nr : 1 = post_type_t::boot_1st;
      uint8_t : 7 ;
  };
  using reg_post_flag_t = reg_t<post_flag_t,reg8_t>;
  /**
   * System halt type control register
   */
  class halt_ctrl_t {
    enum class halt_type_t : uint8_t {
      halt = 0,
      stop = 1,
    };
    public:
      uint8_t : 7 ;
      halt_type_t halt_type : 1 = halt_type_t::halt;
  };
  using reg_halt_ctrl_t = reg_t<halt_ctrl_t,reg8_t>;
};
