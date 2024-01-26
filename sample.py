import random

def generate_test_case(n):
    matrix = [[0] * n for _ in range(n)]

    for i in range(n):
        for j in range(n):
            if i == j:
                matrix[i][j] = 0
            else:
                # Set some values to -1 and others to a random value between 1 and 100
                matrix[i][j] = -1 if random.random() < 0.5 else random.randint(1, 100)

    return matrix

def print_matrix_to_file(matrix, filename):
    with open(filename, 'w') as file:
        for row in matrix:
            file.write(" ".join(map(str, row)) + '\n')

# Example with n = 5 and filename = "test_matrix.txt"
n = 1000
test_case = generate_test_case(n)
filename = "test_matrix.txt"

print("Generated Test Case:")
print_matrix_to_file(test_case, filename)
print(f"Matrix saved to {filename}")
