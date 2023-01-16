// use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead, BufReader, Lines};
use std::path::Path;
use std::convert::TryInto;
use std::fmt::{Debug,Formatter};

struct Point {
	x: i32,
	y: i32
}

impl Debug for Point {
	fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), std::fmt::Error> {
		write!(f, "({}, {})", self.x, self.y)
	}
}

struct Rope {
	head: Point,
	tail: Point,
}

impl Rope {
	fn new() -> Self {
		Self {
			head: Point { x: 0, y: 0 },
			tail: Point { x: 0, y: 0 }
		}
	}

	fn tick(&mut self, m: &Move) {
		use Direction::*;

		let step = m.step as i32;

		match m.dir {
			L => {
				self.head.x -= step
			}
			R => {
				self.head.x += step
			}
			U => {
				self.head.y += step
			}
			D => {
				self.head.y -= step
			}
		}
	}
}

#[derive(Debug)]
enum Direction {
	R,
	U,
	D,
	L
}

impl <T>From<T> for Direction
where 
	T: TryInto<char>,
	<T as TryInto<char>>::Error: Debug
{
	fn from(c: T) -> Self {
		use Direction::*;
		match c.try_into().expect("not u8") {
			'R' => R,
			'U' => U,
			'D' => D,
			'L' => L,
			_ => unreachable!()
		}
    }
}

#[derive(Debug)]
struct Move {
	dir: Direction,
	step: usize
}

impl Move {
	fn new(dir: Direction, step: usize) -> Self {
		Self {
			dir: dir,
			step: step
		}
	}
}

pub fn main() {
    let mut lines = read_lines("input2.txt").expect("no file");

	let mut rope = Rope::new();

	while let Some(Ok(ref line)) = lines.next() {
		let mut split = line.split_whitespace();

		let p1 = split.next().unwrap().bytes().nth(0).unwrap();
		let p2 = split.next().unwrap();

		let this_move = Move::new(p1.into(), usize::from_str_radix(p2, 10).expect("bad number format"));

		rope.tick(&this_move);

		// println!("{:?}", this_move);
	}

	println!("{:?}", rope.head);
}

fn read_lines<T>(filename: T) -> io::Result<Lines<BufReader<File>>>
where
	T: AsRef<Path>
{
    let file = File::open(filename)?;
    Ok(BufReader::new(file).lines())
}
