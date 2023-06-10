/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * LCD registers
 */

#include <cstdint>

#include "gba_types.hpp"
#include "gba_reg_common.hpp"

#pragma once

namespace gba::dtypes {
  /**
   * GBA RGB data type
   */
  class rgb16_t {
    public:
        uint16_t red : 5 = 0;
        uint16_t green : 5 = 0;
        uint16_t blue : 5 = 0;
        uint16_t alpha : 1 = 0;
  };
  using reg_rgb16_t = reg_t<rgb16_t,reg16_t>;
  /**
   * Mode 4 subpixel type
   */
  class m4_sub_t {
    public:
      uint8_t low;
      uint8_t high;
      m4_sub_t(uint8_t low_val,uint8_t high_val) : low(low_val),high(high_val) {};
  };
  using reg_m4_sub_t = reg_t<m4_sub_t,reg16_t>;
  /**
   * GBA LCD modes
   */
  enum class lcd_mode_t : uint16_t {
    mode0 = 0,
    mode1 = 1,
    mode2 = 2,
    mode3 = 3,
    mode4 = 4,
    mode5 = 5,
  };
  /**
   * GBA OAM mapping mode can be two or one dimensional.
   */
  enum class oam_map_t : uint16_t {
    two_dim = 0,
    one_dim = 1
  };
  /**
   * Colours can be acccessed as one 256 entry palette or 16 palettes with 16 entrys.
   */
  enum class pal_mode_t : uint16_t {
    p16x16 = 0,
    p256x1 = 1
  };
  /**
   * objects transfomration type
   */
  enum class obj_mode_t : uint16_t {
    normal = 0,
    semitrans = 1,
    objwinmask = 2,
    prohibited = 3,
  };
  /**
   * object shapy type
   */
  enum class obj_shape_t : uint16_t {
    square = 0,
    wide = 1,
    thin = 2,
    prohibited = 3,
  };
  /**
   * normal attribute 0 regsiter
   */
  class attr0_normal_t {
  public:
    uint16_t offs_y : 8 = 0;
    uint16_t rotscl_en : 1 = 0;
    uint16_t disable : 1 = 0;
    obj_mode_t mode : 2 = obj_mode_t::normal;
    uint16_t mosaic_en : 1 = 0;
    pal_mode_t pal_mode : 1 = pal_mode_t::p16x16;
    obj_shape_t obj_shape : 2 = obj_shape_t::square;
  };
  using reg_attr0_normal_t = reg_t<attr0_normal_t,reg16_t>;
  /**
   * rotscl attribute 0 register
   */
  class attr0_rotscl_t {
  public:
    uint16_t offs_y : 8 = 0;
    uint16_t rotscl_en : 1 = 0;
    uint16_t size_double : 1 = 0;
    obj_mode_t mode : 2 = obj_mode_t::normal;
    uint16_t mosaic_en : 1 = 0;
    pal_mode_t pal_mode : 1 = pal_mode_t::p16x16;
    obj_shape_t obj_shape : 2 = obj_shape_t::square;
  };
  using reg_attr0_rotscl_t = reg_t<attr0_rotscl_t,reg16_t>;
  /**
   * OAM attribute 0 register type
   * The attribute is accessed different for normal and rotscl objects
   */
  class obj_attr0_t {
    union attr0_type_t {
      reg_attr0_normal_t normal;
      reg_attr0_rotscl_t rotscl;
    };
    public:
      attr0_type_t attr;
  };
  using reg_obj_attr0_t = reg_t<obj_attr0_t,reg16_t>;
  /**
   * attribute 1 square dimensions
   */
  enum obj_size_square : uint16_t {
    s8x8 = 0,
    s16x16 = 1,
    s32x32 = 2,
    s64x64 = 3,
  };
  /**
   * attribute 1 tall dimensions
   */
  enum obj_size_tall : uint16_t {
    s16x8 = 0,
    s32x8 = 1,
    s32x16 = 2,
    s64x32 = 3,
  };
  /**
   * attribute 1 thin dimensions
   */
  //TODO: fix tall/thin confusion???
  enum obj_size_thin : uint16_t {
    s8x16 = 0,
    s8x32 = 1,
    s16x32 = 2,
    s32x64 = 3,
  }; // TODO: check if can achive with enum class and union?
  
