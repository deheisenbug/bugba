/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

// Simple example game project
// Todo: cleanup this hacky code. a lot...

#include <cstdint>
#include <cstring>

#include "gba_reg_lcd.hpp"
#include "gba_reg_snd.hpp"
#include "gba_types.hpp"
#include "gba_macros.hpp"
#include "gba_reg_common.hpp"
#include "gba_reg_all.hpp"
#include "gba_mem_regs.hpp"
#include "gba_mem_sym.hpp"
#include "gba_swi.hpp"
#include "gba_bios.hpp"

volatile reg32_t frame = 0;
volatile reg32_t tile_pos = 128;
volatile reg32_t cur_anim = 0;
volatile reg32_t tile_lift = 0;
volatile reg32_t fade = 0;
volatile reg32_t blend = 0;
volatile reg32_t not_random = 0;
const uint32_t fade_pattern[20] = {0,11, 1,19, 6, 2, 4,14,18, 5,10, 8, 9,15, 3,12,17, 7,13,16};
gba::dtypes::ablend_coeff_t glob_ablend = {
  .val_1st = 31,
  .val_2nd = 0,
};

gba::dtypes::reg_key_inp_t key_now;

namespace gba::objects {
  using namespace gba::dtypes;
  using namespace gba::hw;
    enum class game_state_t {
      reset,
      logo,
      logo_trans_0,
      logo_trans_1,
      intro,
      running,
      score,
      win,
    };
    class game_entity_t {
      public:
      bool running;
      uint32_t speed;
      attr0_normal_t attr0;
      attr1_normal_t attr1;
      obj_attr2_t attr2;
      uint32_t oam_index;
      void apply() {
        mem_oam.obj[oam_index].attr0.attr.normal = attr0;;
        mem_oam.obj[oam_index].attr1.attr.normal = attr1;;
        mem_oam.obj[oam_index].attr2 = attr2;;
      }
    };
}

volatile gba::objects::game_state_t game_state = gba::objects::game_state_t::reset;

gba::objects::game_entity_t entity_penguin;
gba::objects::game_entity_t entity_apple;
gba::objects::game_entity_t entity_windows;
gba::objects::game_entity_t entity_android;
gba::objects::game_entity_t entity_chrome;

gba::objects::game_entity_t *entities[5] = {
  &entity_penguin,
  &entity_apple,
  &entity_windows,
  &entity_android,
  &entity_chrome,
};

gba::objects::game_entity_t *enemies[4] = {
  &entity_apple,
  &entity_windows,
  &entity_android,
  &entity_chrome,
};

void loop() {
  asm("1: b 1b");
}

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

