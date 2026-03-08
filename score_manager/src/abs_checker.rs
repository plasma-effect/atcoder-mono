use crate::checker::Checker;
use crate::utils::{CompareResult, Setting, ratio, read_best, write_best};
use std::collections::HashMap;
use std::path::PathBuf;
pub struct AbsChecker {
    best: i64,
    bestpath: PathBuf,
    bests: HashMap<String, i64>,
    targets: HashMap<String, i64>,
    increase: bool,
}

fn compare<T: PartialOrd>(a: T, b: T, increase: bool) -> bool {
    if increase { a > b } else { a < b }
}
impl AbsChecker {
    pub fn create(
        bests: HashMap<String, i64>,
        targets: HashMap<String, i64>,
        setting: &Setting,
    ) -> Self {
        let best = read_best(&setting.best, setting.increase);
        AbsChecker {
            best,
            bestpath: setting.best.clone(),
            bests,
            targets,
            increase: setting.increase,
        }
    }
}

impl Checker for AbsChecker {
    fn compare(&mut self, results: HashMap<String, i64>) -> Vec<CompareResult> {
        let mut sum = 0;
        let mut compare_results = vec![];
        for (name, score) in &results {
            sum += *score;
            if !self.bests.contains_key(name) || compare(*score, self.bests[name], self.increase) {
                self.bests.insert(name.clone(), *score);
            }
            let target: Option<i64> = self.targets.get(name).copied();
            compare_results.push(CompareResult {
                filename: name.clone(),
                score: *score,
                best: self.bests[name],
                target,
            });
        }
        let r = ratio(sum, self.best, self.increase);
        let r = if r > 5.0 {
            ">500.000%".to_string()
        } else {
            format!("{:.3}%", 100.0 * r)
        };
        if compare(sum, self.best, self.increase) {
            let d = (self.best - sum).abs() as i64;
            println!("update the best score by {} ({})", d, r);
            println!("new best score is {}", sum as i64);
            self.best = sum;
            write_best(&self.bestpath, self.best);
        } else {
            let sum = sum as i64;
            let best = self.best as i64;
            println!("{} (best = {}, {})", sum, best, r);
        }
        compare_results
    }
    fn get_best_results<'a>(&'a self) -> &'a HashMap<String, i64> {
        &self.bests
    }
}
