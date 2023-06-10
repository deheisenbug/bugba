/*
 * SPDX-License-Identifier: LGPL-3.0-or-later
 * Copyright 2023 heisenbug heisenbug [at] mailbox _dot_ org
 */

/**
 * Common register templates/classes
 */

#include <cstddef>
#include "gba_macros.hpp"

#pragma once

namespace gba::dtypes {
  /**
   * Register backing type template.
   * The register backing type is used to copy the base type data.
   * Both types need to have the same size.
   *
   * @param reg_base_t register base type
   * @param reg_back_t register backing type
   */
  template<typename reg_base_t, typename reg_back_t>
  packed(class) alignas(sizeof(reg_back_t)) reg_t : public reg_base_t {
    public:
      inline reg_base_t& operator =(const reg_base_t& other) {
        *reinterpret_cast<volatile reg_back_t*>(this) = *reinterpret_cast<const reg_back_t*>(&other);
        return *this;
      }
      inline reg_base_t& operator |(const reg_base_t& other) {
        *reinterpret_cast<volatile reg_back_t*>(this) = *reinterpret_cast<volatile reg_back_t*>(this) | *reinterpret_cast<const reg_back_t*>(&other);
        return *this;
      }
      inline reg_base_t& operator &(const reg_base_t& other) {
        *reinterpret_cast<volatile reg_back_t*>(this) = *reinterpret_cast<volatile reg_back_t*>(this) & *reinterpret_cast<const reg_back_t*>(&other);
        return *this;
      }
      static_assert(
        sizeof(reg_base_t) == sizeof(reg_back_t) //#INFO: compiler depended because __attribute__((packed))
      );
    //#TODO: how to check if resulting class is standard layout?
  };
  /**
   * Base class for memory mapped I/O registers.
   * Ensures, that no memory mapped registers are created dynamically.
   */
  class io_reg {
    public:
      void *operator new(std::size_t count) = delete;
      void operator delete(void * ptr) = delete;
  };
}
