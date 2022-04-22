# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import os
import subprocess
from mach.decorators import (
    CommandArgument,
    Command,
    SubCommand,
)

generated_dir = "toolkit/components/uniffi-bindgen-gecko-js/fixtures/generated"
generated_bindings_paths = {
    # UniFFI Examples/Fixtures
    "third_party/rust/uniffi-example-geometry/src/geometry.udl": {
        "webidl": "dom/chrome-webidl/GeometryScaffolding.webidl",
        "cpp-header": f"{generated_dir}/GeometryScaffolding.h",
        "cpp": f"{generated_dir}/GeometryScaffolding.cpp",
        "js": f"{generated_dir}/Geometry.jsm",
    },
    "third_party/rust/uniffi-example-arithmetic/src/arithmetic.udl": {
        "webidl": "dom/chrome-webidl/ArithmeticScaffolding.webidl",
        "cpp-header": f"{generated_dir}/ArithmeticScaffolding.h",
        "cpp": f"{generated_dir}/ArithmeticScaffolding.cpp",
        "js": f"{generated_dir}/Arithmetic.jsm",
    },
    "third_party/rust/uniffi-example-rondpoint/src/rondpoint.udl": {
        "webidl": "dom/chrome-webidl/RondpointScaffolding.webidl",
        "cpp-header": f"{generated_dir}/RondpointScaffolding.h",
        "cpp": f"{generated_dir}/RondpointScaffolding.cpp",
        "js": f"{generated_dir}/Rondpoint.jsm",
    },
    "third_party/rust/uniffi-example-sprites/src/sprites.udl": {
        "webidl": "dom/chrome-webidl/SpritesScaffolding.webidl",
        "cpp-header": f"{generated_dir}/SpritesScaffolding.h",
        "cpp": f"{generated_dir}/SpritesScaffolding.cpp",
        "js": f"{generated_dir}/Sprites.jsm",
    },
    "third_party/rust/uniffi-example-todolist/src/todolist.udl": {
        "webidl": "dom/chrome-webidl/TodolistScaffolding.webidl",
        "cpp-header": f"{generated_dir}/TodolistScaffolding.h",
        "cpp": f"{generated_dir}/TodolistScaffolding.cpp",
        "js": f"{generated_dir}/Todolist.jsm",
    },
}


def build_uniffi_bindgen_gecko_js(command_context):
    uniffi_root = crate_root(command_context)
    print("Building uniffi-bindgen-gecko-js")
    cmdline = [
        "cargo",
        "build",
        "--release",
        "--manifest-path",
        os.path.join(command_context.topsrcdir, "Cargo.toml"),
        "--package",
        "uniffi-bindgen-gecko-js",
    ]
    subprocess.check_call(cmdline, cwd=uniffi_root)
    print()
    return os.path.join(
        command_context.topsrcdir, "target", "release", "uniffi-bindgen-gecko-js"
    )


@Command(
    "uniffi",
    category="devenv",
    description="Generate JS bindings using uniffi-bindgen-gecko-js",
)
def uniffi(command_context, *runargs, **lintargs):
    """Run uniffi."""
    command_context._sub_mach(["help", "uniffi"])
    return 1


@SubCommand(
    "uniffi",
    "generate",
    description="Generate/regenerate bindings",
)
def generate_command(command_context):
    binary_path = build_uniffi_bindgen_gecko_js(command_context)

    for udl_path, generated_files in generated_bindings_paths.items():
        for mode, path in generated_files.items():
            print(f"Generating {path} from {udl_path} ({mode} mode)")
            subprocess.check_call([binary_path, "--out", path, mode, udl_path])
    return 0


@SubCommand(
    "uniffi",
    "print",
    description="Print generated binding to STOUT",
)
@CommandArgument(
    "filename",
    help="Filename to generate",
)
def print_command(command_context, filename):
    binary_path = build_uniffi_bindgen_gecko_js(command_context)
    for udl_path, generated_files in generated_bindings_paths.items():
        for mode, path in generated_files.items():
            if os.path.basename(path) == filename:
                subprocess.check_call([binary_path, "--stdout", mode, udl_path])
                return 0
    print(f"Don't know how to generate {filename}")
    return 1


def crate_root(command_context):
    return os.path.join(
        command_context.topsrcdir, "toolkit", "components", "uniffi-bindgen-gecko-js"
    )