void lcd_init() {
  using namespace gba::dtypes;
  using namespace gba::hw;
  reg_disp.lcd_conf = (lcd_conf_t){
    .mode = lcd_mode_t::mode1,
    .oam_map = oam_map_t::one_dim,
    .bg0 = 1,
    .bg1 = 1,
    .obj = 1,
  };
  reg_disp.ablend_coeff = glob_ablend;
  reg_disp.coleff_ctrl = (coleff_ctrl_t) {
    .bg0_1st = 1,
    .effect = coleff_t::alphablend,
    .bg1_2nd = 1,
  };
  reg_disp.bg0_conf = (bg_conf_t){
    .priority = 0,
    .char_base_block = 0,
    .pal_mode = pal_mode_t::p16x16,
    .screen_base_block = 5,
    .screen_size = bg_conf_t::size_normal::n256x256,
  };
  reg_disp.bg1_conf = (bg_conf_t){
    .char_base_block = 0,
    .pal_mode = pal_mode_t::p16x16,
    .screen_base_block = 4,
    .screen_size = bg_conf_t::size_normal::n256x256,
  };
  //TODO: create library/function for dynamic tile loading
  std::memcpy(&mem_pal.bg.p16x16.col[0][0],(void*)tile_bg_icePal,16*sizeof(rgb16_t));
  std::memcpy(&mem_pal.bg.p16x16.col[1][0],(void*)tile_title_defaultPal,16*sizeof(rgb16_t));
  std::memcpy(&mem_pal.bg.p16x16.col[2][0],(void*)tile_title_retryPal,2*sizeof(rgb16_t));
  std::memcpy(&mem_pal.bg.p16x16.col[3][0],(void*)tile_title_winPal,16*sizeof(rgb16_t));
  std::memcpy(&mem_pal.obj.p16x16.col[0][0],(void*)penguin_spriteByElthenPal,16*sizeof(rgb16_t));
  std::memcpy(&mem_pal.obj.p16x16.col[1][0],(void*)apple_spritePal,16*sizeof(rgb16_t));
  std::memcpy(&mem_pal.obj.p16x16.col[2][0],(void*)windows_spritePal,16*sizeof(rgb16_t));
  std::memcpy(&mem_pal.obj.p16x16.col[3][0],(void*)android_spritePal,16*sizeof(rgb16_t));
  std::memcpy(&mem_pal.obj.p16x16.col[4][0],(void*)chrome_spritePal,16*sizeof(rgb16_t));
  std::memcpy(&mem_vram.mode1.char_block.bg[0].chars.as_4bpp.tile[0],(void*)tile_bg_iceTiles,8*sizeof(char_4bpp_tile_t));
  std::memcpy(&mem_vram.mode1.char_block.bg[0].chars.as_4bpp.tile[8],(void*)tile_title_defaultTiles,20*12*sizeof(char_4bpp_tile_t));
  std::memcpy(&mem_vram.mode1.char_block.obj[0].chars.as_4bpp.tile[0],(void*)penguin_spriteByElthenTiles,24*sizeof(char_4bpp_tile_t));
  std::memcpy(&mem_vram.mode1.char_block.obj[1].chars.as_4bpp.tile[4],(void*)apple_spriteTiles,4*sizeof(char_4bpp_tile_t));
  std::memcpy(&mem_vram.mode1.char_block.obj[1].chars.as_4bpp.tile[8],(void*)windows_spriteTiles,4*sizeof(char_4bpp_tile_t));
  std::memcpy(&mem_vram.mode1.char_block.obj[1].chars.as_4bpp.tile[12],(void*)android_spriteTiles,4*sizeof(char_4bpp_tile_t));
  std::memcpy(&mem_vram.mode1.char_block.obj[1].chars.as_4bpp.tile[16],(void*)chrome_spriteTiles,4*sizeof(char_4bpp_tile_t));
}

void lcd_setup() {
  using namespace gba::dtypes;
  using namespace gba::hw;
  using namespace gba::objects;
  for(int i = 0; i<18; i++) {
    for(int j = 0; j<30; j++) {
      bg_block_normal_t bg_block;
      bg_block.tile_nr = 4;
      //TODO: create library/function for dynamic tile loading
      mem_vram.mode1.screen_block.se[4].map.normal[i][j] = bg_block;
    }
  }
  for(int i = 18; i<20; i++) {
    for(int j = 0; j<30; j++) {
      bg_block_normal_t bg_block;
      bg_block.tile_nr = (i % 2) * 2 + (j % 2);
      mem_vram.mode1.screen_block.se[4].map.normal[i][j] = bg_block;
    }
  }
  for(int i = 0; i<20; i++) {
    for(int j = 0; j<30; j++) {
      bg_block_normal_t bg_block;
      bg_block.tile_nr = 8;
      bg_block.pal_nr  = 1;
      mem_vram.mode1.screen_block.se[5].map.normal[i][j] = bg_block;
    }
  }
  for(int i = 0; i<11; i++) {
    for(int j = 0; j<20; j++) {
      bg_block_normal_t bg_block;
      bg_block.tile_nr = 8 + i*20 + j;
      bg_block.pal_nr = 1;
      mem_vram.mode1.screen_block.se[5].map.normal[3+i][5+j] = bg_block;
    }
  }
  attr0_normal_t obj_disabled;
  obj_disabled.disable = 1;

  for(int i = 0; i<128 ;i++) {
    mem_oam.obj[i].attr0.attr.normal = obj_disabled;
  }
}

