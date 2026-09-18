#!/usr/bin/env python3

import subprocess
from pathlib import Path
import os
import json

# Files
workspace = Path(__file__).resolve().parent
bazel_bin = workspace / 'bazel-bin'
tao_exe = bazel_bin / 'benchmarks' / 'taolet'
build_bin = workspace / 'bin'
tao_project = '//benchmarks:taolet'

try:
    os.mkdir(build_bin)
except FileExistsError:
    pass

# Profiles
compilers = ['clang', 'gcc']
build_modes = ['default', 'release']
flags = ['-O2', '-O3']

profiles = [compiler + '-' + build_mode
                for build_mode in build_modes
                    for compiler in compilers]

# Table formatting
def get_row_pat(align, widths):
    return '| ' + ' | '.join(['{:' + align + str(l) + '}' for l in widths]) + ' |'

def render_table(title, table):
    header = table[0]
    widths = [0] * len(header)

    for row in table:
        for index, h in enumerate(row):
            widths[index] = max(widths[index], len(str(h)))

    rows = table[1:]
    pattern = get_row_pat('<', widths)
    header_pattern = get_row_pat('^', widths)
    header_str = header_pattern.format(*header)
    divider = '+' + '=' * (len(header_str) - 2) + '+'

    title_pat = '| {:^' + str(len(header_str) - 4) + '} |'

    print(divider)
    print(title_pat.format(title))
    print(divider)
    print(header_str)
    print(divider)
    for row in rows:
        print(pattern.format(*row))
    print(divider)
    print()

# Bazel
def bazel(*args):
    subprocess.run(['bazel', *args], capture_output=True, check=True)

for profile in profiles:
    bench_exe = build_bin / ("taolet-" + profile)
    bazel('clean', '--expunge')
    bazel('build', tao_project, '--config=' + profile)
    os.rename(tao_exe, bench_exe)

# Run benchmarks
runs = {}
for profile in profiles:
    bench_result = subprocess.run([build_bin / ("taolet-" + profile),
        "--benchmark_format=json", "--benchmark_time_unit=ns"], capture_output=True, check=True)
    output = json.loads(bench_result.stdout)
    benchmarks = output['benchmarks']
    runs[profile] = { bench['name']: bench['cpu_time'] for bench in benchmarks }

benchmark_tests = runs[next(iter(runs))].keys()

def percent_change(original, new):
    return (float(new) - float(original)) / float(original)

for build, flag in zip(build_modes, flags):
    rows = [["Benchmark", *compilers, "% change", "Winner"]]
    for benchmark in benchmark_tests:
        scores = [(compiler, runs[compiler + '-' + build][benchmark]) for compiler in compilers]
        raw_scores = ['{:.2f} ns'.format(score) for _, score in scores]
        p_change = '{:.2f}%'.format(percent_change(scores[0][1], scores[1][1]) * 100) if len(compilers) == 2 else "N/a"
        row = [benchmark, *raw_scores, p_change, min(scores, key=lambda x: x[1])[0]]
        rows.append(row)

    render_table(flag + ' BENCHMARKS', rows)
