/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/UniFFIPointerType.h"
#include "mozilla/Logging.h"

static mozilla::LazyLogModule sUniFFIPointerTypeLogger("uniffi_logger");
namespace mozilla::uniffi {

const nsCString& UniFFIPointerType::GetTypeName() { return this->typeName; }

void UniFFIPointerType::DestroyPtr(void* ptr) {
  MOZ_LOG(sUniFFIPointerTypeLogger, LogLevel::Info,
          ("[UniFFI] Calling the object's Rust destructor"));
  RustCallStatus status;
  this->destructor(ptr, &status);
  NS_ASSERTION(status.code == RUST_CALL_SUCCESS,
               "UniFFI destructor call returned a non-success result");
}

}  // namespace mozilla::uniffi