void snd_init() {
  using namespace gba::dtypes;
  using namespace gba::hw;
  using namespace gba::objects;
  reg_snd.snd_stat = (snd_stat_t){
    .snd_en = true,
  };
  reg_snd.snd_dmg_cnt = (snd_dmg_cnt_t) {
    .vol_r = 7,
    .vol_l = 7,
    .ch1_r_en = true,
    .ch4_r_en = true,
    .ch1_l_en = true,
    .ch4_l_en = true,
  };
  reg_snd.snd_ds_cnt = (snd_ds_cnt_t) {
    .dmglvl = snd_dmglvl_t::pct100,
  };
}

void snd_setup() {
  using namespace gba::dtypes;
  using namespace gba::hw;
  using namespace gba::objects;
  reg_snd.snd_ch1_sweep = (snd_sweep_t) {
    .shift = 1,
    .dir = snd_sweep_dir_t::inc,
    .time = 5,
  };
  reg_snd.snd_ch1_cnt = (snd_cnt_t) {
    .duty = snd_duty_t::pct50,
    .env_vol = 15,
  };
  reg_snd.snd_ch4_cnt = (snd_cnt_t) {
    .len = 1,
    .env_step = 3,
    .env_vol = 3,
  };
}

void entity_setup() {
  using namespace gba::dtypes;
  using namespace gba::hw;
  using namespace gba::objects;
  for(uint32_t i = 0; i < sizeof(entities)/sizeof(entities[0]);i++) {
    entities[i]->running = false;
    entities[i]->speed = 1;
    entities[i]->attr0.offs_y = 128;
    entities[i]->attr1.obj_size = obj_size_square::s16x16;
    entities[i]->attr1.offs_x = 240;
    if(i>0) {
      entities[i]->attr2.pal_nr = i;
      entities[i]->attr2.tile_nr = 512 + i*4;
    }
    entities[i]->oam_index = i;
    entities[i]->apply();
  }
}

void enemies_reset() {
  using namespace gba::dtypes;
  using namespace gba::hw;
  using namespace gba::objects;
  for(uint32_t i = 0; i < sizeof(enemies)/sizeof(enemies[0]);i++) {
    enemies[i]->running = false;
    enemies[i]->speed = 1;
    enemies[i]->attr0.offs_y = 128;
    enemies[i]->attr1.offs_x = 240;
    enemies[i]->apply();
  }
}

