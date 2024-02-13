#!/usr/bin/env python3
###############################################################################
#
# Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
# more contributor license agreements.  See the NOTICE file distributed
# with this work for additional information regarding copyright ownership.
# Accellera licenses this file to you under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#  http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.  See the License for the specific language governing
# permissions and limitations under the License.
#
###############################################################################
#
# run-test.py --
# Python wrapper to run a regression test and perform golden log comparison
#
# Original Author: Philipp A. Hartmann, Apple Inc.
#
###############################################################################

"""
Run a SystemC regression test, including "golden log" file comparison,
logic adopted from tests/scripts/verify.pl
"""

import logging
import re
import subprocess
import sys
from difflib import unified_diff
from os import environ as env, uname
from pathlib import Path

if (sys.version_info.major, sys.version_info.minor) >= (3, 8):
    from shlex import join as shjoin
else:
    from collections.abc import Iterable

    # approximate shlex.join with plain join on Python < 3.8
    def shjoin(cmd: Iterable) -> str:
        return " ".join(cmd)


# setup logger
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger("run-test")

TEST_BUILD_TYPE = env["TEST_BUILD_TYPE"]
TEST_FULLNAME = env["TEST_FULLNAME"]
TEST_LEAFNAME = env["TEST_LEAFNAME"]
TEST_LOGFILE = f"{TEST_LEAFNAME}.log"
TEST_SKIPPED = env.get("TEST_SKIPPED", "false")

SKIP_RETURN_CODE = 111


def str2bool(val: str) -> bool:
    """
    Convert a string representation of truth to True or False, adapted from deprecated distutils.util.strtobool.
    True values are 'y', 'yes', 't', 'true', 'on', and '1'; false values are 'n', 'no', 'f', 'false', 'off', and '0'.
    Raises ValueError if 'val' is anything else.
    """
    val = val.lower()
    if val in {"y", "yes", "t", "true", "on", "1"}:
        return True
    if val in {"n", "no", "f", "false", "off", "0"}:
        return False
    err_msg = f"invalid truth value {val}"
    raise ValueError(err_msg)


def detect_arch() -> str:
    arch = env["SYSTEMC_ARCH"]
    if arch == "macosuniversal":
        machine = uname().machine
        return {"x86_64": "macosx64", "arm64": "macosarm64"}[machine]
    return arch


SYSTEMC_ARCH = detect_arch()


class GoldenLogComparisonError(Exception):
    "custom error when we found an issue during golden log validation"


def strip_log(log_file: Path) -> Path:
    "harmonize simulation output before golden log comparison"

    stripped_file = Path(f"{log_file}.stripped")
    logger.debug("stripping log file '%s' to '%s'", log_file, stripped_file)

    with log_file.open() as log_fd:
        log_lines = log_fd.readlines()

    # check for simulation pattern in log
    re_sim_pat = re.compile(r"^SystemC")
    re_strip_head_pat = re.compile(r"^SystemC Simulation")
    re_strip_tail_pat = re.compile(r"^End of SystemC Simulation")

    # check simulator pattern
    if not any(re_sim_pat.match(line) for line in log_lines):
        msg = "missing simulator pattern in output"
        raise GoldenLogComparisonError(msg)

    # strip header
    sim_start = 0
    sim_end = len(log_lines)
    while sim_start < sim_end and not re_strip_head_pat.match(log_lines[sim_start]):
        sim_start += 1

    # strip footer
    re_strip_tail_pat = re.compile(r"^End of SystemC Simulation")
    while sim_end > sim_start:
        sim_end -= 1
        if re_strip_tail_pat.match(log_lines[sim_end]):
            break

    if sim_start >= sim_end:
        msg = "missing simulator boundaries in output"
        raise GoldenLogComparisonError(msg)

    log_lines = log_lines[sim_start:sim_end]

    # remove file and line number information (always changes!)
    re_strip_file_line = re.compile(r"^(In file: )[^:]+:[0-9]+\r?$")
    for i in range(len(log_lines)):
        log_lines[i] = re_strip_file_line.sub(r"\1<removed by verify.pl>", log_lines[i])

    with stripped_file.open("w") as stripped_fd:
        stripped_fd.writelines(log_lines)

    return stripped_file


def strip_trace(trace_file: Path) -> Path:
    "harmonize simulation output before golden trace comparison"

    stripped_file = Path(f"{trace_file}.stripped)")
    logger.debug("stripping trace file '%s' to '%s'", trace_file, stripped_file)

    with trace_file.open() as trace_fd:
        # skip the first seven lines, which include SystemC version and date information
        for _ in range(7):
            trace_fd.readline()

        with stripped_file.open("w") as stripped_fd:
            # copy remainder of the file
            stripped_fd.writelines(trace_fd.readlines())

    return stripped_file


