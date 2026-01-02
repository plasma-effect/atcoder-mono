use bpaf::Bpaf;
use regex::Regex;
use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::f64;
use std::io::Read;
use std::io::Write;
use std::process::Command;
use std::vec;
use std::{fs::File, io, path::PathBuf};

use crate::manager_base::expand;
mod manager_base;

#[derive(Debug, Clone, Bpaf)]
#[bpaf(options, version)]
struct Opts {
    #[bpaf(long, long("setting"))]
    setting: PathBuf,
}

#[derive(Debug, Serialize, Deserialize)]
struct Setting {
    increase: bool,
    relative: bool,
    best: PathBuf,
    best_detail: PathBuf,
    compare: PathBuf,
    target: PathBuf,
    input: PathBuf,
    output: PathBuf,
    vis: PathBuf,
}

fn read_file(path: &PathBuf) -> io::Result<Vec<String>> {
    let mut f = File::open(path)?;
    let mut contents = String::new();
    f.read_to_string(&mut contents).unwrap();
    Ok(contents.lines().map(|x| x.to_string()).collect())
}

fn read_file_or_default(path: &PathBuf) -> HashMap<String, i64> {
    match read_file(&path) {
        Ok(lines) => expand(lines),
        Err(_) => HashMap::new(),
    }
}

fn read_best(path: &PathBuf, increase: bool) -> String {
    match File::open(path) {
        Ok(mut f) => {
            let mut line = String::new();
            f.read_to_string(&mut line).unwrap();
            line.trim_end().into()
        }
        Err(_) => {
            if increase {
                "1".to_string()
            } else {
                "100000000000".to_string()
            }
        }
    }
}

fn write_best(path: &PathBuf, best: String) {
    let mut f = File::create(path).unwrap();
    writeln!(&mut f, "{}", best).unwrap();
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

fn ratio(a: f64, b: f64, increase: bool) -> f64 {
    if increase {
        a / b
    } else if a == 0.0 {
        f64::INFINITY
    } else {
        b / a
    }
}

fn compare<T: PartialOrd>(a: T, b: T, increase: bool) -> bool {
    if increase { a > b } else { a < b }
}

fn print_compare_results_abs(path: &PathBuf, mut compare_results: Vec<(&String, i64, i64, i64)>) {
    let mut cmpf = File::create(path).unwrap();
    compare_results.sort_by(|a, b| a.0.cmp(b.0));
    for (name, d, score, best) in compare_results {
        writeln!(
            &mut cmpf,
            "{}: {} (now = {}, best = {})",
            name, d, score, best
        )
        .unwrap();
    }
}

fn absolute_check(
    results: &HashMap<String, i64>,
    bests: &mut HashMap<String, i64>,
    setting: &Setting,
) -> io::Result<()> {
    let mut sum: i64 = 0;
    let mut ranking = vec![];
    let mut compare_results = vec![];
    let increase = setting.increase;
    for (name, score) in results {
        sum += score;
        if !bests.contains_key(name) || compare(*score, bests[name], increase) {
            bests.insert(name.clone(), *score);
        }
        let d = -(score - bests[name]).abs();
        compare_results.push((name, d, *score, bests[name]));
        ranking.push((d, name.clone()));
    }
    print_compare_results_abs(&setting.compare, compare_results);
    let best: i64 = read_best(&setting.best, increase).parse().unwrap();
    let r = ratio(sum as f64, best as f64, increase);
    let r = if r > 5.0 {
        ">500.000%".to_string()
    } else {
        format!("{:.3}%", 100.0 * r)
    };
    if compare(sum, best, increase) {
        println!("update the best score by {} ({})", (best - sum).abs(), r);
        println!("new best score is {}", sum);
        write_best(&setting.best, sum.to_string());
    } else {
        println!(
            "{} (best = {}, {:.3}%)",
            sum,
            best,
            100.0 * ratio(sum as f64, best as f64, increase)
        );
    }
    ranking.sort();
    println!("check follow cases:");
    for (_, name) in ranking.iter().take(5) {
        let score = results[name];
        let best = bests[name];
        println!(
            "{}: {:+} (now = {}, best = {})",
            name,
            score - best,
            score,
            best
        );
    }
    Ok(())
}

fn rscore(score: i64) -> f64 {
    if score == 0 { 0.0 } else { (score as f64).ln() }
}

fn print_compare_results_rel(path: &PathBuf, mut compare_results: Vec<(&String, f64, i64, i64)>) {
    let mut cmpf = File::create(path).unwrap();
    compare_results.sort_by(|a, b| a.0.cmp(b.0));
    for (name, d, score, best) in compare_results {
        let relscore = rscore(score);
        let relbest = rscore(best);
        writeln!(
            &mut cmpf,
            "{}: {:7.3} (now = ({:.3}, {}), best = ({:.3}, {}))",
            name, d, relscore, score, relbest, best
        )
        .unwrap();
    }
}

fn relative_check(
    results: &HashMap<String, i64>,
    bests: &mut HashMap<String, i64>,
    setting: &Setting,
) -> io::Result<()> {
    let mut sum: f64 = 0.0;
    let mut ranking = vec![];
    let mut compare_results = vec![];
    let increase = setting.increase;
    for (name, score) in results {
        let relscore = rscore(*score);
        sum += relscore;
        if !bests.contains_key(name) || compare(*score, bests[name], increase) {
            bests.insert(name.clone(), *score);
        }
        let relbest = (bests[name] as f64).ln();
        let d = -(relscore - relbest).abs();
        ranking.push((d, name.clone()));
        compare_results.push((name, d, *score, bests[name]));
    }
    print_compare_results_rel(&setting.compare, compare_results);
    let best: f64 = read_best(&setting.best, increase).parse().unwrap();
    if compare(sum, best, increase) {
        println!("update the best score by {:.3}", (best - sum).abs());
        println!("new best score is {:.3}", sum);
        write_best(&setting.best, sum.to_string());
    } else {
        println!(
            "{:.3} (best = {:.3}, diff = {:.3})",
            sum,
            best,
            (sum - best).abs()
        );
    }
    ranking.sort_by(|(a0, a1), (b0, b1)| {
        if let Some(r) = a0.partial_cmp(b0)
            && r.is_ne()
        {
            return r;
        }
        return a1.cmp(b1);
    });
    println!("check follow cases:");
    println!("vs best");
    for (d, name) in ranking.iter().take(5) {
        let score = results[name];
        let best = bests[name];
        println!(
            "  {}: {:7.3} (now = ({:.3}, {}), best = ({:.3}, {}))",
            name,
            d,
            (score as f64).ln(),
            score,
            (best as f64).ln(),
            best
        );
    }
    Ok(())
}

fn main() -> io::Result<()> {
    let opts = opts().run();
    let mut setting = String::new();
    let mut f = File::open(opts.setting).unwrap();
    f.read_to_string(&mut setting).unwrap();
    let setting: Setting = serde_json::from_str(&setting).unwrap();
    let results = read_results(&setting.input, &setting.output, &setting.vis)?;
    let mut bests = read_file_or_default(&setting.best_detail);
    if !setting.relative {
        absolute_check(&results, &mut bests, &setting)?;
    } else {
        relative_check(&results, &mut bests, &setting)?;
    }
    let mut f = File::create(setting.best_detail).unwrap();
    for (name, score) in bests {
        writeln!(&mut f, "{} = {}", name, score)?;
    }
    return Ok(());
}
