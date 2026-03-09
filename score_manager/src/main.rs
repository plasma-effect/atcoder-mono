use bpaf::Bpaf;
use regex::Regex;
use std::collections::HashMap;
use std::io::Read;
use std::io::Write;
use std::process::Command;
use std::vec;
use std::{fs::File, io, path::PathBuf};

use crate::abs_checker::AbsChecker;
use crate::checker::Checker;
use crate::manager_base::expand;
use crate::rel_checker::{RelChecker, rscore};
use crate::utils::CompareResult;
use crate::utils::{Setting, compare, ratio};

mod abs_checker;
mod checker;
mod manager_base;
mod rel_checker;
mod utils;

#[derive(Debug, Clone, Bpaf)]
#[bpaf(options, version)]
struct Opts {
    #[bpaf(long, long("setting"))]
    setting: PathBuf,
}

fn read_file_base(path: &PathBuf) -> io::Result<Vec<String>> {
    let mut f = File::open(path)?;
    let mut contents = String::new();
    f.read_to_string(&mut contents).unwrap();
    Ok(contents.lines().map(|x| x.to_string()).collect())
}

fn read_file_or_default(path: &PathBuf) -> HashMap<String, i64> {
    match read_file_base(&path) {
        Ok(lines) => expand(lines),
        Err(_) => HashMap::new(),
    }
}

fn read_results(
    input: &PathBuf,
    output: &PathBuf,
    vis: &PathBuf,
) -> std::io::Result<HashMap<String, i64>> {
    let mut results = HashMap::new();
    let r = Regex::new("^Score = (\\d+)").unwrap();
    for entry in std::fs::read_dir(&input)? {
        if let Ok(dir) = entry {
            let filename = dir
                .path()
                .file_name()
                .unwrap()
                .to_str()
                .unwrap()
                .to_string();
            let output_filename = output.join(&filename);
            let output = Command::new(&vis)
                .args([&dir.path(), &output_filename])
                .output()?;
            if let Ok(lines) = String::from_utf8(output.stdout) {
                for line in lines.lines() {
                    if let Some(cap) = r.captures(&line) {
                        results.insert(
                            filename.clone(),
                            cap.get(1).unwrap().as_str().to_string().parse().unwrap(),
                        );
                        break;
                    }
                }
            }
        }
    }
    Ok(results)
}

fn write_compare_result(setting: &Setting, compare_results: &Vec<CompareResult>) {
    let mut cmpf = File::create(&setting.compare_detail).unwrap();
    for result in compare_results {
        let score = result.score;
        let best = result.best;
        let name = &result.filename;
        if !setting.relative {
            let diff = score - best;
            writeln!(
                &mut cmpf,
                "{}: {} (now = {}, best = {})",
                name, diff, score, best
            )
        } else {
            let relscore = rscore(score);
            let relbest = rscore(best);
            if let Some(target) = result.target {
                let reltarget = rscore(target);
                let ratio = ratio(score, target, setting.increase);
                writeln!(
                    &mut cmpf,
                    "{}: {:7.3} (now = ({:.3}, {}), best = ({:.3}, {}), target = ({:.3}, {}))",
                    name, ratio, relscore, score, relbest, best, reltarget, target
                )
            } else {
                let reldiff = (rscore(score) - rscore(best)).abs();
                writeln!(
                    &mut cmpf,
                    "{}: {:7.3} (now = ({:.3}, {}), best = ({:.3}, {}))",
                    name, reldiff, relscore, score, relbest, best
                )
            }
        }
        .unwrap();
    }
}

fn main() -> io::Result<()> {
    let opts = opts().run();
    let mut setting = String::new();
    let mut f = File::open(opts.setting).unwrap();
    f.read_to_string(&mut setting).unwrap();
    let setting: Setting = serde_json::from_str(&setting).unwrap();
    let results = read_results(&setting.input, &setting.output, &setting.vis)?;
    let bests = read_file_or_default(&setting.best_detail);
    let targets = read_file_or_default(&setting.target);
    let mut checker: Box<dyn Checker> = if !setting.relative {
        Box::new(AbsChecker::create(bests, targets, &setting))
    } else {
        Box::new(RelChecker::create(bests, targets, &setting))
    };
    let mut compare_results = checker.compare(results);
    compare_results.sort_by(|a, b| a.filename.cmp(&b.filename));
    write_compare_result(&setting, &compare_results);
    let vs_best = checker.make_vs_best_results(&compare_results);
    let vs_target = checker.make_vs_target_results(&compare_results);
    println!("check follow cases:",);
    println!("vs best:");
    for message in vs_best.iter().take(5) {
        println!("{}", message);
    }
    if vs_target.len() > 0 {
        if !setting.relative {
            println!("absolute score:");
        } else {
            println!("vs target:");
        }
        for message in vs_target.iter().take(5) {
            println!("{}", message);
        }
    }
    let bests = checker.get_best_results();
    let mut f = File::create(setting.best_detail).unwrap();
    for (name, score) in bests {
        writeln!(&mut f, "{} = {}", name, score)?;
    }
    return Ok(());
}
