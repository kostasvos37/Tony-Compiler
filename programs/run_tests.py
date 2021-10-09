"""Run tests."""
import glob
import os
import subprocess

path_to_tests = os.path.dirname(os.path.realpath(__file__))
path_for_correct_tests = os.path.join(path_to_tests, "correct")
unexpected_errors, expected_correct = [], []
print("Running correct programs...\n")
for filename in glob.glob(os.path.join(path_for_correct_tests, '*.tony')):
    compile_to_llvm = subprocess.run(
        ['./tony', filename],
        capture_output=True)
    if (compile_to_llvm.returncode != 0):
        print("Test:", filename, "has a semantic/syntactic error...")
        continue
    create_executable = subprocess.run(
        ["clang", "out.ll", "lib.a", "-o", "out.o", "-lgc"],
        capture_output=True)
    if (create_executable.returncode != 0):
        print("Test:", filename, "has an LLVM error...")
        continue
    run_executable = subprocess.run(["./out.o"], capture_output=True)
    if run_executable.stdout.decode("utf-8") != "42":
        unexpected_errors.append(filename)
    else:
        expected_correct.append(filename)

print("Test programs that ran successfully as expected...")
for f in expected_correct:
    print("  ", os.path.basename(f))

print("\nTest programs that errored out unexpectedly...")
for f in unexpected_errors:
    print("  ", os.path.basename(f))

print("\n--------------------------------------------------------\n")

path_for_incorrect_tests = os.path.join(path_to_tests, "incorrect")
expected_errors, unexpected_correct = [], []
print("Running incorrect programs...\n")
for filename in glob.glob(os.path.join(path_for_incorrect_tests, '*.tony')):
    compile_to_llvm = subprocess.run(
        ['./tony', filename],
        capture_output=True)
    if (compile_to_llvm.returncode != 0):
        expected_errors.append(filename)
        continue
    create_executable = subprocess.run(
        ["clang", "out.ll", "lib.a", "-o", "out.o", "-lgc"],
        capture_output=True)
    if (create_executable.returncode != 0):
        expected_errors.append(filename)
        continue
    unexpected_correct.append(filename)

print("Test programs that ran successfully as expected...")
for f in expected_errors:
    print("  ", os.path.basename(f))

print("\nTest programs that unexpectedly didn't error out...")
for f in unexpected_correct:
    print("  ", os.path.basename(f))
