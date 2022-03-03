# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
import buildconfig
import os
import six
import subprocess

def generate(output, udl_file, output_type):
    args = [
        buildconfig.substs["UNIFFI_BINDGEN_GECKO_JS"],
        '--stdout',
        output_type,
        udl_file
    ]

    p = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    stdout = six.ensure_text(stdout)
    stderr = six.ensure_text(stderr)
    if p.returncode != 0:
        print(stderr)
        return p.returncode
    else:
        output.write(stdout)
        deps = set()
        deps.add(udl_file)
        # TODO: Add uniffi.toml and other dependencies (UNIFFI-140)
        return deps
