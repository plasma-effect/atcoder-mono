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

fn main() -> io::Result<()> {
    let opts = opts().run();
    let mut setting = String::new();
    let mut f = File::open(opts.setting).unwrap();
    f.read_to_string(&mut setting).unwrap();
    let setting: Setting = serde_json::from_str(&setting).unwrap();
    let results = read_results(&setting.input, &setting.output, &setting.vis)?;
    let bests = read_file_or_default(&setting.best_detail);
    let targets = read_file_or_default(&setting.target);
    if !setting.relative {
        let mut checker = AbsChecker::create(bests, targets, &setting);
        let mut compare_results = checker.compare(results);
        let mut cmpf = File::create(&setting.best_detail).unwrap();
        compare_results.sort_by(|a, b| a.filename.cmp(&b.filename));
        for result in &compare_results {
            let score = result.score;
            let best = result.best;
            let diff = score - best;
            writeln!(
                &mut cmpf,
                "{}: {} (now = {}, best = {})",
                &result.filename, diff, score, best
            )
            .unwrap();
        }
        let compare = setting.compare;
        compare_results.sort_by(|a, b| {
            if compare {
                let d0 = -(a.best - a.score).abs();
                let d1 = -(b.best - b.score).abs();
                d0.cmp(&d1)
            } else {
                a.score.cmp(&b.score)
            }
        });
        println!(
            "check follow cases (sorted by {}):",
            if compare { "difference" } else { "score" }
        );
        for result in compare_results.iter().take(5) {
            let score = result.score;
            let best = result.best;
            if compare {
                println!(
                    "{}: {:+} (now = {}, best = {})",
                    &result.filename,
                    score - best,
                    score,
                    best
                );
            } else {
                println!(
                    "{}: {} (best = {}, diff = {:+})",
                    &result.filename,
                    score,
                    best,
                    score - best
                );
            }
        }
        let bests = checker.get_best_results();
        let mut f = File::create(setting.best_detail).unwrap();
        for (name, score) in bests {
            writeln!(&mut f, "{} = {}", name, score)?;
        }
    } else {
        let mut checker = RelChecker::create(bests, targets, &setting);
        let mut compare_results = checker.compare(results);
        let mut vs_best = vec![];
        let mut vs_target = vec![];
        let mut cmpf = File::create(setting.compare_detail).unwrap();
        compare_results.sort_by(|a, b| a.filename.cmp(&b.filename));
        for result in &compare_results {
            let score = result.score;
            let relscore = rscore(score);
            let best = result.best;
            let relbest = rscore(best);
            let name = &result.filename;
            let d0 = (rscore(score) - rscore(best)).abs();
            vs_best.push((-d0, score, best, name.clone()));
            if let Some(target) = result.target {
                let reltarget = rscore(target);
                let d1 = ratio(score, target, setting.increase);
                writeln!(
                    &mut cmpf,
                    "{}: {:7.3} (now = ({:.3}, {}), best = ({:.3}, {}), target = ({:.3}, {}))",
                    name, d1, relscore, score, relbest, best, reltarget, target
                )
                .unwrap();
                vs_target.push((d1, score, target, result.filename.clone()));
            } else {
                writeln!(
                    &mut cmpf,
                    "{}: {:7.3} (now = ({:.3}, {}), best = ({:.3}, {}))",
                    name, d0, relscore, score, relbest, best
                )
                .unwrap();
            }
        }
        vs_best.sort_by(|a, b| a.0.total_cmp(&b.0));
        vs_target.sort_by(|a, b| a.0.total_cmp(&b.0));

        println!("check follow cases (sorted by difference):");
        println!("vs best");
        for result in vs_best.iter().take(5) {
            let (d, score, best, name) = result;
            let d = d.abs();
            let rs = rscore(*score);
            let rb = rscore(*best);
            println!(
                "  {}: {:7.3} (now = ({:.3}, {}), best = ({:.3}, {}))",
                name, d, rs, score, rb, best
            );
        }
        if vs_target.len() > 0 {
            println!("vs target");
            for result in vs_target.iter().take(5) {
                let (r, score, target, name) = result;
                let r = 100. * r.abs();
                let rs = rscore(*score);
                let rt = rscore(*target);
                println!(
                    "  {}: {:7.3}% (now = ({:.3}, {}), target = ({:.3}, {}))",
                    name, r, rs, score, rt, target,
                );
            }
        }
        let bests = checker.get_best_results();
        let mut f = File::create(setting.best_detail).unwrap();
        for (name, score) in bests {
            writeln!(&mut f, "{} = {}", name, score)?;
        }
    }
    return Ok(());
}
