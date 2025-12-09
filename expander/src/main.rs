use bpaf::Bpaf;
use std::{
    collections::{HashMap, HashSet},
    fs::{self, File},
    io::{self, Read, Write},
    path::PathBuf,
    vec,
};

use crate::expander_base::expand;
mod expander_base;

#[derive(Debug, Clone, Bpaf)]
#[bpaf(options, version)]
struct Opts {
    #[bpaf(short, long, short('o'), long("output"))]
    output: PathBuf,

    #[bpaf(short('i'), long("include"))]
    includes: Vec<PathBuf>,

    #[bpaf(long("ignore"))]
    ignores: Vec<String>,

    #[bpaf(positional("filename"))]
    basename: PathBuf,
}

fn read_dirs<'a>(
    basename: &'a PathBuf,
    dirpath: &PathBuf,
) -> io::Result<Vec<(&'a PathBuf, PathBuf)>> {
    let mut ret = vec![];
    for entry in fs::read_dir(dirpath)? {
        let dir = entry?;
        let meta = dir.metadata()?;
        let path = dir.path();
        if meta.is_dir() {
            let mut internal = read_dirs(&basename, &path)?;
            ret.append(&mut internal);
        } else if let Some(ext) = path.extension() {
            if ext == "hpp" || ext == "h" {
                ret.push((basename, path));
            }
        }
    }
    return Ok(ret);
}

fn read_file(path: &PathBuf) -> Vec<String> {
    let mut f = File::open(path).unwrap();
    let mut contents = String::new();
    f.read_to_string(&mut contents).unwrap();
    return contents.lines().map(|x| x.to_string()).collect();
}

fn main() -> io::Result<()> {
    let opts = opts().run();
    let mut filenames = vec![];
    for dirname in &opts.includes {
        let mut internal = read_dirs(&dirname, &dirname)?;
        filenames.append(&mut internal);
    }
    let mut files = HashMap::new();
    let basefile = read_file(&opts.basename);
    let basename = opts.basename.to_str().unwrap().to_string();
    files.insert(basename.clone(), basefile);
    for (root, name) in filenames {
        let f = read_file(&name);
        files.insert(
            name.strip_prefix(root)
                .unwrap()
                .to_str()
                .unwrap()
                .to_string(),
            f,
        );
    }
    let output = expand(basename, HashSet::from_iter(opts.ignores), files);
    let mut f = File::create(opts.output)?;
    for line in output {
        writeln!(f, "{}", line)?;
    }
    return Ok(());
}
