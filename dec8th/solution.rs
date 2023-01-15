use std::collections::HashMap;
use std::convert::TryInto;
use std::fs::File;
use std::io::{self, BufRead, BufReader, Lines};
use std::path::Path;

pub fn main() {
    let lines: Vec<Vec<u8>> = read_lines("./input.txt")
        .expect("no input")
        .map(|line| Vec::from(line.unwrap().as_bytes()))
        .collect();

    let mut map: HashMap<(usize, usize), i32> = HashMap::with_capacity(99);

    let len: usize = lines[0].len();

    let mut bonus = 0;

    for i in 0..len {
        let mut horizontal = Vec::with_capacity(99);
        let mut vertical = Vec::with_capacity(99);

        for j in 0..len {
            horizontal.push(lines[i][j]);
            vertical.push(lines[j][i]);
        }

        tree_line(&horizontal, &mut map, |x: usize| (i, x));
        tree_line(&vertical, &mut map, |x: usize| (x, i));
    }

    let mut plotted = vec![vec![-0xff; len]; len];

    for (key, value) in &map {
        plotted[key.0][key.1] = *value;
    }

    for row in plotted {
        for c in row {
            print!(
                "{}",
                if c == -0xff {
                    '_'
                } else {
                    char::from_u32((c + 48).try_into().unwrap()).unwrap()
                }
            );
        }
        println!();
    }

    println!("\n$ {}", map.len())
}

fn tree_line<F>(line: &[u8], map: &mut HashMap<(usize, usize), i32>, coord_gen: F) -> ()
where
    F: Fn(usize) -> (usize, usize),
{
    let len = line.len();
    let (mut i, mut j) = (0, len - 1);

    let (mut biggest_l, mut biggest_r) = (-1, -1);

    let mut result = 0;

    // let zed: usize = row;

    loop {
        let l: i32 = line[i] as i32 - 48;
        let r: i32 = line[j] as i32 - 48;

        // println!("({l}, {r})");

        if l > biggest_l {
            result += 1;
            biggest_l = l;

            let coord = coord_gen(i);
            map.insert(coord, map.get(&coord).unwrap_or(&0) + 1);

            println!("## line[{i}] (aka. {l}) adds +1.\n## DEBUG:\n##\t- biggest_l = {biggest_l},\n##\t- coord = {:?},\n##\t- result = {result}\n", coord);
        }

        if r > biggest_r {
            result += 1;
            biggest_r = r;

            let coord = coord_gen(j);
            map.insert(coord, map.get(&coord).unwrap_or(&0) + 1);

            println!("## line[{j}] (aka. {r}) adds +1.\n## DEBUG:\n##\t- biggest_r = {biggest_r},\n##\t- coord = {:?},\n##\t- result = {result}\n", coord);
        }

        if j == 0 {
            break;
        }
		
        i += 1;
        j -= 1;
    }
}

fn read_lines<T: AsRef<Path>>(filename: T) -> io::Result<Lines<BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(BufReader::new(file).lines())
}
