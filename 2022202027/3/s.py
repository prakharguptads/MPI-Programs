import subprocess

cpp_file = "3.cpp"
input_file = "sample_matrix.txt"
output_file = "output.txt"

# Compile the C++ program
compile_command = f"mpic++ {cpp_file} -o your_program"
subprocess.run(compile_command, shell=True, check=True)

# Run the compiled program with input from the text file and capture the output
run_command = f"mpirun -np 12 --use-hwthread-cpus --oversubscribe ./your_program < {input_file}"
result = subprocess.run(run_command, shell=True, text=True, capture_output=True)

# Write the output to another file
with open(output_file, "w") as f:
    f.write(result.stdout)


# Print any errors, if any
if result.stderr:
    print("Error:")
    print(result.stderr)
