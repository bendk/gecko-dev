/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_UniFFI_h
#define mozilla_dom_UniFFI_h

#include <stdint.h>

namespace uniffi {
// Valid values for UniFFIRustCallResult.code and RustCallStatus.code
const int8_t CALL_SUCCESS = 0;
const int8_t CALL_ERROR = 1;
const int8_t CALL_INTERNAL_ERROR = 2;
}  // namespace uniffi

// structs/functions from UniFFI
extern "C" {
struct RustBuffer {
  int32_t capacity;
  int32_t len;
  uint8_t* data;
};

struct RustCallStatus {
  int8_t code;
  RustBuffer error_buf;
};

RustBuffer uniffi_rustbuffer_alloc(int32_t size, RustCallStatus* call_status);
void uniffi_rustbuffer_free(RustBuffer buf, RustCallStatus* call_status);
}

#endif /* mozilla_dom_UniFFI_h */