def compare_logfile() -> None:
    "compare test output with golden reference"

    # For trace tests work on .vcd or .awif files.
    # For all other tests work on plain .log files.
    trace_test = re.match(r".*/(vcd|a?wif)_trace/.*", TEST_FULLNAME)
    if trace_test:
        ext = trace_test.group(1)
        ext = "awif" if ext == "wif" else ext
        output_file = Path(f"{TEST_LEAFNAME}.{ext}")
        stripped_file = strip_trace(output_file)
    else:
        output_file = Path(TEST_LOGFILE)
        stripped_file = strip_log(output_file)

    # lookup golden log
    golden_dir = Path(Path(f"{TEST_FULLNAME}").name) / "golden"
    if not golden_dir.is_dir():
        msg = f"missing golden log dir {golden_dir}"
        raise GoldenLogComparisonError(msg)

    golden_file = golden_dir / f"{output_file}.{SYSTEMC_ARCH}"
    if not golden_file.exists():
        golden_file = golden_dir / output_file
    logger.debug("found golden log file '%s'", golden_file)

    with golden_file.open() as golden_fd:
        golden_lines = golden_fd.readlines()
    with stripped_file.open() as stripped_fd:
        stripped_lines = stripped_fd.readlines()

    diff_file = Path(f"{TEST_LEAFNAME}.diff")
    if diff_file.exists():
        logger.debug("removing outdated diff file '%s'", diff_file)
        diff_file.unlink()

    # compute difference between expected and actual log file
    diff = list(unified_diff(golden_lines, stripped_lines, str(golden_file), str(stripped_file)))

    if diff:
        logger.debug("storing differences in '%s'", diff_file)
        with diff_file.open("w") as diff_fd:
            diff_fd.writelines(diff)

        logger.error("differences in the output:")
        for d in diff:
            logger.error(d.rstrip())

        msg = f"output mismatch detected, check {diff_file}"
        raise GoldenLogComparisonError(msg)


def main() -> None:
    "main regression test execution wrapper"

    # get the command-line (command that should be triggered)
    command = sys.argv[1:]
    if not command:
        err_msg = "no command specified"
        raise RuntimeError(err_msg)

    # get variables from env, properly die if they are missing

    # create logfile in the current working directory
    # use mode='w' to overwrite file (default will append logs for each run)
    dbg_log = logging.FileHandler(f"{TEST_LEAFNAME}.run-test.log", mode="w")
    dbg_log.setLevel(logging.DEBUG)
    dbg_log.setFormatter(logging.Formatter("%(asctime)15s %(name)s - %(levelname)s - %(message)s"))
    logger.addHandler(dbg_log)

    if str2bool(TEST_SKIPPED):
        logger.info("skipping '%s': <%s>", TEST_FULLNAME, shjoin(command))
        sys.exit(SKIP_RETURN_CODE)

    logger.info("starting '%s': <%s> logging to <%s> pwd <%s>", TEST_FULLNAME, shjoin(command), dbg_log, Path.cwd())

    # check for sanitizer suppressions
    if TEST_BUILD_TYPE.lower().endswith("san"):
        sanitizer_suppressions = Path(Path(f"{TEST_FULLNAME}").name) / f"{TEST_BUILD_TYPE.lower()}_suppressions.txt"
        options_var = f"{TEST_BUILD_TYPE.upper()}_OPTIONS"
        if sanitizer_suppressions.exists():
            suppression_option = f"suppressions={sanitizer_suppressions}"
            env[options_var] = f"{env[options_var]}:{suppression_option}" if options_var in env else suppression_option
        if options_var in env:
            logger.info("%s = %s", options_var, env[options_var])

    try:
        with Path(TEST_LOGFILE).open("w") as logfile_fd:
            # execute test - ignore return code (check is done per golden.log comparison)
            subprocess.run(command, stdout=logfile_fd, stderr=subprocess.STDOUT, shell=False, check=False)  # noqa:S603
            logfile_fd.write("End of SystemC Simulation\n")

        compare_logfile()
        logger.info("success")
    except subprocess.CalledProcessError as exception:
        logger.critical("failed to execute process: %s", exception)
        sys.exit(1)
    except GoldenLogComparisonError as exception:
        logger.critical("failed to validate golden log: %s", exception)
        sys.exit(1)
    except Exception:
        logger.exception("fatal error")
        sys.exit(1)


if __name__ == "__main__":
    main()
