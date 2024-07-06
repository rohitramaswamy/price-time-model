import matplotlib.pyplot as plt


def read_numbers_from_file(filename):
    numbers = []
    with open(filename, "r") as file:
        for line in file:
            try:
                number = float(line.strip())
                numbers.append(number)
            except ValueError:
                print(f"Skipping invalid value: {line.strip()}")
    return numbers


def plot_numbers(numbers):
    x_values = range(len(numbers))
    plt.plot(x_values, numbers, linestyle="-", color="b", label="Data Line")
    plt.xlabel("Index")
    plt.ylabel("Value")
    plt.title("Line Plot from File Data")
    plt.grid(True)
    plt.legend()
    plt.savefig("plot.png")
    plt.close()


if __name__ == "__main__":
    filename = "numbers.txt"
    numbers = read_numbers_from_file(filename)
    if numbers:
        plot_numbers(numbers)
    else:
        print("No valid numbers to plot.")
