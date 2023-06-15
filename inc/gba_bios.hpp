/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * Bios functions wrappers
 */

#include "gba_swi.hpp"
#include "gba_macros.hpp"
#include "gba_reg_irq.hpp"
#include "gba_types.hpp"
#include <cstdint>

#pragma once

namespace gba::bios {

namespace call {
    using namespace gba::dtypes;
    packed(class) reset_flags {
        public:
            uint32_t ewram : 1 = 0;
            uint32_t iwram : 1 = 0;
            uint32_t pal : 1 = 0;
            uint32_t vram : 1 = 0;
            uint32_t oam : 1 = 0;
            uint32_t regsio : 1 = 0;
            uint32_t regsnd : 1 = 0;
            uint32_t regall : 1 = 0;
            uint32_t : 24;
    };
    static_assert(
        sizeof(reset_flags) == sizeof(uint32_t)
    );
    enum class intr_discard : uint32_t {
        no = 0,
        yes = 1,
    };
    always_inline
    inline void reset_soft() {
        swi<0x00,void>();
        unreachable();
    }
    always_inline
    inline void reset_ram(reset_flags flags) {
        swi<0x01,void>(flags);
    }
    always_inline
    inline void reset_halt() {
        swi<0x02,void>();
    }
    always_inline
    inline void reset_stop() {
        swi<0x03,void>();
    }
    always_inline
    inline void wait_intr(intr_discard discard ,irq_flags_t flags) {
        swi<0x04,void>(discard, flags);
    }
    always_inline
    inline void wait_vblank() {
        swi<0x05, void>();
    }
    always_inline
    inline int32_t math_div(int32_t num, uint32_t div) {
        return swi<0x06, int32_t>(num,div);
    }
    always_inline
    inline int32_t math_divarm(int32_t num, uint32_t div) {
        return swi<0x07, int32_t>(div,num);
    }
    always_inline
    inline int32_t math_sqrt(int32_t num) {
        return swi<0x08, int32_t>(num);
    }
    //TODO: fix number format
    always_inline
    inline int32_t math_arctan(int16_t num) {
        return swi<0x09, int32_t>(num);
    }
    //TODO: fix number format
    always_inline
    inline int32_t math_arctan2(int16_t x,uint16_t y) {
        return swi<0x0A, int32_t>(x,y);
    }
    packed(class) cpuset_conf {
        public:
            enum class mode_t : uint32_t {
                copy = 0,
                fill = 1,
            };
            //Info: only used with cpuset
            enum class word_t : uint32_t {
                half = 0,
                full = 1,
            };
            uint32_t ewram : 21 = 0;
            uint32_t : 3;
            mode_t mode : 1 = mode_t::copy;
            uint32_t : 1;
            word_t word : 1 = word_t::half;
    };
    static_assert(
        sizeof(cpuset_conf) == sizeof(uint32_t)
    );
    always_inline
    inline void cpuset(size_t* src, size_t* dst, cpuset_conf conf) {
        swi<0x0B, void>(src,dst,conf);
    }
    always_inline
    inline void cpufastset(size_t* src, size_t* dst, cpuset_conf conf) {
        swi<0x0C, void>(src,dst,conf);
    }
    namespace secret {
        always_inline
        inline int32_t checksum() {
            return swi<0x0D, int32_t>();
        }
    }
    always_inline
    inline int16_t misc_crc16(size_t* addr, uint32_t len, int16_t init = 0xffff) {
        return swi<0x0E, int16_t>(init, addr, len);
    }
    //TODO: 0x0F misc_isdebugger
    //TODO: 0x10-0x18 decompress
    //TODO: 
    always_inline
    inline void snd_bias(uint32_t level, uint32_t delay) {
        swi<0x19, void>(level, delay);
    }
    //TODO: driver area structure?
    always_inline
    inline void snd_init(size_t* area) {
        swi<0x1A, void>(area);
    }
    packed(class) snd_mode_conf {
        public:
            enum class revb_state : uint32_t {
                ignore = 0,
                appply = 1,
            };
            //Info: only used with cpuset
            enum class freq : uint32_t {
                Hz5734 = 1,
                Hz7884 = 2,
                Hz10512 = 3,
                Hz13379 = 4,
                Hz15768 = 5,
                Hz18157 = 6,
                Hz21024 = 7,
                Hz26758 = 8,
                Hz31536 = 9,
                Hz36314 = 10,
                Hz40137 = 11,
                Hz42048 = 12,
            };
            enum class dac_bits : uint32_t {
                bits9 = 8,
                bits8 = 9,
                bits7 = 10,
                bits6 = 11,
            };
            uint32_t revb : 7 = 0;
            revb_state revb_state : 1 = revb_state::ignore;
            uint32_t simultan : 4 = 8;
            uint32_t vol_main : 4 = 15;
            freq freq : 4 = freq::Hz13379;
            dac_bits dac_bits : 4 = dac_bits::bits8;
            uint32_t : 8;
    };
    static_assert(
        sizeof(snd_mode_conf) == sizeof(uint32_t)
    );
    always_inline
    inline void snd_mode(snd_mode_conf conf) {
        swi<0x1B, void>(conf);
    }
    always_inline
    inline void snd_main() {
        swi<0x1C, void>();
    }
    always_inline
    inline void snd_vsync() {
        swi<0x1D, void>();
    }
    always_inline
    inline void snd_chan_clear() {
        swi<0x1E, void>();
    }
    //TODO: wavedata scruct?
    always_inline
    inline uint32_t snd_midi2freq(size_t* wavedata, uint8_t midikey, int8_t halftones) {
        return swi<0x1F, uint32_t>(wavedata, midikey, halftones);
    }
    // Mboot
    namespace secret {
        enum class mboot_mode : uint32_t {
            KHz256 = 0,
            KHz115 = 1,
            MHz2 = 2,
        };
    }
    //TODO: multiboot param scructure and transfer protocol
    always_inline
    inline uint32_t mboot(size_t* param, secret::mboot_mode mode = secret::mboot_mode::KHz115) {
        return swi<0x25, uint32_t>(param,mode);
    }
    namespace secret {
        always_inline
        inline void reset_hard() {
            swi<0x26, void>();
            unreachable();
        }
        always_inline
        inline void reset_custom(uint32_t val = 0x00) {
            uint32_t dummy = 0;
            swi<0x27, void>(dummy,dummy,val);
        }
    }
    always_inline
    inline void snd_vsync_off() {
        swi<0x28, void>();
    }
    always_inline
    inline void snd_vsync_on() {
        swi<0x29, void>();
    }
}
}
