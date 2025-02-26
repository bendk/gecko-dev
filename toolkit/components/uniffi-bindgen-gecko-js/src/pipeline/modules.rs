/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

use super::*;
use crate::Config;
use std::collections::HashMap;

pub fn pass(mut config_map: HashMap<String, Config>) -> impl FnMut(&mut Module) -> Result<()> {
    move |module: &mut Module| {
        module.config = config_map.remove(&module.crate_name).unwrap_or_default();
        // Set the `fixture` flag for modules that belong to the text fixtures
        // To do this in a general way, we would need to remember which `Metadata::Namespace` items
        // came from the fixtures library and create a closure that captured that set.
        //
        // However, since we only have two fixture modules, we can do this very easily.
        if module.name.starts_with("uniffi_bindings_tests") {
            module.fixture = true;
        }
        Ok(())
    }
}
