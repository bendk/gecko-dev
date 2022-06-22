/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsPrintfCString.h"
#include "mozilla/EndianUtils.h"
#include "mozilla/dom/UniFFIPointer.h"
#include "mozilla/dom/UniFFIBinding.h"
#include "mozilla/Logging.h"

static mozilla::LazyLogModule sUniFFIPointerLogger("uniffi_logger");

namespace mozilla::dom {
using uniffi::UniFFIPointerType;

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(UniFFIPointer)

NS_IMPL_CYCLE_COLLECTING_ADDREF(UniFFIPointer)
NS_IMPL_CYCLE_COLLECTING_RELEASE(UniFFIPointer)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(UniFFIPointer)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

// Static function
already_AddRefed<UniFFIPointer> UniFFIPointer::Create(
    void* aPtr, UniFFIPointerType* aType) {
  RefPtr<UniFFIPointer> uniFFIPointer = new UniFFIPointer(aPtr, aType);
  return uniFFIPointer.forget();
}

already_AddRefed<UniFFIPointer> UniFFIPointer::Read(
    const ArrayBuffer& aArrayBuff, long aPosition, UniFFIPointerType* aType) {
  MOZ_LOG(sUniFFIPointerLogger, LogLevel::Info,
          ("[UniFFI] Reading Pointer from buffer"));
  aArrayBuff.ComputeState();
  // in Rust and Write(), a pointer is converted to a void* then written as u64
  // BigEndian we do the reverse here
  uint8_t* data_ptr = aArrayBuff.Data() +
                      aPosition;  // Pointer arithmetic, move by position bytes
  void* ptr = (void*)mozilla::BigEndian::readUint64(data_ptr);
  return UniFFIPointer::Create(ptr, aType);
}

void UniFFIPointer::Write(const ArrayBuffer& aArrayBuff, long aPosition,
                          UniFFIPointerType* aType, ErrorResult& aError) const {
  if (!this->IsSamePtrType(aType)) {
    aError.ThrowUnknownError(nsPrintfCString(
        "Attempt to write pointer with wrong type: %s (expected: %s)",
        aType->GetTypeName().get(), this->type->GetTypeName().get()));
    return;
  }
  MOZ_LOG(sUniFFIPointerLogger, LogLevel::Info,
          ("[UniFFI] Writing Pointer to buffer"));
  aArrayBuff.ComputeState();

  // in Rust and Read(), a u64 is read as BigEndian and then converted to a
  // pointer we do the reverse here
  uint8_t* data_ptr = aArrayBuff.Data() +
                      aPosition;  // Pointer arithmetic, move by position bytes
  mozilla::BigEndian::writeUint64(data_ptr, (uint64_t)GetPtr());
}

UniFFIPointer::UniFFIPointer(void* aPtr, UniFFIPointerType* aType) {
  ptr = aPtr;
  type = aType;
}

JSObject* UniFFIPointer::WrapObject(JSContext* aCx,
                                    JS::Handle<JSObject*> aGivenProto) {
  return dom::UniFFIPointer_Binding::Wrap(aCx, this, aGivenProto);
}

void* UniFFIPointer::GetPtr() const {
  MOZ_LOG(sUniFFIPointerLogger, LogLevel::Info,
          ("[UniFFI] Getting raw pointer"));
  return this->ptr;
}

bool UniFFIPointer::IsSamePtrType(const UniFFIPointerType* type) const {
  return this->type == type;
}

UniFFIPointer::~UniFFIPointer() {
  MOZ_LOG(sUniFFIPointerLogger, LogLevel::Info,
          ("[UniFFI] Destroying pointer"));
  this->type->DestroyPtr(this->ptr);
}

}  // namespace mozilla::dom
