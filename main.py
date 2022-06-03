from random import seed
from network import initialize_network
from network import show_network
from training import train_network
from training import train_until_network

def main():
    seed(1)
    dataset = [[2.7810836, 2.550537003, 0],
        [1.465489372, 2.362125076, 0],
        [3.396561688, 4.400293529, 0],
        [1.38807019, 1.850220317, 0],
        [3.06407232, 3.005305973, 0],
        [7.627531214, 2.759262235, 1],
        [5.332441248, 2.088626775, 1],
        [6.922596716, 1.77106367, 1],
        [8.675418651, -0.242068655, 1],
        [7.673756466, 3.508563011, 1]]

    n_inputs = len(dataset[0]) - 1
    n_outputs = len(set([row[-1] for row in dataset]))

    network = initialize_network(n_inputs, [2], n_outputs)

    #train_until_network(network, dataset, 0.5, n_outputs, 0.001)
    train_network(network, dataset, 0.5, 100, n_outputs)

    print("-----------------------")
    show_network(network)


if __name__ == '__main__':
    main()
