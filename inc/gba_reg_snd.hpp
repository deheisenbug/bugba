/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * Sound registers
 */

#include <cstdint>

#include "gba_types.hpp"
#include "gba_reg_common.hpp"

#pragma once

namespace gba::dtypes {
  /**
   * Sound control registers
   */
  enum class snd_sweep_dir_t : uint16_t {
    inc = 0,
    dec = 1,
  };
  class snd_sweep_t {
    public:
        uint16_t shift : 3 = 0;
        snd_sweep_dir_t dir : 1 = snd_sweep_dir_t::dec;
        uint16_t time : 3 = 0;
        uint16_t : 9;
  };
  using reg_snd_sweep_t = reg_t<snd_sweep_t,reg16_t>;
  
  enum class snd_duty_t : uint16_t {
    pct12 = 0,
    pct25 = 1,
    pct50 = 2,
    pct75 = 3,
  };
  enum class snd_env_dir_t : uint16_t {
    dec = 0,
    inc = 1,
  };
  class snd_cnt_t {
    public:
        uint16_t len : 6 = 0;
        snd_duty_t duty : 2 = snd_duty_t::pct12;
        uint16_t env_step : 3 = 0;
        snd_env_dir_t env_dir : 1 = snd_env_dir_t::dec;
        uint16_t env_vol : 4 = 0;
  };
  using reg_snd_cnt_t = reg_t<snd_cnt_t,reg16_t>;
  class snd_freq_t {
    public:
        uint16_t rate : 11 = 0;
        uint16_t : 3;
        uint16_t timed : 1 = 0;
        uint16_t reset : 1 = 0;
  };
  using reg_snd_freq_t = reg_t<snd_freq_t,reg16_t>;
  enum class snd_wavedim_t : uint16_t {
    one_32 = 0,
    two_64 = 1,
  };
  class snd_sel_t {
    public:
        uint16_t : 5;
        snd_wavedim_t wavedim : 1 = snd_wavedim_t::one_32;
        uint16_t banknr : 1 = 0;
        uint16_t enabled : 1 = 0;
        uint16_t : 8;
  };
  using reg_snd_sel_t = reg_t<snd_sel_t,reg16_t>;
  enum class snd_lvl_t : uint16_t {
    off = 0,
    pct100 = 1,
    pct50 = 2,
    pct25 = 3,
  };
  class snd_vol_t {
    public:
        uint16_t len : 8 = 0;
        uint16_t : 5;
        snd_lvl_t lvl : 2  = snd_lvl_t::off;
        uint16_t force75 : 1 = 0;
  };
  using reg_snd_vol_t = reg_t<snd_vol_t,reg16_t>;
  enum class snd_step_t : uint16_t {
    bits15 = 0,
    bits7 = 1,
  };
  class snd_noise_t {
    public:
        uint16_t ratio : 3 = 0;
        snd_step_t step : 1 = snd_step_t::bits15;
        uint16_t shift : 4 = 0;
        uint16_t : 6;
        uint16_t timed : 1 = 0;
        uint16_t reset : 1 = 0;
  };
  using reg_snd_noise_t = reg_t<snd_noise_t,reg16_t>;
  class snd_dmg_cnt_t {
    public:
        uint16_t vol_r : 3 = 0;
        uint16_t : 1;
        uint16_t vol_l : 3 = 0;
        uint16_t : 1;
        uint16_t ch1_r_en : 1 = 0;
        uint16_t ch2_r_en : 1 = 0;
        uint16_t ch3_r_en : 1 = 0;
        uint16_t ch4_r_en : 1 = 0;
        uint16_t ch1_l_en : 1 = 0;
        uint16_t ch2_l_en : 1 = 0;
        uint16_t ch3_l_en : 1 = 0;
        uint16_t ch4_l_en : 1 = 0;
  };
  enum class snd_dmglvl_t : uint16_t {
    pct25 = 0,
    pct50 = 1,
    pct100 = 2,
  };
  enum class snd_dslvl_t : uint16_t {
    pct50 = 0,
    pct100 = 1,
  };
  using reg_snd_dmg_cnt_t = reg_t<snd_dmg_cnt_t,reg16_t>;
  class snd_ds_cnt_t {
    public:
        snd_dmglvl_t dmglvl : 2 = snd_dmglvl_t::pct25;
        snd_dslvl_t cha_lvl : 1 = snd_dslvl_t::pct50;
        snd_dslvl_t chb_lvl : 1 = snd_dslvl_t::pct50;
        uint16_t : 4;
        uint16_t cha_r_en : 1 = 0;
        uint16_t cha_l_en : 1 = 0;
        uint16_t cha_timer : 1 = 0;
        uint16_t cha_reset : 1 = 0;
        uint16_t chb_r_en : 1 = 0;
        uint16_t chb_l_en : 1 = 0;
        uint16_t chb_timer : 1 = 0;
        uint16_t chb_reset : 1 = 0;
  };
  using reg_snd_ds_cnt_t = reg_t<snd_ds_cnt_t,reg16_t>;
  class snd_stat_t {
    public:
        uint16_t ch1_active : 1 = 0;
        uint16_t ch2_active : 1 = 0;
        uint16_t ch3_active : 1 = 0;
        uint16_t ch4_active : 1 = 0;
        uint16_t : 3;
        uint16_t snd_en: 1 = 0;
        uint16_t : 8;
  };
  enum class snd_ampres_t : uint16_t {
    bits9 = 0,
    bits8 = 1,
    bits7 = 2,
    bits6 = 3,
  };
  using reg_snd_stat_t = reg_t<snd_stat_t,reg16_t>;
  class snd_bias_t {
    public:
        uint16_t : 1;
        uint16_t bias_lvl : 9  = 0;
        uint16_t : 4;
        snd_ampres_t ampres : 2 = snd_ampres_t::bits9;
  };
  using reg_snd_bias_t = reg_t<snd_bias_t,reg16_t>;
}
