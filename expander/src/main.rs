use bpaf::Bpaf;
use std::{
    collections::HashMap,
    fs::{self, File},
    io::{self, Read},
    path::PathBuf,
    vec,
};
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
    dbg!(&dirpath);
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
    println!("{:#?}", &opts);
    let mut filenames = vec![];
    for dirname in &opts.includes {
        let mut internal = read_dirs(&dirname, &dirname)?;
        filenames.append(&mut internal);
    }
    // let mut files = HashMap::new();
    let basefile = read_file(&opts.basename);
    println!("{:#?}", basefile);
    return Ok(());
}
