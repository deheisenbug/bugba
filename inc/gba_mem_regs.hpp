/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * memory regions and registers
 */

#include <cstdint>
#include <type_traits>

#include <gba_reg_all.hpp>

#pragma once

namespace gba::hw {
  using namespace gba::dtypes;
  /**
   * color palette memory access container
   */
  packed(class) color_palette : public io_reg {
    public:
      packed(struct) p16x16_t {
        reg_rgb16_t col[16][16];
      };
      packed(struct) p256x1_t {
        reg_rgb16_t col[256];
      };
      union layout_t {
        p16x16_t p16x16;
        p256x1_t p256x1;
      } packed();
      layout_t bg;
      layout_t obj;
  };
  static_assert(
    std::is_standard_layout_v<color_palette>
  );
  /**
   * video memory access container
   */
  packed(class) video_memory : public io_reg {
    packed(struct) m012_screen_block {
      bg_screen_block_t se[32];
      bg_screen_block_t pad0[16];
    };
    static_assert(
      sizeof(m012_screen_block) == 98304
    );
    packed(struct) m012_char_block {
      bg_char_block_t bg[4];
      bg_char_block_t obj[2];
    };
    static_assert(
      sizeof(m012_char_block) == 98304
    );
    union m012_t {
      m012_char_block char_block;
      m012_screen_block screen_block;
    } packed();
    static_assert(
      sizeof(m012_t) == 98304
    );
    packed(struct) m3_t {
      reg_rgb16_t pix[160][240];
      reg_rgb16_t pad0[160][16];
      bg_char_block_t obj;
    };
    static_assert(
      sizeof(m3_t) == 98304
    );
    packed(struct) m4_t {
      reg_m4_sub_t frame0[160][120];
      reg_m4_sub_t pad0[160][8];
      reg_m4_sub_t frame1[160][120];
      reg_m4_sub_t pad1[160][8];
      bg_char_block_t obj;
    };
    static_assert(
      sizeof(m4_t) == 98304
    );
  public:
    union {
      m012_t mode0;
      m012_t mode1;
      m012_t mode2;
      m3_t mode3;
      m4_t mode4;
      // TODO: m5
    } packed();
  };
  static_assert(
    std::is_standard_layout_v<video_memory>
  );
  static_assert(
    sizeof(video_memory) == 98304
  );
  /**
   * Object attribute memory access container
   */
  packed(class) oam_memory : public io_reg {
    public:
      oam_obj_t obj[128];
  };
  static_assert(
    std::is_standard_layout_v<oam_memory>
  );
  static_assert(
    sizeof(oam_memory) == 0x400
  );
  /**
   * Display cotrol access container
   */
  packed(class) align(4) disp_control : public io_reg {
    public:
      reg_lcd_conf_t lcd_conf;
      uint16_t unused_00;
      reg_lcd_status_t lcd_status;
      uint8_t lcd_vcount;
      uint8_t unused_01;
      reg_bg_conf_t bg0_conf;
      reg_bg_conf_t bg1_conf;
      reg_bg_conf_t bg2_conf;
      reg_bg_conf_t bg3_conf;
      reg_bg_scroll_t bg0_scroll;
      reg_bg_scroll_t bg1_scroll;
      reg_bg_scroll_t bg2_scroll;
      reg_bg_scroll_t bg3_scroll;
      reg_bg_rotscl_t bg2_rotscl_dx;
      reg_bg_rotscl_t bg2_rotscl_dmx;
      reg_bg_rotscl_t bg2_rotscl_dy;
      reg_bg_rotscl_t bg2_rotscl_dmy;
      reg_bg_rotref_t bg2_rotref_x;
      reg_bg_rotref_t bg2_rotref_y;
      reg_bg_rotscl_t bg3_rotscl_dx;
      reg_bg_rotscl_t bg3_rotscl_dmx;
      reg_bg_rotscl_t bg3_rotscl_dy;
      reg_bg_rotscl_t bg3_rotscl_dmy;
      reg_bg_rotref_t bg3_rotref_x;
      reg_bg_rotref_t bg3_rotref_y;
      reg_win_coord_t win0_xdim;
      reg_win_coord_t win1_xdim;
      reg_win_coord_t win0_ydim;
      reg_win_coord_t win1_ydim;
      reg_win_ctrl_t win_in0_ctrl;
      reg_win_ctrl_t win_in1_ctrl;
      reg_win_ctrl_t win_out_ctrl;
      reg_win_ctrl_t win_obj_ctrl;
      reg_mosaic_dim_t mosaic_dim;
      uint16_t unused02;
      reg_coleff_ctrl_t coleff_ctrl;
      reg_ablend_coeff_t ablend_coeff;
      reg_bright_coeff_t bright_coeff;
  };
  static_assert(
    std::is_standard_layout_v<disp_control>
  );
  static_assert(
    sizeof(disp_control) == 0x58
  );
  /**
   * sound cotrol access container
   */
  packed(class) align(4) snd_control : public io_reg {
    public:
      reg_snd_sweep_t snd_ch1_sweep;
      reg_snd_cnt_t snd_ch1_cnt;
      reg_snd_freq_t snd_ch1_freq;
      uint16_t unused_00;
      reg_snd_cnt_t snd_ch2_cnt;
      uint16_t unused_01;
      reg_snd_freq_t snd_ch2_freq;
      uint16_t unused_02;
      reg_snd_sel_t snd_ch3_sel;
      reg_snd_vol_t snd_ch3_vol;
      reg_snd_freq_t snd_ch3_freq;
      uint16_t unused_03;
      reg_snd_cnt_t snd_ch4_cnt;
      uint16_t unused_04;
      reg_snd_noise_t snd_ch4_noise;
      uint16_t unused_05;
      reg_snd_dmg_cnt_t snd_dmg_cnt;
      reg_snd_ds_cnt_t snd_ds_cnt;
      reg_snd_stat_t snd_stat;
      uint16_t unused_06;
      reg_snd_bias_t snd_bias;
      uint16_t unused_07;
      //TODO: Wave pattern registers/ram
      //TODO: snd dma channels
  };
  static_assert(
    std::is_standard_layout_v<snd_control>
  );
  static_assert(
    sizeof(snd_control) == 0x2c
  );
  // TODO: DMA control
  // TODO: Timer control
  // TODO: Serial control
  // TODO: Keyboard input/control functions?
  
  /**
   * key input control container
   */
  packed(class) align(2) key_control : public io_reg {
    public:
      reg_key_inp_t key_inp;
      reg_key_ctrl_t key_ctrl;
  };
  static_assert(
    std::is_standard_layout_v<key_control>
  );
  static_assert(
    sizeof(key_control) == 0x4
  );
  /**
   * interrupt controll container
   */
  packed(class) align(2) irq_control : public io_reg {
    public:
      reg_irq_en_t irq_en;
      reg_irq_flags_t irq_flags;
      reg_wait_ctrl_t wait_ctrl;
      uint16_t unused00;
      reg_irq_men_t irq_men;
      uint16_t unused01;
  };
  static_assert(
    std::is_standard_layout_v<irq_control>
  );
  static_assert(
    sizeof(irq_control) == 0xC
  );
  /**
   * system power control container
   */
  packed(class) pwr_control : public io_reg {
    public:
      reg_post_flag_t post_flag;
      reg_halt_ctrl_t halt_ctrl;
  };
  static_assert(
    std::is_standard_layout_v<pwr_control>
  );
  static_assert(
    sizeof(pwr_control) == 0x2
  );
  // TODO: misc control
};
