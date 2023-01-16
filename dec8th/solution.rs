use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead, BufReader, Lines};
use std::path::Path;

struct Forest {
    pub line_len: usize,
    pub matrix: Vec<Vec<u8>>,
}

impl Forest {
    fn row(&self, y: usize) -> Vec<u8> {
        self.matrix[y].clone()
    }

    fn column(&self, x: usize) -> Vec<u8> {
        let mut vertical = Vec::with_capacity(self.line_len);

        for i in 0..self.line_len {
            vertical.push(self.matrix[i][x]);
        }

        vertical
    }

    fn new(matrix: Vec<Vec<u8>>) -> Self {
        assert_eq!(matrix.len(), matrix[0].len());

        Self {
            line_len: matrix.len(),
            matrix: matrix,
        }
    }

    fn get_visible_trees<F>(
        &self,
        line: &[u8],
        map: &mut HashMap<(usize, usize), i32>,
        coord_gen: F,
    ) where
        F: Fn(usize) -> (usize, usize),
    {
        let len = line.len();
        let (mut i, mut j) = (0, len - 1);

        let (mut biggest_l, mut biggest_r) = (-1, -1);

        loop {
            let l: i32 = line[i] as i32 - 48;
            let r: i32 = line[j] as i32 - 48;

            if l > biggest_l {
                biggest_l = l;

                let coord = coord_gen(i);
                map.insert(coord, map.get(&coord).unwrap_or(&0) + 1);
            }

            if r > biggest_r {
                biggest_r = r;

                let coord = coord_gen(j);
                map.insert(coord, map.get(&coord).unwrap_or(&0) + 1);
            }

            if j == 0 {
                break;
            }

            i += 1;
            j -= 1;
        }
    }

    fn scenic_score_for_plane(plane: Vec<u8>, idx: usize) -> i32 {
        let left = plane[..idx].iter().rev();
        let right = &plane[idx + 1..];

        let value_ceil =
            <u8 as Into<i32>>::into(*plane.get(idx).expect("no value at idx")) - 48;

        let mut dist_l = 0;

        for i in left {
            let as_i32: i32 = (i - 48).into();
            dist_l += 1;

            if as_i32 >= value_ceil {
                break;
            }
        }

        if dist_l == 0 {
            return 0; // because, if this is zero, the product will be zeroed out.
        }

        let mut dist_r = 0;
        for i in right {
            let as_i32: i32 = (i - 48).into();
            dist_r += 1;

            if as_i32 >= value_ceil {
                break;
            }
        }

        dist_l * dist_r
    }

    fn scenic_score_for_point(&self, point: (usize, usize)) -> i32 {
        let (hidx, vidx) = point;
        let mut result = 1;

        let row = self.row(hidx);
        result *= Self::scenic_score_for_plane(row, vidx);

        if result == 0 {
            return 0; // because, if this is zero, the product will be zeroed out.
        }

        let column = self.column(vidx);
        result *= Self::scenic_score_for_plane(column, hidx);

        result
    }
}

pub fn main() {
    let lines: Vec<Vec<u8>> = read_lines("./input.txt")
        .expect("no input")
        .map(|line| Vec::from(line.unwrap().as_bytes()))
        .collect();

    let forest = Forest::new(lines);

    let mut map: HashMap<(usize, usize), i32> = HashMap::new();
    let mut best_score: i32 = 0;

    for i in 0..forest.line_len {
        let horizontal = forest.row(i);
        let vertical = forest.column(i);

        for j in 0..forest.line_len {
            let score = forest.scenic_score_for_point((i, j));

            if score > best_score {
                best_score = score;
            }
        }

        forest.get_visible_trees(&horizontal, &mut map, |x: usize| (i, x));
        forest.get_visible_trees(&vertical, &mut map, |x: usize| (x, i));
    }

    println!("PART ONE: {}", map.len());
    println!("PART TWO: {best_score}");
}

fn read_lines<T: AsRef<Path>>(filename: T) -> io::Result<Lines<BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(BufReader::new(file).lines())
}
