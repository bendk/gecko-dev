/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

use anyhow::{Context, Result};
use clap::{ArgEnum, Parser};
use heck::ToUpperCamelCase;
use serde::Deserialize;
use std::fs::File;
use std::io::Write;
use std::path::{Path, PathBuf};
use uniffi_bindgen::{
    generate_external_bindings, BindingGenerator, ComponentInterface, EmptyBindingGeneratorConfig,
};

mod render;

#[derive(ArgEnum, Copy, Clone, Debug, Deserialize, PartialEq, PartialOrd, Ord, Eq)]
enum Mode {
    Webidl,
    CPPHeader,
    CPP,
    JS,
}

#[derive(Clone, Parser)]
#[clap(name = "uniffi-bindgen-gecko-js")]
#[clap(version = clap::crate_version!())]
#[clap(about = "JS bindings generator for Rust")]
#[clap(propagate_version = true)]
struct CliArgs {
    /// Write output to STDOUT
    #[clap(long)]
    stdout: bool,

    /// Write output to path
    #[clap(long, parse(from_os_str), value_name = "FILE")]
    out: Option<PathBuf>,

    /// Directory in which to write generated files. Default is same folder as .udl file.
    #[clap(long, parse(from_os_str), value_name = "DIR")]
    out_dir: Option<PathBuf>,

    /// Path to the optional uniffi config file. If not provided, uniffi-bindgen will try to guess it from the UDL's file location.
    #[clap(long, parse(from_os_str), value_name = "DIR")]
    config_path: Option<PathBuf>,

    /// Generation mode
    #[clap(arg_enum)]
    mode: Mode,

    /// Path to the UDL file
    udl_file: PathBuf,
}

struct GeckoJsBindingGenerator {
    args: CliArgs,
}

// Use EmptyBindingGeneratorConfig for now
type Config = EmptyBindingGeneratorConfig;

impl GeckoJsBindingGenerator {
    fn new(args: CliArgs) -> Self {
        Self { args }
    }

    fn create_writer(
        &self,
        ci: &ComponentInterface,
        out_dir: &Path,
    ) -> anyhow::Result<Box<dyn Write>> {
        if self.args.stdout {
            return Ok(Box::new(std::io::stdout()));
        }

        let out_path = self.args.out.clone().unwrap_or_else(|| {
            let filename = self.calc_filename(ci);
            out_dir.join(&filename)
        });
        Ok(Box::new(File::create(&out_path).context(format!(
            "Failed to create {:?}",
            out_path.file_name()
        ))?))
    }

    fn calc_filename(&self, ci: &ComponentInterface) -> String {
        match self.args.mode {
            Mode::Webidl => format!("{}Scaffolding.webidl", ci.namespace().to_upper_camel_case()),
            Mode::CPP => format!("{}Scaffolding.cpp", ci.namespace().to_upper_camel_case()),
            Mode::CPPHeader => format!("{}Scaffolding.h", ci.namespace().to_upper_camel_case()),
            Mode::JS => format!("{}.jsm", ci.namespace().to_upper_camel_case()),
        }
    }
}

impl BindingGenerator for GeckoJsBindingGenerator {
    type Config = Config;

    fn write_bindings(
        &self,
        ci: ComponentInterface,
        config: Self::Config,
        out_dir: &Path,
    ) -> anyhow::Result<()> {
        let writer = self.create_writer(&ci, out_dir)?;
        render::render_file(self.args.mode, ci, config, writer)
    }
}

pub fn run_main() -> Result<()> {
    let args = CliArgs::parse();
    let binding_generator = GeckoJsBindingGenerator::new(args.clone());
    generate_external_bindings(
        binding_generator,
        args.udl_file,
        args.config_path,
        args.out_dir,
    )
}
