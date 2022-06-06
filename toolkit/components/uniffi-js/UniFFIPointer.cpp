/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/UniFFIPointer.h"
#include "mozilla/dom/UniFFIPointerBinding.h"
#include "mozilla/Logging.h"

static mozilla::LazyLogModule sUniFFIPointerLogger("uniffi_logger");
namespace mozilla::dom {

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_0(UniFFIPointer)

NS_IMPL_CYCLE_COLLECTING_ADDREF(UniFFIPointer)
NS_IMPL_CYCLE_COLLECTING_RELEASE(UniFFIPointer)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(UniFFIPointer)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

// Static function
already_AddRefed<UniFFIPointer> UniFFIPointer::Create(void *aPtr, UniFFIPointerType* aType) {
  RefPtr<UniFFIPointer> uniFFIPointer = new UniFFIPointer(aPtr, aType);
  return uniFFIPointer.forget();
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
    MOZ_LOG(sUniFFIPointerLogger, LogLevel::Info, ("[UniFFI] Getting raw pointer"));
    return this->ptr;
}

bool UniFFIPointer::IsSamePtrType(const UniFFIPointerType* type) const {
    return this->type == type;
}

UniFFIPointer::~UniFFIPointer() {
  MOZ_LOG(sUniFFIPointerLogger, LogLevel::Info, ("[UniFFI] Destroying pointer"));
  this->type->DestroyPtr(this->ptr);
}

} // namespace mozilla::dom