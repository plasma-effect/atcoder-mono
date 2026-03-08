use crate::checker::Checker;
use crate::compare;
use crate::utils::CompareResult;
use crate::utils::Setting;
use crate::utils::ratio;
use crate::utils::read_best;
use crate::utils::write_best;
use std::{collections::HashMap, path::PathBuf};

pub struct RelChecker {
    best: f64,
    bestpath: PathBuf,
    bests: HashMap<String, i64>,
    targets: HashMap<String, i64>,
    increase: bool,
}

pub fn rscore(score: i64) -> f64 {
    if score == 0 { 0.0 } else { (score as f64).ln() }
}
impl RelChecker {
    pub fn create(
        bests: HashMap<String, i64>,
        targets: HashMap<String, i64>,
        setting: &Setting,
    ) -> Self {
        let best = read_best(&setting.best, setting.increase);
        RelChecker {
            best,
            bestpath: setting.best.clone(),
            bests,
            targets,
            increase: setting.increase,
        }
    }
}

impl Checker for RelChecker {
    fn compare(&mut self, results: HashMap<String, i64>) -> Vec<crate::utils::CompareResult> {
        let mut rsum = 0.0;
        let mut tsum: Option<f64> = Some(0.0);
        let mut compare_results = vec![];
        for (name, score) in results {
            let target: Option<i64> = self.targets.get(&name).copied();
            rsum += rscore(score);
            tsum = if let Some(v) = tsum
                && let Some(u) = target
            {
                Some(v + ratio(score, u, self.increase))
            } else {
                None
            };
            if !self.bests.contains_key(&name) || compare(score, self.bests[&name], self.increase) {
                self.bests.insert(name.clone(), score);
            }
            compare_results.push(CompareResult {
                filename: name.clone(),
                score,
                best: self.bests[&name],
                target,
            });
        }
        if compare(rsum, self.best, self.increase) {
            println!("update the best score by {:.3}", (self.best - rsum).abs());
            println!("new best score is {:.3}", rsum);
            self.best = rsum;
            write_best(&self.bestpath, self.best);
        } else {
            println!(
                "{:.3} (best = {:.3}, diff = {:.3})",
                rsum,
                self.best,
                (rsum - self.best).abs()
            );
        }
        compare_results
    }

    fn get_best_results<'a>(&'a self) -> &'a HashMap<String, i64> {
        &self.bests
    }
}