int main()
{
  using namespace gba::dtypes;
  using namespace gba::hw;
  using namespace gba::objects;
  using namespace gba::bios::call;
  lcd_init();
  lcd_setup();
  snd_init();
  snd_setup();
  entity_setup();
  irq_init();
  while(1) {
    static uint32_t logo_frame_0 = 0;
    static uint32_t logo_frame_1 = 0;
    static uint32_t intro_frame = 0;
    static uint32_t running_frame = 0;
    static uint32_t downforce = 0;
    static uint32_t lift = 0;
    static uint32_t skip_spawn = 0;
    static uint32_t enemy_speed = 1;
    static uint32_t was_finished = 0;
    static uint32_t initial_run = true;
    wait_vblank();
    frame = frame + 1;
    key_now = reg_key.key_inp;
    switch (game_state) {
      case game_state_t::reset:
          logo_frame_0 = 0;
          logo_frame_1 = 0;
          intro_frame = 0;
          running_frame = 0;
          downforce = 0;
          lift = 0;
          skip_spawn = 0;
          enemy_speed = 1;
          if (initial_run) {
            entity_setup();
            entity_penguin.attr0.offs_y = 128;
            entity_penguin.attr1.offs_x = 112;
            entity_penguin.apply();
          }
          enemies_reset();
          if (was_finished) {
            snd_setup();
            was_finished = 0;
          }
          game_state = game_state_t::logo;
        break;
      case game_state_t::logo:
        if (key_now.btn_sta == key_state_t::down) {
          game_state = game_state_t::logo_trans_0;
        };
        break;
      case game_state_t::logo_trans_0:
        if (initial_run) {
          if ((frame % 2) == 0 ) {
            if(logo_frame_0 < 20) {
              bg_block_normal_t bg_block;
              bg_block.tile_nr = 5;
              bg_block.pal_nr  = 0;
              for(int i = 0;i<30;i++) {
                mem_vram.mode1.screen_block.se[5].map.normal[fade_pattern[logo_frame_0]][i] = bg_block;
              }
              logo_frame_0 = logo_frame_0 + 1;
            } else {
              game_state = game_state_t::logo_trans_1;
            }
          }
        } else {
            game_state = game_state_t::logo_trans_1;
        }
        break;
      case game_state_t::logo_trans_1:
        if ((frame % 2) ==0) {
          if (logo_frame_1 < 32) {
            glob_ablend.val_1st = 31-logo_frame_1;
            glob_ablend.val_2nd = logo_frame_1;
            reg_disp.ablend_coeff = glob_ablend;
            logo_frame_1 = logo_frame_1 +1;
          } else {
            game_state = game_state_t::intro;
          }
        }
        downforce = 0;
        break;
      case game_state_t::intro:
          if (entity_penguin.attr1.offs_x > 16) {
            entity_penguin.attr1.offs_x -=1;
            entity_penguin.attr1.flip_x = 1;
            entity_penguin.attr2.tile_nr = ((intro_frame/2) % 6) * 4;
            entity_penguin.attr0.offs_y += (downforce - lift);
            if (entity_penguin.attr0.offs_y < 128) {
              if((running_frame % 3) == 0) {
                downforce +=1;
              }
            } else {
              downforce =0;
              lift = 0;
            }
            if (entity_penguin.attr0.offs_y > 128) {
              entity_penguin.attr0.offs_y = 128;
            }
            entity_penguin.apply();
          } else {
            entity_penguin.attr1.flip_x = 0;
            entity_penguin.apply();
            game_state = game_state_t::running;
          }
          intro_frame = intro_frame + 1;
        break;
      case game_state_t::running:
        if (key_now.btn_a == key_state_t::down || key_now.btn_r == key_state_t::down || key_now.btn_l == key_state_t::down ) {
          entity_penguin.attr2.tile_nr  = 4 * ((running_frame/2) % 6);
          if(running_frame % 8 == 0 && entity_penguin.attr0.offs_y == 128) {
            reg_snd.snd_ch4_noise = (snd_noise_t) {
              .ratio = 0,
              .shift = 2,
              .timed = true,
              .reset = true,
            };
          }
          not_random = not_random + frame;
        }
        if (key_now.btn_r == key_state_t::down && entity_penguin.attr1.offs_x < 224 ) {
          entity_penguin.attr1.offs_x = entity_penguin.attr1.offs_x +1;
          entity_penguin.attr1.flip_x = 0;
        } else if (key_now.btn_l == key_state_t::down && entity_penguin.attr1.offs_x > 0) {
          entity_penguin.attr1.offs_x = entity_penguin.attr1.offs_x -1;
          entity_penguin.attr1.flip_x = 1;
        }
        if (key_now.btn_a == key_state_t::down && entity_penguin.attr0.offs_y == 128) {
          using namespace gba::hw;
          lift = 6;
          reg_snd.snd_ch1_freq = (snd_freq_t) {
            .rate = 700,
            .reset = true,
          };
        }
        entity_penguin.attr0.offs_y += (downforce - lift);
        if (entity_penguin.attr0.offs_y < 128) {
          if((running_frame % 3) == 0) {
            downforce +=1;
          }
        } else {
          downforce =0;
          lift = 0;
        }
        if (entity_penguin.attr0.offs_y > 128) {
          entity_penguin.attr0.offs_y = 128;
        }
        if ( ( ( not_random % 300 ) < (16 + (enemy_speed * 16 )) ) && skip_spawn == 0) {
          if ( ! enemies[not_random % 4]->running ) {
            enemies[not_random % 4]->running = true;
            enemies[not_random % 4]->speed = enemy_speed;
            skip_spawn = (64 - enemy_speed * 6);
          }
        }
        if ( skip_spawn > 0 ) {
           skip_spawn -= 1;
        }
        for(game_entity_t *en : enemies ){
          if ( en->running ) {
            if ( en->attr1.offs_x < 1 || en->attr1.offs_x > 240 ) {
                en->attr1.offs_x = 240;
                en->running = false;
            } else {
              en->attr1.offs_x = en->attr1.offs_x - en->speed;
            }
          }
          en->apply();
        }
        entity_penguin.apply();
        if ( ( (frame % 900) == 0 ) && enemy_speed < 6) {
          enemy_speed +=1;
        }
        running_frame +=1;
        not_random = not_random + 1337 ;
        for (game_entity_t *en :enemies) {
          if (entity_penguin.attr1.offs_x + 12 > en->attr1.offs_x && entity_penguin.attr1.offs_x - 12 < en->attr1.offs_x && entity_penguin.attr0.offs_y + 14 > en->attr0.offs_y)
          game_state = game_state_t::score;
        }
        if ( enemy_speed == 5 ) {
          game_state = game_state_t::win;
        }
        if (key_now.btn_sta == key_state_t::down && key_now.btn_sel == key_state_t::down) {
          game_state = game_state_t::reset;
        }
        break;
      case game_state_t::score:
        if (initial_run) {
          std::memcpy(&mem_vram.mode1.char_block.bg[0].chars.as_4bpp.tile[8],(void*)tile_title_retryTiles,20*12*sizeof(char_4bpp_tile_t));
          initial_run = false;
        }
          if(!was_finished) {
            was_finished = 1;
            reg_snd.snd_ch4_cnt = (snd_cnt_t) {
              .len = 1,
              .env_step = 6,
              .env_vol = 10,
            };
            reg_snd.snd_ch4_noise = (snd_noise_t) {
              .ratio = 3,
              .shift = 5,
              .timed = true,
              .reset = true,
            };
            for(int i = 0; i<20; i++) {
              for(int j = 0; j<30; j++) {
                bg_block_normal_t bg_block;
                bg_block.tile_nr = 8;
                bg_block.pal_nr  = 1;
                mem_vram.mode1.screen_block.se[5].map.normal[i][j] = bg_block;
              }
            }
            for(int i = 0; i<11; i++) {
              for(int j = 0; j<20; j++) {
                bg_block_normal_t bg_block;
                bg_block.tile_nr = 8 + i*20 + j;
                bg_block.pal_nr = 2;
                mem_vram.mode1.screen_block.se[5].map.normal[3+i][5+j] = bg_block;
              }
            }
            glob_ablend.val_1st = 31;
            glob_ablend.val_2nd = 0;
            reg_disp.ablend_coeff = glob_ablend;
          }
        if (key_now.btn_sta == key_state_t::down && key_now.btn_sel == key_state_t::down) {
          game_state = game_state_t::reset;
        }
        break;
      case game_state_t::win:
        if(!was_finished) {
          was_finished = 1;
          enemies_reset();
          std::memcpy(&mem_vram.mode1.char_block.bg[0].chars.as_4bpp.tile[8],(void*)tile_title_winTiles,20*12*sizeof(char_4bpp_tile_t));
          for(int i = 0; i<20; i++) {
            for(int j = 0; j<30; j++) {
              bg_block_normal_t bg_block;
              bg_block.tile_nr = 8;
              bg_block.pal_nr  = 1;
              mem_vram.mode1.screen_block.se[5].map.normal[i][j] = bg_block;
            }
          }
          for(int i = 0; i<11; i++) {
            for(int j = 0; j<20; j++) {
              bg_block_normal_t bg_block;
              bg_block.tile_nr = 8 + i*20 + j;
              bg_block.pal_nr = 3;
              mem_vram.mode1.screen_block.se[5].map.normal[3+i][5+j] = bg_block;
            }
          }
          glob_ablend.val_1st = 31;
          glob_ablend.val_2nd = 0;
          reg_disp.ablend_coeff = glob_ablend;
          reg_snd.snd_ch1_sweep = (snd_sweep_t) {
            .shift = 1,
            .dir = snd_sweep_dir_t::inc,
            .time = 7,
          };
          reg_snd.snd_ch1_freq = (snd_freq_t) {
            .rate = 100,
            .reset = true,
          };
        }
        if (key_now.btn_sta == key_state_t::down && key_now.btn_sel == key_state_t::down) {
          game_state = game_state_t::reset;
        }
        break;
    }
  };
  loop();
  return 0;
}
