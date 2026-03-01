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

pub fn make_ratio(increase: bool) -> fn(i64, i64) -> f64 {
    if increase {
        |a, b| (a as f64) / (b as f64)
    } else {
        |a, b| {
            if a == 0 {
                f64::INFINITY
            } else {
                (b as f64) / (a as f64)
            }
        }
    }
}

pub fn make_compare(increase: bool) -> fn(i64, i64) -> bool {
    if increase { |a, b| a > b } else { |a, b| a < b }
}

trait HasDefault {
    fn default(increase: bool) -> Self;
}
impl HasDefault for i64 {
    fn default(increase: bool) -> Self {
        if increase { 1 } else { 100000000000 }
    }
}
impl HasDefault for f64 {
    fn default(increase: bool) -> Self {
        if increase { 1.0 } else { 100000000000.0 }
    }
}

pub fn read_best<T: FromStr + HasDefault>(path: &PathBuf, increase: bool) -> T {
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
