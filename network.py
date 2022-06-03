from random import random

# Initialize a network
"""
n_inputs: number of inputs
n_hidden: n of hidden layers (list of numbers, each numer is the number of neurons of each hidden layer)
n_outputs: number of neurons for output

EXAMPLE:
IL = input layer
HL1 = hidden layer 1
HL2 = hidden layer 2
OL = output layer

With parameters 4, [3, 3], 2

.IL    HL1    HL2    OL
.
.[]
.      []      []
.[]                   []
.      []      []
.[]                   []
.      []      []
.[]

network[0] has len 4 (connections from IL to HL1). Every item has a list of 4 weights
network[1] has len 3 (connections from HL1 to HL2). Every item has a list of 3 weights
network[2] has len 3 (connections from HL2 to OL). Every item has a list of 2 weights

"""

def initialize_network(n_inputs, n_hidden_layers, n_outputs):

    network = list()

    for n_hidden in n_hidden_layers:
        hidden_layer = [{'weights':[random() for i in range(n_inputs + 1)]} for i in range(n_hidden)]
        network.append(hidden_layer)
        n_inputs = n_hidden

    output_layer = [{'weights':[random() for i in range(n_hidden_layers[-1] + 1)]} for i in range(n_outputs)]
    network.append(output_layer)

    return network


def show_network(network):
    print(f"Network with {len(network)} layers (expluding input)")
    print("Where:")
    print(f"{len(network[0][0]['weights']) - 1} inputs are expected from the first hidden layer")
    for i in range(len(network)):
        if (i == len(network) - 1):
            print(f"in output layer there are {len(network[i])} neurons ")
        else:
            print(f"in hidden layer{i + 1} there are {len(network[i])} neurons ")
