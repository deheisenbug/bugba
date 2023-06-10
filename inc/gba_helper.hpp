/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * Some helper functions/templates
 * TODO: move to own library?
 */

#include "gba_macros.hpp"

always_inline
inline void loop() {
  asm("1: b 1b");
}
