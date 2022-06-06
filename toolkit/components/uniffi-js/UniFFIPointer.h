/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_UniFFIPointer_h
#define mozilla_dom_UniFFIPointer_h

#include "nsISupports.h"
#include "nsWrapperCache.h"
#include "nsString.h"
#include "mozilla/dom/UniFFIPointerType.h"



namespace mozilla::dom {

class UniFFIPointer final: public nsISupports, public nsWrapperCache {
  public:
    NS_DECL_CYCLE_COLLECTING_ISUPPORTS
    NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(UniFFIPointer)

    static already_AddRefed<UniFFIPointer> Create(void *ptr, UniFFIPointerType* type);

    UniFFIPointer(void *ptr, UniFFIPointerType* type);

    JSObject* WrapObject(JSContext* aCx,
                        JS::Handle<JSObject*> aGivenProto) override;
    nsISupports* GetParentObject() { return nullptr; }
    void *getPtr() const;

    bool isSamePtrType(const UniFFIPointerType* type) const;


  private:
    UniFFIPointerType* type;  
    void *ptr;

  protected:
  ~UniFFIPointer();

};
}  // namespace mozilla::dom

#endif /* mozilla_dom_UniFFIPointer_h */

