use core::f64;
use serde::{Deserialize, Serialize};
use std::io::{Read, Write};
use std::{fs::File, path::PathBuf, str::FromStr};

#[derive(Debug, Serialize, Deserialize)]
pub struct Setting {
    pub increase: bool,
    pub relative: bool,
    pub best: PathBuf,
    pub best_detail: PathBuf,
    pub compare: PathBuf,
    pub target: PathBuf,
    pub input: PathBuf,
    pub output: PathBuf,
    pub vis: PathBuf,
}

pub struct CompareResult {
    pub filename: String,
    pub score: i64,
    pub best: i64,
    pub target: Option<i64>,
}

pub(crate) trait Value64 {
    fn default(increase: bool) -> Self;
    fn to_f64(&self) -> f64;
}
impl Value64 for i64 {
    fn default(increase: bool) -> Self {
        if increase { 1 } else { 100000000000 }
    }
    fn to_f64(&self) -> f64 {
        *self as f64
    }
}
impl Value64 for f64 {
    fn default(increase: bool) -> Self {
        if increase { 1.0 } else { 100000000000.0 }
    }
    fn to_f64(&self) -> f64 {
        *self
    }
}

pub fn compare<T: PartialOrd>(a: T, b: T, increase: bool) -> bool {
    if increase { a > b } else { a < b }
}

pub fn ratio<T: Value64>(a: T, b: T, increase: bool) -> f64 {
    let a = a.to_f64();
    let b = b.to_f64();
    if increase {
        a / b
    } else if a == 0.0 {
        f64::INFINITY
    } else {
        b / a
    }
}

pub fn read_best<T: FromStr + Value64>(path: &PathBuf, increase: bool) -> T {
    match File::open(path) {
        Ok(mut f) => {
            let mut line = String::new();
            f.read_to_string(&mut line).unwrap();
            match line.trim_end().to_string().parse() {
                Ok(v) => v,
                Err(_) => T::default(increase),
            }
        }
        Err(_) => T::default(increase),
    }
}

pub fn write_best<T: ToString>(path: &PathBuf, best: T) {
    let mut f = File::create(path).unwrap();
    writeln!(&mut f, "{}", best.to_string()).unwrap();
}
