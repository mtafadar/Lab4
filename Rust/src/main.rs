// #![feature(str_split_once)]

use std::io::*;
use std::fs::{File};
use std::env;
use std::fs::set_permissions;
use std::fs::Permissions;
use std::os::unix::fs::PermissionsExt;

pub struct TreeNode {
  left: Option<Box<TreeNode>>,
  right: Option<Box<TreeNode>>,
  op: Op
}

pub enum Op {
  Add,
  Sub,
  Mul,
  Div,
  Id(i32)
}

impl TreeNode {
  pub fn new(op: Op, left: Option<TreeNode>, right: Option<TreeNode>) -> Self {
    match op {
      Op::Id(_) => {
        TreeNode {
          op: op,
          left: None,
          right: None,
        }
      },
      _ => {
        TreeNode {
          op: op,
          left: Some(Box::new(left.unwrap())),
          right: Some(Box::new(right.unwrap())),
        }
      }
    }
  }
}

pub fn calculate(node: &Box<TreeNode>) -> f32 {
  let mut l:Option<f32> = None;
  let mut r:Option<f32> = None;

  if let Some(left) = &node.left {
    l = Some(calculate(left));
  }

  if let Some(right) = &node.right {
    r = Some(calculate(right));
  }

  let l: f32 = if let Some(val) = l{val} else {0.0};
  let r: f32 = if let Some(val) = r{val} else {0.0};

  // ToDo-1: complete this section for - / * 
  match node.op {
    Op::Add => {l + r},
     
  }
}

fn build_tree(line: &str) -> Option<TreeNode> {
  // ToDo-4: find the position of Add (+) and Sub (-) in the string
  // and store it in index 
  // Note: you should traverse the string from LEFT TO RIGHT
  match index {
    Some(p) => {
      let l = build_tree(&line[..p].trim());
      let r = build_tree(&line[p+1..].trim());
      let node = match &line[p..p+1] {
        "+" => TreeNode::new(Op::Add, l, r),
        "-" => TreeNode::new(Op::Sub, l, r),
        _ => panic!("Not matched")
      };
      return Some(node);
    }
    None => ()
  }

  // ToDo-4: find the position of Mul (*) and Div (/) in the string
  // and store it in index 
  // Note: you should traverse the string from RIGHT TO LEFT
  match index {
    Some(p) => {
      let l = build_tree(&line[..p].trim());
      let r = build_tree(&line[p+1..].trim());
      let node = match &line[p..p+1] {
        "*" => TreeNode::new(Op::Mul, l, r),
        "/" => TreeNode::new(Op::Div, l, r),
        _ => panic!("Not matched")
      };
      return Some(node);
    }
    None => ()
  }

  let value = &line.trim().parse::<i32>();
  match value {
    Ok(val) => Some(TreeNode::new(Op::Id(*val), None, None)),
    Err(_) => panic!("Unexpected input")
  }
}

fn main() -> Result<()> {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
      println!("Usage: ./calc [filename]\n");
      return Err(Error::from(ErrorKind::InvalidInput));
    }
    // ToDo-2: open a file and set its permissions to 
    // allow everybody to read and execute, but only the 
    // owner can write to it (i.e., rwxr-xr-x)
    let reader = BufReader::new(stdin());

    print!("$ ");
    stdout().flush()?;

    for line in reader.lines() {
      let result = calculate(&Box::new(build_tree(&line?).unwrap()));
      println!("{}", result);
      // ToDo-3: write the result into the file 
      print!("$ ");
      stdout().flush()?;
    }

    Ok(())
}