  class attr1_normal_t {
  public:
    uint16_t offs_x : 9 = 0;
    uint16_t : 3;
    uint16_t flip_x : 1 = 0;
    uint16_t flip_y : 1 = 0;
    uint16_t obj_size : 2 = obj_size_square::s8x8;
  };
  using reg_attr1_normal_t = reg_t<attr1_normal_t,reg16_t>;
  class attr1_rotscl_t {
  public:
    uint16_t offs_x : 9 = 0;
    uint16_t rotscl_sel : 5 = 0;
    uint16_t obj_size : 2 = obj_size_square::s8x8;
  };
  using reg_attr1_rotscl_t = reg_t<attr1_rotscl_t,reg16_t>;
  /**
   * OAM attribute 1 type
   * The attribute is accessed differently for normal and rotscl objects
   */
  class obj_attr1_t {
    union attr1_type_t {
      reg_attr1_normal_t normal;
      reg_attr1_rotscl_t rotscl;
    };
    public:
      attr1_type_t attr;
  };
  using reg_obj_attr1_t = reg_t<obj_attr1_t,reg16_t>;
  /**
   * OAM attribute 2 register
   */
  class obj_attr2_t {
    public:
      reg16_t tile_nr : 10 = 0;
      reg16_t prio : 2 = 0;
      reg16_t pal_nr : 4 = 0;
  };
  using reg_obj_attr2_t = reg_t<obj_attr2_t,reg16_t>;
  /**
   * OAM attribute and rotscl container class
   */
  packed(class) align(2) oam_obj_t {
    public:
      reg_obj_attr0_t attr0;
      reg_obj_attr1_t attr1;
      reg_obj_attr2_t attr2;
      reg16_t rotscl;
  };
  static_assert(
    sizeof(oam_obj_t) == 4 * sizeof(uint16_t)
  );
  /**
   * 4 bits per pixel 2 byte register container
   */
  class char_4x4bpp_t {
    public:
      uint16_t p1 : 4 = 0;
      uint16_t p2 : 4 = 0;
      uint16_t p3 : 4 = 0;
      uint16_t p4 : 4 = 0;
  };
  using reg_char_4x4bpp_t = reg_t<char_4x4bpp_t,reg16_t>;
  /**
   * 8 bits per pixel 2 byte register container
   */
  class char_2x8bpp_t {
    public:
      uint16_t p1 : 8 = 0;
      uint16_t p2 : 8 = 0;
  };
  using reg_char_2x8bpp_t = reg_t<char_2x8bpp_t,reg16_t>;
  /**
   * 4 bits per pixel full tile struct
   */
  packed(struct) char_4bpp_tile_t {
    reg_char_4x4bpp_t part[16];
  };
  static_assert(
    sizeof(char_4bpp_tile_t) == 32
  );
  /**
   * 8 bits per pixel full tile struct
   */
  packed(struct) char_8bpp_tile_t {
    reg_char_2x8bpp_t part[32];
  };
  static_assert(
    sizeof(char_8bpp_tile_t) == 64
  );
  /**
   * Whole char block container class
   */
  packed(class) bg_char_block_t {
    packed(struct) char_4bpp_block_t {
      char_4bpp_tile_t tile[512];
    };
    static_assert(
      sizeof(char_4bpp_block_t) == 16384
    );
    packed(struct) char_8bpp_block_t {
       char_8bpp_tile_t tile[256];
    };
    static_assert(
      sizeof(char_8bpp_block_t) == 16384
    );
    union chars_t {
      char_4bpp_block_t as_4bpp;
      char_8bpp_block_t as_8bpp;
    } packed();
    static_assert(
      sizeof(chars_t) == 16384
    );
    public:
      chars_t chars;
  };
  static_assert(
    sizeof(bg_char_block_t) == 16384
  );
  /**
   * Normal screen block register
   */
  class bg_block_normal_t {
    public:
      uint16_t tile_nr : 10 = 0;
      uint16_t flip_x : 1 = 0;
      uint16_t flix_y : 1 = 0;
      uint16_t pal_nr : 4 = 0;
  };
  using reg_bg_block_normal_t = reg_t<bg_block_normal_t,reg16_t>;
  /**
   * Rotscl screen block register
   */
  class bg_block_rotscl_t {
    public:
      uint8_t tile_nr = 0;
  };
  using reg_bg_block_rotscl_t = reg_t<bg_block_rotscl_t,reg8_t>;
  /**
   * screen block container type
   */
  packed(class) bg_screen_block_t {
    union block_t {
      reg_bg_block_normal_t normal[32][32];
      reg_bg_block_rotscl_t rotscl[2048];
    } packed();
    public:
      block_t map;
  };
  static_assert(
    sizeof(bg_screen_block_t) == 2048
  );
  /**
   * LCD configuration register
   */
  class lcd_conf_t {
    public:
      lcd_mode_t mode : 3 = lcd_mode_t::mode0;
      uint16_t gbc_mode : 1 = 0;
      uint16_t frame_select : 1 = 0;
      uint16_t allow_oam_hblank : 1 = 0;
      oam_map_t oam_map : 1 = oam_map_t::two_dim;
      uint16_t force_blank : 1 = 0;
      uint16_t bg0 : 1 = 0;
      uint16_t bg1 : 1 = 0;
      uint16_t bg2 : 1 = 0;
      uint16_t bg3 : 1 = 0;
      uint16_t obj : 1 = 0;
      uint16_t win0_display : 1 = 0;
      uint16_t win1_display : 1 = 0;
      uint16_t obj_win_display : 1 = 0;
  };
  using reg_lcd_conf_t = reg_t<lcd_conf_t,reg16_t>;
  /**
   * LCD status register
   */
  class lcd_status_t {
  public:
    uint16_t vblank_flag : 1 = 0;
    uint16_t hblank_flag : 1 = 0;
    uint16_t vcount_flag : 1 = 0;
    uint16_t vblank_irq_en : 1 = 0;
    uint16_t hblank_irq_en : 1 = 0;
    uint16_t vcount_irq_en : 1 = 0;
    uint16_t : 2;
    uint16_t vcount_set : 8 = 0;
  };
  using reg_lcd_status_t = reg_t<lcd_status_t,reg16_t>;
  /**
   * Background general configuration register
   */
  class bg_conf_t {
    public:
      enum size_normal : uint16_t {
        n256x256 = 0,
        n512x256 = 1,
        n256x512 = 2,
        n512x512 = 3,
      };
      enum size_rotscl : uint16_t {
        rs128x128 = 0,
        rs256x256 = 1,
        rs512x512 = 2,
        rs1024x1024 = 3,
      };
      uint16_t priority : 2 = 0;
      uint16_t char_base_block : 2 = 0;
      uint16_t : 2;
      uint16_t mosaic : 1 = 0;
      pal_mode_t pal_mode : 1 = pal_mode_t::p16x16;
      uint16_t screen_base_block : 5 = 0;
      uint16_t disp_area_overflow : 1 = 0; //#TODO: make enum?
      uint16_t screen_size : 2 = size_normal::n256x256;
  };
  using reg_bg_conf_t = reg_t<bg_conf_t,reg16_t>;
  /**
   * Background  offset register
   */
  class bg_offs_t {
    public:
      uint16_t offs : 9 = 0;
      uint16_t : 7;
  };
  using reg_bg_offs_t = reg_t<bg_offs_t,reg16_t>;
  /**
   * Background scroll register
   */
  class bg_scroll_t {
    public:
      bg_offs_t x;
      bg_offs_t y;
  };
  using reg_bg_scroll_t = reg_t<bg_scroll_t,reg32_t>;
  /**
   * Background rotation and scaling register
   */
  class bg_rotscl_t {
    public:
      uint16_t frac_part : 8 = 0;
      int16_t int_part : 8 = 0;
  };
  using reg_bg_rotscl_t = reg_t<bg_rotscl_t,reg16_t>;
  /**
   * Background rotation reference point
   */
  class bg_rotref_t {
    public:
      uint32_t frac_part : 8 = 0;
      int32_t int_part : 20 = 0;
      uint32_t : 4;
  };
  using reg_bg_rotref_t = reg_t<bg_rotref_t,reg32_t>;
  /**
   * window coordinates register
   */
  class win_coord_t {
    public:
      uint16_t max : 8 = 0;
      uint16_t min : 8 = 0;
  };
  using reg_win_coord_t = reg_t<win_coord_t,reg16_t>;
  /**
   * window control register
   */
  class win_ctrl_t {
    public:
      uint8_t bg0 : 1 = 0;
      uint8_t bg1 : 1 = 0;
      uint8_t bg2 : 1 = 0;
      uint8_t bg3 : 1 = 0;
      uint8_t obj : 1 = 0;
      uint8_t col : 1 = 0;
      uint8_t : 2;
  };
  using reg_win_ctrl_t = reg_t<win_ctrl_t,reg8_t>;
  /**
   * Mosaic dimension register
   */
  class mosaic_dim_t {
    public:
      uint16_t bg_x : 4 = 0;
      uint16_t bg_y : 4 = 0;
      uint16_t obj_x : 4 = 0;
      uint16_t obj_y : 4 = 0;
  };
  using reg_mosaic_dim_t = reg_t<mosaic_dim_t,reg16_t>;
  /**
   * Color effect type
   */
  enum class coleff_t : uint16_t {
    none = 0,
    alphablend = 1,
    bright_inc = 2,
    bright_dev = 2,
  };
  /**
   * color effects control register
   */
  class coleff_ctrl_t {
    /// Effect type
    public:
      uint16_t bg0_1st : 1 = 0;
      uint16_t bg1_1st : 1 = 0;
      uint16_t bg2_1st : 1 = 0;
      uint16_t bg3_1st : 1 = 0;
      uint16_t obj_1st : 1 = 0;
      uint16_t bdr_1st : 1 = 0;
      coleff_t effect : 2 = coleff_t::none;
      uint16_t bg0_2nd : 1 = 0;
      uint16_t bg1_2nd : 1 = 0;
      uint16_t bg2_2nd : 1 = 0;
      uint16_t bg3_2nd : 1 = 0;
      uint16_t obj_2nd : 1 = 0;
      uint16_t bdr_2nd : 1 = 0;
      uint16_t : 2;
  };
  using reg_coleff_ctrl_t = reg_t<coleff_ctrl_t,reg16_t>;
  /**
   * Alpha blending coefficient register
   */
  class ablend_coeff_t {
    public:
      uint16_t val_1st : 5 = 0;
      uint16_t : 3;
      uint16_t val_2nd : 5 = 0;
      uint16_t : 3;
  };
  using reg_ablend_coeff_t = reg_t<ablend_coeff_t,reg16_t>;
  /**
   * brightness coefficent register
   */
  class bright_coeff_t {
    public:
      uint32_t val : 5 = 0;
      uint32_t : 27;
  };
  using reg_bright_coeff_t = reg_t<bright_coeff_t,reg32_t>;
}
