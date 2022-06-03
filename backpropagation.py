# Calculate the derivative of an neuron output (sigmoid derivative)
def transfer_derivative(output):
    return output * (1.0 - output)


def is_output_layer(i_layer, len_network):
    return i_layer == (len_network - 1)


# Backpropagate error and store in neurons
# bias is updated in train function
def backward_propagate_error(network, expected):
    for i_layer in reversed(range(len(network))):
        layer = network[i_layer]
        errors = list()
        if not is_output_layer(i_layer, len(network)):
            for i_layer_neuron in range(len(layer)):
                error = 0.0
                for next_neuron in network[i_layer + 1]:
                    error += (next_neuron['weights'][i_layer_neuron] * next_neuron['delta'])
                errors.append(error)
        else:
            for i_layer_neuron in range(len(layer)):
                neuron = layer[i_layer_neuron]
                errors.append(neuron['output'] - expected[i_layer_neuron])

        for i_layer_neuron in range(len(layer)):
            neuron = layer[i_layer_neuron]
            neuron['delta'] = errors[i_layer_neuron] * transfer_derivative(neuron['output'])
