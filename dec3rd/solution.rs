use std::path::Path;
use std::io::{self, Lines, BufReader, BufRead};
use std::fs::File;
use std::collections::{BTreeMap};

pub fn main() {
	let mut lines = read_lines("./input.txt").expect("no input");

	let mut sum: u32 = 0;

	while let (
		Some(Ok(sack1)), 
		Some(Ok(sack2)), 
		Some(Ok(sack3))
	) = (lines.next(), lines.next(), lines.next()) {
		sum += weight_of_occurrence(get_group_badge(&sack1, &sack2, &sack3));
	}

	dbg!(sum);

	// for line in lines {
	// 	let line = line.unwrap();

	// 	let line: (&str, &str) = line.split_at(line.len() / 2);

	// 	for occurrence in get_group_badge(line.0, line.1) {
	// 		let weight = weight_of_occurrence(occurrence);
	// 		dbg!(weight);
	// 		sum += weight;
	// 	}
	// }

	// dbg!(sum);
}

pub fn weight_of_occurrence(c: char) -> u32 {
	let c = c as u32;
	if c > 0x60 && c < 0x7B {
		c - 0x60
	} else {
		c - 0x26
	}
} 

pub fn get_group_badge<'a>(first: &String, second: &String, third: &String) -> char {
	use std::cmp::max;
	let max_len: usize = max(first.len(), max(second.len(), third.len()));

	let mut occurrences: BTreeMap<char, u8> = BTreeMap::new();

	let first = first.as_bytes();
	let second = second.as_bytes();
	let third = third.as_bytes();


	for i in 0..max_len {
		if let Some(c) = first.get(i) {
			let c = *c as char;
			let count = *occurrences.get(&c).unwrap_or(&0) | 0b001;

			if count == 0b111 {
				return c;
			}

			occurrences.insert(c, count);
		} 
		if let Some(c) = second.get(i) {
			let c = *c as char;
			let count = *occurrences.get(&c).unwrap_or(&0) | 0b010;

			if count == 0b111 {
				return c;
			}

			occurrences.insert(c, count);
		} 
		if let Some(c) = third.get(i) {
			let c = *c as char;
			let count = *occurrences.get(&c).unwrap_or(&0) | 0b100;

			if count == 0b111 {
				return c;
			}

			occurrences.insert(c, count);
		}
	}

	unreachable!()
}

fn read_lines<T: AsRef<Path>>(filename: T) -> io::Result<Lines<BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(BufReader::new(file).lines())
}