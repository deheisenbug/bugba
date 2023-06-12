/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

// Simple LCD mode3 bitmap pixel example

#include "gba_mem_sym.hpp"
#include "gba_helper.hpp"

int main()
{
  using namespace gba::dtypes;
  using namespace gba::hw;

  reg_disp.lcd_conf = (lcd_conf_t){
    .mode = lcd_mode_t::mode3,
    .bg2 = true,
  };
  mem_vram.mode3.pix[72][112] = {.red = 31};
  mem_vram.mode3.pix[72][128] = {.green = 31};
  mem_vram.mode3.pix[88][112] = {.blue = 31};
  mem_vram.mode3.pix[88][128] = {
    .red = 31,
    .green = 31,
  };
  loop();
}
