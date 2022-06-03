from math import exp

# Calculate neuron activation for an input
def activate(weights, inputs):
	activation = weights[-1]
	for i in range(len(weights)-1):
		activation += weights[i] * inputs[i]
	return activation


# Transfer neuron activation
def transfer(activation):
	return 1.0 / (1.0 + exp(-activation))