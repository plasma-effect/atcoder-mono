use crate::utils::CompareResult;
use std::collections::HashMap;

pub trait Checker {
    fn compare(&mut self, results: HashMap<String, i64>) -> Vec<CompareResult>;
    fn get_best_results<'a>(&'a self) -> &'a HashMap<String, i64>;
    fn make_vs_best_results(&self, compare_results: &Vec<CompareResult>) -> Vec<String>;
    fn make_vs_target_results(&self, compare_results: &Vec<CompareResult>) -> Vec<String>;
}
