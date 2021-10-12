import yaml
from os import path
from pprint import pprint
from pathlib import Path
import os
import time
import json
import re
import mmap

from concurrent.futures import ThreadPoolExecutor, as_completed
from subprocess import run, PIPE, STDOUT
import pty

import argparse

this_dir = path.dirname(path.realpath(__file__))
test_dir = path.join(path.dirname(this_dir), "test")
default_fqbn_yaml_file = path.join(test_dir, "examples-board-fqbns.yaml")
default_examples_dir = path.join(path.dirname(this_dir), "examples")
default_jobs = os.cpu_count()

parser = argparse.ArgumentParser()
parser.add_argument("board", help="The board to compile for", type=str)
parser.add_argument("--fqbn_file", help="The YAML file containing the FQBNs for different boards", type=str, default=default_fqbn_yaml_file)
parser.add_argument("--examples_dir", help="The directory with the examples to compile", type=str, default=default_examples_dir)
parser.add_argument("--include_unlabeled_examples", help="Also compile examples without an @boards label", action="store_true")
parser.add_argument("-j", "--jobs", help=f"The number of jobs to schedule in parallel (default {default_jobs})", type=int, default=default_jobs)
args = parser.parse_args()

with open(args.fqbn_file, "r") as f:
    board_fqbns = yaml.safe_load(f)

# Find all *.ino files
examples = Path(args.examples_dir).rglob('*.ino')
# Only keep those that match the name of the folder they're in
examples = filter(lambda p: p.parts[-2] == p.stem, examples)

def get_boards(path):
    with open(path, 'rb', 0) as f:
        s = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
        match = b'@boards'
        pos = s.find(match)
        if pos == -1:
            return []
        s.seek(pos + len(match))
        bboards = s.readline()
        sboards = bboards.decode('utf-8')
        return list(map(str.lower, map(str.strip, sboards.split(','))))

def build_example(path, board, fqbn, include_unlabeled_examples):
    try:
        example_boards = get_boards(path)
        if len(example_boards) > 0 and not board.lower() in example_boards:
            return None
        elif len(example_boards) == 0 and not include_unlabeled_examples:
            return None
        name = path.stem
        cwd = path.parent
        cmd = ['arduino-cli', 'compile', 
            '--format', 'json', 
            '--build-cache-path', '/tmp/core-' + board,
            '-b', fqbn,
            '--warnings', 'all',
            name]
        # TODO: libraries path?
        start = time.time()
        result = run(cmd, cwd=cwd, stdout=PIPE, stderr=PIPE)
        end = time.time()
        out_str = result.stdout.decode('utf-8')
        err_str = result.stderr.decode('utf-8')
        output = json.loads(out_str)
        output["stderr"] = err_str
        return output, result.returncode, end - start
    except Exception as e:
        output = {"success": False,
                  "compiler_err": str(e) + '\r\n' + out_str + '\r\n',
                  "stderr": err_str}
        return output, 1, 0

def compile_core(board, fqbn):
    os.makedirs('/tmp/arduino-builder-empty-sketch', exist_ok=True)
    with open('/tmp/arduino-builder-empty-sketch/arduino-builder-empty-sketch.ino', 'w') as sketch:
        sketch.write("void setup() {} void loop() {}")
    cwd = '/tmp/arduino-builder-empty-sketch'
    cmd = ['arduino-cli', 'compile', 
           '--format', 'json', 
           '--build-cache-path', '/tmp/core-' + board,
           '-b', fqbn,
           '--warnings', 'all',
           'arduino-builder-empty-sketch']
    print("Building core ...")
    start = time.time()
    result = run(cmd, cwd=cwd, stdout=PIPE, stderr=PIPE)
    end = time.time()
    out_str = result.stdout.decode('utf-8')
    err_str = result.stderr.decode('utf-8')
    output = json.loads(out_str)
    if output["success"]:
        print(f"Done in {end - start:.3f}s")
    else:
        print(cmd)
        print(out_str)
        print(err_str)
        print(output["compiler_err"])
        raise RuntimeError("Failed to compile core")

bold = "\033[1m"
red = "\033[0;31m"
green = "\033[0;32m"
yellow = "\033[1;33m"
reset = "\033[0m"

failures = []
total_count = 0
board = args.board
fqbn = board_fqbns[board.lower()]
include_unlabeled_examples = args.include_unlabeled_examples

compile_core(board, fqbn)

with ThreadPoolExecutor(max_workers=args.jobs) as executor:
    fut2example = { executor.submit(build_example, ex, board, fqbn, include_unlabeled_examples): ex for ex in examples }
    for fut in as_completed(fut2example.keys()):
        if fut.result() is None: continue
        example, output = fut2example[fut], fut.result()
        example_rel = example.relative_to(args.examples_dir)
        success = output[0]["success"]
        sizes = ""
        if "builder_result" in output[0] and "executable_sections_size" in output[0]["builder_result"]:
            try:
                for section in output[0]["builder_result"]["executable_sections_size"]:
                    name = section["name"]
                    sz = section["size"]
                    max_sz = section.get("maxSize", section.get("max_size", 0.))
                    perc = 100. * sz / max_sz if max_sz > 0 else 0
                    sizes += f'{name}: {sz:,} / {max_sz:,} = {perc:.2f}%\n'
            except KeyError as e:
                print(e)
                sizes = str(output[0]["builder_result"]["executable_sections_size"])
        color = green if success else red
        print(f"""
{bold}{board}{reset}: {color}"{example_rel}"{reset}
----- {output[2]:.3f}s ----- 
{sizes}
{output[0]["compiler_err"]}
{output[0]["stderr"]}
{"success" if success else "fail"}
""")
        if not success:
            failures.append((board, example_rel))
        total_count += 1

if len(failures) == 0:
    print(f'\r\n-----\r\n\r\nSuccessfully compiled {total_count} examples\r\n')
else:
    print(f'\r\n-----\r\n\r\nFailed to compile {len(failures)} of {total_count} examples:\r\n')
    print('\n'.join(map(lambda x: f' - {bold}{x[0]}{reset}: {red}"{x[1]}"{reset}', failures)), '\n')

exit(len(failures))