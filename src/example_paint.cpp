/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

// Simple LCD mode3 bitmap pixel example

#include "gba_mem_sym.hpp"
#include "gba_helper.hpp"
#include "gba_bios.hpp"
#include "gba_reg_key.hpp"

//TODO: move to library
void irq_handler() {
  using namespace gba::dtypes;
  using namespace gba::hw;
  irq_flags_t irq_flags = {
    .lcd_vbl = 1,
  };

  reg_irq.irq_flags = irq_flags;
  bios_irqflags = bios_irqflags | irq_flags;
}

//TODO: move to library
void irq_init() {
  using namespace gba::dtypes;
  irq_vector = irq_handler;
  reg_irq.irq_en = (irq_en_t){
    .lcd_vbl = 1,
  };
  reg_disp.lcd_status = (lcd_status_t){
    .vblank_irq_en = 1,
  };
  reg_irq.irq_men = (irq_men_t){
    .enable = 1,
  };
}

void clear_pix(int x, int y) {
  if (mem_vram.mode3.pix[y][x].blue == 31) {
    if (mem_vram.mode3.pix[y][x].red == 31) {
      mem_vram.mode3.pix[y][x] = {.red = 31, .blue = 0};
    } else {
      mem_vram.mode3.pix[y][x] = {.blue = 0};
    }
  }
}

int main()
{
  using namespace gba::dtypes;
  using namespace gba::hw;
  using namespace gba::bios::call;
  irq_init();
  reg_disp.lcd_conf = (lcd_conf_t){
    .mode = lcd_mode_t::mode3,
    .bg2 = true,
  };
  int x = 120;
  int y = 80;
  int frame = 0;
  key_inp_t key_now;
  while(true) {
    int x_new = x;
    int y_new = y;
    if (frame%4 == 0) {
      bool moved = false;
      bool paint = false;
      key_now = reg_key.key_inp;
      if (key_now.btn_l == key_state_t::down && x_new > 0) {x_new--;moved=true;}
      if (key_now.btn_r == key_state_t::down && x_new < 239) {x_new++;moved=true;}
      if (key_now.btn_u == key_state_t::down && y_new > 0) {y_new--;moved=true;}
      if (key_now.btn_d == key_state_t::down && y_new < 159) {y_new++;moved=true;}
      if (key_now.btn_a == key_state_t::down) {
        paint = true;
        mem_vram.mode3.pix[y][x] = {.red = 31, .blue = 31};
      }
      if (moved) {
        clear_pix(x, y);
        x = x_new;
        y = y_new;
      }
      if (!paint) {
        if (mem_vram.mode3.pix[y][x].red != 31 || key_now.btn_b == key_state_t::down) {
          mem_vram.mode3.pix[y][x] = {.blue = 31};
        } else {
          mem_vram.mode3.pix[y][x] = {.red = 31, .blue = 31};
        }
      }
    }
    wait_vblank();
    frame++;
  }
  loop();
}
