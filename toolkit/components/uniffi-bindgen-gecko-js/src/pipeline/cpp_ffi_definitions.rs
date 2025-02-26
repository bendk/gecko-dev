/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

use super::*;

use std::collections::HashSet;

pub fn pass(root: &mut Root) -> Result<()> {
    // Make sure to only include each FFI definitions once since we're combining FFI definitions
    // from different modules.
    let mut seen = HashSet::new();
    root.cpp_scaffolding.ffi_definitions = CombinedItems::new(root, |module, _, items| {
        module.visit(|def: &FfiDefinition| {
            let name = match def {
                FfiDefinition::RustFunction(FfiFunction { name, .. })
                | FfiDefinition::FunctionType(FfiFunctionType { name, .. })
                | FfiDefinition::Struct(FfiStruct { name, .. }) => name,
            };
            if seen.insert(name.to_string()) {
                items.push(def.clone())
            }
        })
    });
    Ok(())
}
