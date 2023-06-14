/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * Software Interrupt aka. BIOS call helper/template
 * TODO: somehow allow/implement multiple return values
 */

#include <cstddef>
#include <cstdint>
#include <type_traits>

#pragma once

namespace gba::bios {
    //TODO: check if number of swi_args is < 4 and if argument fits one register (?)
    template <uint32_t swi_nr, class ret_val, class... swi_args> requires std::is_same_v<ret_val, void>
    [[gnu::naked]]
    inline void swi(swi_args...) noexcept {
        asm volatile (
#if defined( __thumb__ )
            "swi %[swi_nr]\n"
#elif defined( __arm__ )
            "swi %[swi_nr] << 16\n"
#else
            #error "Unsupported architecture or instruction set"
#endif
            "bx lr"
            :: [swi_nr]"i"(swi_nr)
        );
    }

    template <uint32_t swi_nr, class ret_val, class... swi_args>
    [[nodiscard]] [[gnu::naked]]
    inline auto swi(swi_args...) noexcept -> ret_val {
        asm volatile (
#if defined( __thumb__ )
            "swi %[swi_nr]\n"
#elif defined( __arm__ )
            "swi %[swi_nr] << 16\n"
#else
            #error "Unsupported architecture or instruction set"
#endif
            "bx lr"
            :: [swi_nr]"i"(swi_nr)
        );
    }


}
