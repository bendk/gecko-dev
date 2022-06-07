/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

// Opaque type used to represent a pointer from Rust
[ChromeOnly, Exposed=Window]
interface UniFFIPointer {};

// Types that can be passed or returned from scaffolding functions
//
// - double is used for all numeric types (including bool, which the JS code
//   coerces to an int)
// - ArrayBuffer is used for RustBuffer
// - UniFFIPointer is used for Arc pointers
typedef (double or ArrayBuffer or UniFFIPointer) UniFFIScaffoldingType;

// The result of a call into UniFFI scaffolding call
enum UniFFIScaffoldingCallCode {
   "success",         // Successful return
   "error",           // Rust Err return
   "internal-error",  // Internal/unexpected error
};

dictionary UniFFIScaffoldingCallResult {
    required UniFFIScaffoldingCallCode code;
    // For success, this will be the return value for non-void returns
    // For error, this will be an ArrayBuffer storing the serialized error value
    UniFFIScaffoldingType data;
    // For internal-error, this will be a string describing the error
    ByteString internalErrorMessage;
};
