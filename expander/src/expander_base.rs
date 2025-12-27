use regex::Regex;
use std::collections::{HashMap, HashSet};

fn create_regex(r: &str) -> Regex {
    return Regex::new(r).unwrap();
}

enum BlockAction {
    NONE,
    YetElse(i32),
    DoneElse(i32),
}

pub fn expand(
    basename: String,
    ignore_blocks: HashSet<String>,
    files: HashMap<String, Vec<String>>,
) -> Vec<String> {
    let mut stack: Vec<(String, usize)> = vec![];
    let mut blocks: Vec<(String, BlockAction)> = vec![];
    let mut count = 0;
    let mut output = vec![];
    let mut opened = HashSet::new();
    let pragma_one = create_regex("^\\s*#\\s*pragma\\s+once");
    let include_a = create_regex("^\\s*#\\s*include\\s*\"([^\"]*)\"");
    let include_b = create_regex("^\\s*#\\s*include\\s*<([^>]*)>");
    let start_ifdef = create_regex("^\\s*#\\s*ifdef\\s+([[:word:]]+)");
    let start_ifndef = create_regex("^\\s*#\\s*ifndef\\s+([[:word:]]+)");
    let start_if = create_regex("^\\s*#\\s*if\\s+");
    let else_ifdef = create_regex("^\\s*#\\s*else");
    let end_ifdef = create_regex("^\\s*#\\s*endif");
    stack.push((basename, 0));
    while stack.len() != 0 {
        let (filename, index) = stack.pop().unwrap();
        let contents = files.get(&filename).unwrap();
        stack.push((filename, index + 1));
        if index == contents.len() {
            stack.pop();
            continue;
        }
        let line = &contents[index];
        if pragma_one.is_match(&line) {
            continue;
        } else if let Some(caps) = include_a.captures(&line) {
            let filename = caps.get(1).unwrap().as_str().to_string();
            if opened.contains(&filename) {
                continue;
            } else {
                opened.insert(filename.clone());
                if files.contains_key(&filename) {
                    stack.push((filename, 0));
                    continue;
                }
            }
        } else if let Some(caps) = include_b.captures(&line) {
            let filename = caps.get(1).unwrap().as_str().to_string();
            if opened.contains(&filename) {
                continue;
            } else {
                opened.insert(filename.clone());
                if files.contains_key(&filename) {
                    stack.push((filename, 0));
                    continue;
                }
            }
        } else if let Some(caps) = start_ifdef.captures(&line) {
            let name = caps.get(1).unwrap().as_str().to_string();
            if ignore_blocks.contains(&name) {
                count += 1;
                blocks.push((name, BlockAction::YetElse(1)));
                continue;
            } else {
                blocks.push((name, BlockAction::NONE));
            }
        } else if let Some(caps) = start_ifndef.captures(&line) {
            let name = caps.get(1).unwrap().as_str().to_string();
            if ignore_blocks.contains(&name) {
                blocks.push((name, BlockAction::YetElse(0)));
                continue;
            } else {
                blocks.push((name, BlockAction::NONE));
            }
        } else if let Some(_) = start_if.captures(&line) {
            blocks.push(("".into(), BlockAction::NONE));
        } else if else_ifdef.is_match(&line) {
            let (_, a) = blocks.pop().unwrap();
            match a {
                BlockAction::NONE => {
                    blocks.push(("".to_string(), BlockAction::NONE));
                }
                BlockAction::YetElse(v) => {
                    count -= v;
                    count += 1 - v;
                    blocks.push(("".to_string(), BlockAction::DoneElse(1 - v)));
                    continue;
                }
                _ => {
                    let (filename, index) = stack.pop().unwrap();
                    panic!(
                        "given file includes invalid ifdef-else block (see {}, {})",
                        filename, index
                    );
                }
            }
        } else if end_ifdef.is_match(&line) {
            let (_, a) = blocks.pop().unwrap();
            match a {
                BlockAction::YetElse(v) => {
                    count -= v;
                    continue;
                }
                BlockAction::DoneElse(v) => {
                    count -= v;
                    continue;
                }
                _ => {}
            }
        }
        if count == 0 {
            output.push(line.clone());
        }
    }
    return output;
}

#[cfg(test)]
mod tests {
    use std::collections::{HashMap, HashSet};

    use crate::expander_base::expand;
    fn setup(strs: Vec<&str>) -> Vec<String> {
        let mut ret = vec![];
        ret.reserve(strs.len());
        for str in strs {
            ret.push(str.to_string());
        }
        return ret;
    }

