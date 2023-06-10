/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * helper macros. keep at minimum!
 */

#pragma once

#define packed(type) type __attribute__((packed))
#define align(n) __attribute__((aligned(n)))
#define always_inline __attribute__((always_inline))
#define unreachable() __builtin_unreachable()
