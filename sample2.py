import random

def generate_matrix(n):
    matrix = [[random.randint(0, 1) for _ in range(n)] for _ in range(n)]
    return matrix

def write_matrix_to_file(matrix, filename):
    with open(filename, 'w') as file:
        for row in matrix:
            file.write(' '.join(map(str, row)) + '\n')

# Example: Generating a 5x5 matrix and writing it to a file
n = 1000
sample_matrix = generate_matrix(n)
output_filename = 'sample_matrix.txt'

print(f"Generated {n}x{n} matrix:")

write_matrix_to_file(sample_matrix, output_filename)
print(f"Matrix written to {output_filename}")
