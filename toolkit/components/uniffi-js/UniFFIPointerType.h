/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_UniFFIPointerType_h
#define mozilla_UniFFIPointerType_h

#include "nsISupports.h"
#include "nsWrapperCache.h"
#include "nsString.h"
#include "UniFFIRust.h"

namespace mozilla::uniffi {

/**
 * UniFFIPointerType is the base class for the types of all
 * UniFFI allocated pointers.
 * Each UniFFIPointer will have a UniFFIPointerType, which will be a statically
 * allocated type per object exposed by the UniFFI interface
 **/
class UniFFIPointerType {
 public:
  const nsCString& GetTypeName();

  // Destroys the pointer by passing it to the Rust destructor
  // the Rust destructor will give back ownership of the pointer to Rust which
  // will deallocate it.
  void DestroyPtr(void* ptr);

 protected:
  nsCString typeName;
  // The Rust destructor for the pointer, this gives back ownership to Rust
  void (*destructor)(void*, RustCallStatus*);
};
}  // namespace mozilla::uniffi

#endif /* mozilla_UniFFIPointerType_h */
