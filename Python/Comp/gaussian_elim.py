#!/usr/bin/env python3
import time
import numpy as np

def main():
    M = np.random.randint(10, size=(4,5)).astype(float)
    M_shape = M.shape

    print("original:\n", M)
    for row in range(0, M_shape[0]):
        # Sort the array by row
        front = M[row::]
        if M[0][row] == 0:
            front = move_rows(row, front)

        if front[0][row] != 1:
            front[0] = front[0]/ front[0][row]

        front = normalize(row, front)
        M[row::] = front

    for row in range(0, M_shape[0]):
        """make it count from last row"""
        row = M_shape[0] - row
        M[:row:] = normalize(row-1, M[:row:], True)

    print("solved:\n", M)


def move_rows(target, matrix):
    """when target position is zero switch rows until it is not zero"""
    new = np.ones(matrix.shape)
    top = 0
    bottom = -1

    for row in matrix:
        if row[target] != 0:
            # adds the rows from the top
            new[top] = row
            top += 1
        else:
            # adds the rows from the bottom
            new[bottom] = row
            bottom -= 1

    return new


def normalize(target, matrix, reverse=False):
    """Will set all other rows to 0 in the target column"""
    if not reverse:
        data = enumerate(matrix)
        condition = 0
    else:
        data = reversed(list(enumerate(matrix)))
        condition = matrix.shape[0] - 1

    for i, row in data:
        if i == condition:
            continue

        if row[target] != 0:
            matrix[i] = row - row[target] * matrix[condition]

    return matrix


if __name__ == "__main__":
    start = time.perf_counter()
    main()
    print("time taken: ", time.perf_counter() - start, "s")
