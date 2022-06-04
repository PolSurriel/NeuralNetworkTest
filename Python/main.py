from random import seed
from network import initialize_network
from network import show_network
from training import train_network
from training import train_network_until
from forward_porpagation import forward_propagate


# Make a prediction with a network
def predict(network, row):
    outputs = forward_propagate(network, row[0])
    return outputs.index(max(outputs))


def main():
    seed(1)

    # Dataset values: [0] --> INPUTS [1] --> EXPECTED OUTPUTS
    dataset = [
        [[2.7810836, 2.550537003], [0, 1]],
        [[1.465489372, 2.362125076], [0, 1]],
        [[3.396561688, 4.400293529], [0, 1]],
        [[1.38807019, 1.850220317], [0, 1]],
        [[3.06407232, 3.005305973], [0, 1]],
        [[7.627531214, 2.759262235], [1, 0]],
        [[5.332441248, 2.088626775], [1, 0]],
        [[6.922596716, 1.77106367], [1, 0]],
        [[8.675418651, -0.242068655], [1, 0]],
        [[7.673756466, 3.508563011], [1, 0]]
    ]

    n_inputs = len(dataset[0][0])
    n_outputs = len(dataset[0][1])
    n_hidden_layers = [3, 3, 2]

    network = initialize_network(n_inputs, n_hidden_layers, n_outputs)

    train_network_until(network, dataset, 0.5, 0.001)

    print("-----------------------")
    show_network(network)


    print("-----------------------")

    for row in dataset:
        output = predict(network, row);
        print(f"expected: {row[1].index(max(row[1]))} give: {output}")

if __name__ == '__main__':
    main()
