from backpropagation import backward_propagate_error
from forward_porpagation import forward_propagate


# Update network weights with error
def update_weights(network, row, l_rate):
    for i in range(len(network)):
        inputs = row[:-1]
        if i != 0:
            inputs = [neuron['output'] for neuron in network[i - 1]]
        for neuron in network[i]:
            for j in range(len(inputs)):
                neuron['weights'][j] -= l_rate * neuron['delta'] * inputs[j]
            neuron['weights'][-1] -= l_rate * neuron['delta']


def train_iteration(network, train_dataset, l_rate, n_outputs):
    sum_error = 0
    for row in train_dataset:
        outputs = forward_propagate(network, row)
        expected = [0 for i in range(n_outputs)]
        expected[row[-1]] = 1
        sum_error += sum([(expected[i] - outputs[i]) ** 2 for i in range(len(expected))])
        backward_propagate_error(network, expected)
        update_weights(network, row, l_rate)
    return sum_error


# Train a network for a fixed number of epochs
def train_network(network, train_dataset, l_rate, n_epoch:int, n_outputs:int):
    for epoch in range(n_epoch):
        sum_error = train_iteration(network, train_dataset, l_rate, n_outputs)
        print('>epoch=%d, lrate=%.3f, error=%.3f' % (epoch, l_rate, sum_error))

    return {'epoch': epoch, 'error': sum_error}


def train_until_network(network, train_dataset, l_rate:float, n_outputs:int, error_target:float):
    sum_error = 1.1
    epoch = 0
    while sum_error > error_target:
        sum_error = train_iteration(network, train_dataset, l_rate, n_outputs)
        print('>epoch=%d, lrate=%.3f, error=%.3f' % (epoch, l_rate, sum_error))
        epoch += 1

    return {'epoch': epoch, 'error': sum_error}