use std::path::Path;
use std::io::{self, Lines, BufReader, BufRead};
use std::fs::File;
use std::collections::HashSet;

pub fn main() {
	let lines = read_lines("./input.txt").expect("no input");

	let mut sum: u32 = 0;

	for line in lines {
		let line = line.unwrap();

		let line: (&str, &str) = line.split_at(line.len() / 2);

		for occurrence in common_occurrences(line.0, line.1) {
			let weight = weight_of_occurrence(occurrence);
			dbg!(weight);
			sum += weight;
		}
	}

	dbg!(sum);
}

pub fn weight_of_occurrence(c: char) -> u32 {
	let c = c as u32;
	if c > 0x60 && c < 0x7B {
		c - 0x60
	} else {
		c - 0x26
	}
} 

pub fn common_occurrences<'a>(bottom: &'a str, top: &'a str) -> HashSet<char> {
	let size = bottom.len();

	let mut bottom_characters: HashSet<char> = HashSet::with_capacity(size);
	let mut top_characters: HashSet<char> = HashSet::with_capacity(size);
	let mut result: HashSet<char> = HashSet::with_capacity(size * 2);

	for chars in bottom.chars().zip(top.chars()) {
		if bottom_characters.contains(&chars.1) {
			result.insert(chars.1);
		} else {
			top_characters.insert(chars.1);
		}

		if top_characters.contains(&chars.0) {
			result.insert(chars.0);
		} else {
			bottom_characters.insert(chars.0);
		}
	}

	result
}

fn read_lines<T: AsRef<Path>>(filename: T) -> io::Result<Lines<BufReader<File>>> {
    let file = File::open(filename)?;
    Ok(BufReader::new(file).lines())
}