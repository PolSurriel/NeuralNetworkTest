from math import exp

# Calculate neuron activation for an input
def activate(weights, bias, inputs):
	activation = bias
	for i in range(len(weights)):
		activation += weights[i] * inputs[i]
	return activation


# Transfer neuron activation simoid
def transfer(activation):
	return 1.0 / (1.0 + exp(-activation))