    #[test]
    fn include_works_1() {
        let basename = "main.cpp".to_string();
        let ignore = HashSet::new();
        let main = setup(vec![
            "#include <bits/stdc++.h>",
            "#include \"test.hpp\"",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let sub = setup(vec![
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
        ]);
        let expected = setup(vec![
            "#include <bits/stdc++.h>",
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let files = HashMap::from([
            ("main.cpp".to_string(), main),
            ("test.hpp".to_string(), sub),
        ]);
        let output = expand(basename, ignore, files);
        assert_eq!(output, expected);
    }
    #[test]
    fn include_works_2() {
        let basename = "main.cpp".to_string();
        let ignore = HashSet::new();
        let main = setup(vec![
            "#include <bits/stdc++.h>",
            "#include <test.hpp>",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let sub = setup(vec![
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
        ]);
        let expected = setup(vec![
            "#include <bits/stdc++.h>",
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let files = HashMap::from([
            ("main.cpp".to_string(), main),
            ("test.hpp".to_string(), sub),
        ]);
        let output = expand(basename, ignore, files);
        assert_eq!(output, expected);
    }
    #[test]
    fn nested_include() {
        let basename = "main.cpp".to_string();
        let ignore = HashSet::new();
        let main = setup(vec![
            "#include <bits/stdc++.h>",
            "#include <test.hpp>",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let sub0 = setup(vec!["#include <values.hpp>"]);
        let sub1 = setup(vec![
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
        ]);
        let expected = setup(vec![
            "#include <bits/stdc++.h>",
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let files = HashMap::from([
            ("main.cpp".to_string(), main),
            ("test.hpp".to_string(), sub0),
            ("values.hpp".to_string(), sub1),
        ]);
        let output = expand(basename, ignore, files);
        assert_eq!(output, expected);
    }
    #[test]
    fn ignore_duplicate() {
        let basename = "main.cpp".to_string();
        let ignore = HashSet::new();
        let main = setup(vec![
            "#include <bits/stdc++.h>",
            "#include \"header0.hpp\"",
            "#include \"header1.hpp\"",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let header0 = setup(vec![
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
        ]);
        let header1 = setup(vec!["#include <print>", "#include <header0.hpp>"]);
        let files = HashMap::from([
            ("main.cpp".to_string(), main),
            ("header0.hpp".to_string(), header0),
            ("header1.hpp".to_string(), header1),
        ]);
        let expected = setup(vec![
            "#include <bits/stdc++.h>",
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
            "#include <print>",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let output = expand(basename, ignore, files);
        assert_eq!(output, expected);
    }

    #[test]
    fn remove_ifdef() {
        let main = setup(vec![
            "#include <print>",
            "#ifdef LOCAL_DEBUG",
            "static_assert(false);",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main)]);
        let expected = setup(vec![
            "#include <print>",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, expected);
    }
    #[test]
    fn remove_ifdef_else() {
        let main = setup(vec![
            "#include <print>",
            "#ifdef LOCAL_DEBUG",
            "static_assert(false);",
            "#else",
            "static_assert(true);",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main)]);
        let expected = setup(vec![
            "#include <print>",
            "static_assert(true);",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, expected);
    }
    #[test]
    fn ignore_ifdef() {
        let main = setup(vec![
            "#include <print>",
            "#ifdef NDEBUG",
            "#define ASSERT(...)",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main.clone())]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, main);
    }
    #[test]
    fn ignore_ifdef_else() {
        let main = setup(vec![
            "#include <print>",
            "#ifdef NDEBUG",
            "#define ASSERT(...)",
            "#else",
            "#define ASSERT(...) void(0)",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main.clone())]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, main);
    }
    #[test]
    fn remove_ifndef() {
        let main = setup(vec![
            "#include <print>",
            "#ifndef LOCAL_DEBUG",
            "static_assert(true);",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main)]);
        let expected = setup(vec![
            "#include <print>",
            "static_assert(true);",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, expected);
    }
    #[test]
    fn remove_ifndef_else() {
        let main = setup(vec![
            "#include <print>",
            "#ifndef LOCAL_DEBUG",
            "static_assert(true);",
            "#else",
            "static_assert(false);",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main)]);
        let expected = setup(vec![
            "#include <print>",
            "static_assert(true);",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, expected);
    }
    #[test]
    fn ignore_ifndef() {
        let main = setup(vec![
            "#include <print>",
            "#ifndef NDEBUG",
            "#define ASSERT(...)",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main.clone())]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, main);
    }
    #[test]
    fn ignore_ifndef_else() {
        let main = setup(vec![
            "#include <print>",
            "#ifndef NDEBUG",
            "#define ASSERT(...)",
            "#else",
            "#define ASSERT(...) void(0)",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main.clone())]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, main);
    }
    #[test]
    fn ignore_if() {
        let main = setup(vec![
            "#include <print>",
            "#if !defined(NDEBUG)",
            "#define ASSERT(...)",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main.clone())]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, main);
    }
    #[test]
    fn ignore_if_else() {
        let main = setup(vec![
            "#include <print>",
            "#if !defined(NDEBUG)",
            "#define ASSERT(...)",
            "#else",
            "#define ASSERT(...) void(0)",
            "#endif",
            "int main() {",
            "  std::println(\"Hello World\");",
            "}",
        ]);
        let ignore = HashSet::from(["LOCAL_DEBUG".into()]);
        let files = HashMap::from([("main.cpp".to_string(), main.clone())]);
        let output = expand("main.cpp".into(), ignore, files);
        assert_eq!(output, main);
    }
    #[test]
    fn remove_pramga_one() {
        let basename = "main.cpp".to_string();
        let ignore = HashSet::new();
        let main = setup(vec![
            "#include <bits/stdc++.h>",
            "#include \"test.hpp\"",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let sub = setup(vec![
            "#pragma once",
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
        ]);
        let expected = setup(vec![
            "#include <bits/stdc++.h>",
            "const int val = 10;",
            "const int val2 = 20;",
            "const int val3 = 30;",
            "int main() {",
            "  std::println(\"{}\", val);",
            "}",
        ]);
        let files = HashMap::from([
            ("main.cpp".to_string(), main),
            ("test.hpp".to_string(), sub),
        ]);
        let output = expand(basename, ignore, files);
        assert_eq!(output, expected);
    }
}
