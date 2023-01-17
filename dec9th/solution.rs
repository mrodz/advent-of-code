use std::collections::HashSet;
use std::convert::TryInto;
use std::fmt::{Debug, Formatter};
use std::fs::File;
use std::io::{self, BufRead, BufReader, Lines};
use std::path::Path;

#[derive(PartialEq, Clone, Copy, Eq, Hash)]
struct Point {
    x: i32,
    y: i32,
}

impl Debug for Point {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), std::fmt::Error> {
        write!(f, "({}, {})", self.x, self.y)
    }
}

impl Point {
    fn new() -> Self {
        Self { x: 0, y: 0 }
    }

    fn move_once(&mut self, dir: &Direction) {
        use Direction::*;
        match dir {
            L => self.x -= 1,
            R => self.x += 1,
            U => self.y += 1,
            D => self.y -= 1,
        }
    }
}

struct Rope {
    head: Point,
    tail: Point,
    prev_head: Point,
    visited_by_tail: HashSet<Point>,
}

impl Rope {
    fn new() -> Self {
        let mut visited_by_tail = HashSet::new();
        visited_by_tail.insert(Point::new());

        Self {
            head: Point::new(),
            tail: Point::new(),
            prev_head: Point::new(),
            visited_by_tail: visited_by_tail,
        }
    }

    fn is_touching(&self) -> bool {
        let is_overlap = self.head == self.tail; // head & tail are on top of each other.

        let same_x = self.head.x == self.tail.x; // head & tail share X axis
        let same_y = self.head.y == self.tail.y; // head & tail share Y axis

        let is_touching_v = (self.head.y - self.tail.y).abs() == 1; // head & tail are one unit apart on the Y axis
        let is_touching_h = (self.head.x - self.tail.x).abs() == 1; // head & tail are one unit apart on the X axis

        // if head & tail are one unit apart on BOTH axiis, they must be diagonal.
        let is_diagonal = is_touching_v && is_touching_h;

        // if any of the conditions are true, the ends are touching.
        is_overlap || is_diagonal || (is_touching_v && same_x) || (is_touching_h && same_y)
    }

    fn r#move(&mut self, m: &Move) {
        for _ in 0..m.step {
            self.tick(&m.dir);
        }
    }

    fn tick(&mut self, m: &Direction) {
        self.prev_head = self.head;

        self.head.move_once(m);

        if self.is_touching() {
            return;
        }

        // the tail has to move!
        self.tail = self.prev_head;
        self.visited_by_tail.insert(self.tail);
    }
}

#[derive(Debug)]
enum Direction {
    R,
    U,
    D,
    L,
}

impl<T> From<T> for Direction
where
    T: TryInto<char>,
    <T as TryInto<char>>::Error: Debug,
{
    fn from(c: T) -> Self {
        use Direction::*;
        match c.try_into().expect("not u8") {
            'R' => R,
            'U' => U,
            'D' => D,
            'L' => L,
            _ => unreachable!(),
        }
    }
}

#[derive(Debug)]
struct Move {
    dir: Direction,
    step: usize,
}

impl Move {
    fn new(dir: Direction, step: usize) -> Self {
        Self {
            dir: dir,
            step: step,
        }
    }
}

pub fn main() {
    let mut lines = read_lines("input.txt").expect("no file");

    let mut rope = Rope::new();

    while let Some(Ok(ref line)) = lines.next() {
        let mut split = line.split_whitespace();

        let p1 = split.next().unwrap().bytes().nth(0).unwrap();
        let p2 = split.next().unwrap();

        let this_move = Move::new(
            p1.into(),
            usize::from_str_radix(p2, 10).expect("bad number format"),
        );

        rope.r#move(&this_move);
    }

    println!("PART ONE: {}", rope.visited_by_tail.len());
}

fn read_lines<T>(filename: T) -> io::Result<Lines<BufReader<File>>>
where
    T: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(BufReader::new(file).lines())
}
