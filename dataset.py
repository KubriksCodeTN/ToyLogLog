#!/usr/bin/python3

import random

# Generate a dataset of unique integers
random.seed(4)
unique_integers = set(random.sample(range(1, 1000000), 10000))  # 10,000 unique integers
with open('unique_integers.txt', 'w') as f:
    for item in unique_integers:
        f.write(f"{item}\n")
