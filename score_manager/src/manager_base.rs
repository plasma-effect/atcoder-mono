use regex::Regex;
use std::collections::HashMap;

fn create_regex(r: &str) -> Regex {
    return Regex::new(r).unwrap();
}

pub fn expand(lines: Vec<String>) -> HashMap<String, i64> {
    let mut output = HashMap::new();
    let r = create_regex("^\\s*([^=]*\\S)\\s*=\\s*(\\d+)");
    for line in lines {
        if let Some(cap) = r.captures(&line) {
            let name = cap.get(1).unwrap().as_str().to_string();
            let score: i64 = cap.get(2).unwrap().as_str().to_string().parse().unwrap();
            output.insert(name, score);
        }
    }
    return output;
}

#[cfg(test)]
mod tests {
    use crate::manager_base::expand;
    fn setup(strs: Vec<&str>) -> Vec<String> {
        let mut ret = vec![];
        ret.reserve(strs.len());
        for str in strs {
            ret.push(str.to_string());
        }
        return ret;
    }

    #[test]
    fn expand_works() {
        let lines = setup(vec![
            "0000.txt=1",
            "0001.txt =10",
            "0002.txt= 100",
            "0003.txt = 1000",
        ]);
        let scores = expand(lines);
        assert!(scores.contains_key("0000.txt"));
        assert_eq!(scores["0000.txt"], 1);
        assert!(scores.contains_key("0001.txt"));
        assert_eq!(scores["0001.txt"], 10);
        assert!(scores.contains_key("0002.txt"));
        assert_eq!(scores["0002.txt"], 100);
        assert!(scores.contains_key("0003.txt"));
        assert_eq!(scores["0003.txt"], 1000);
    }
}
