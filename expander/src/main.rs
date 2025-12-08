pub(crate) mod expander_base;
use bpaf::Bpaf;
use std::{fs, io, path::PathBuf, vec};

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

fn read_dirs(basename: &PathBuf, dirpath: &PathBuf) -> io::Result<Vec<PathBuf>> {
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
                ret.push(path.strip_prefix(basename).unwrap().into());
            }
        }
    }
    return Ok(ret);
}

fn main() -> io::Result<()> {
    let opts = opts().run();
    println!("{:#?}", &opts);
    let mut files = vec![];
    for dirname in opts.includes {
        let mut internal = read_dirs(&dirname, &dirname)?;
        files.append(&mut internal);
    }
    println!("{:#?}", files);
    Ok(())
}
