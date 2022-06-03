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

            neuron['bias'] -= l_rate * neuron['delta']


def train_iteration(network, train_dataset, l_rate):
    sum_error = 0
    for row in train_dataset:
        # forward progagation
        outputs = forward_propagate(network, row[0])

        # format expected output
        expected = row[1]

        # calculate error and back propagate
        sum_error += sum([(expected[i] - outputs[i]) ** 2 for i in range(len(expected))])
        backward_propagate_error(network, expected)

        # apply gradient descent
        update_weights(network, row[0], l_rate)

    return sum_error


# Train a network for a fixed number of epochs
def train_network(network, train_dataset, l_rate, n_epoch:int):
    for epoch in range(n_epoch):
        sum_error = train_iteration(network, train_dataset, l_rate)
        print('>epoch=%d, lrate=%.3f, error=%.3f' % (epoch, l_rate, sum_error))

    return {'epoch': epoch, 'error': sum_error}


def train_network_until(network, train_dataset, l_rate:float, error_target:float):
    sum_error = 1.1
    epoch = 0
    while sum_error > error_target:
        sum_error = train_iteration(network, train_dataset, l_rate)
        print('>epoch=%d, lrate=%.3f, error=%.3f' % (epoch, l_rate, sum_error))
        epoch += 1

    return {'epoch': epoch, 'error': sum_error}