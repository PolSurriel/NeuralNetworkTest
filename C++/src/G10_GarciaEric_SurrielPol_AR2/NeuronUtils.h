#pragma once

#include "Neuron.h"

namespace ann {

	neuron random_neuron(int n_weights) {

		std::vector<float> weights = std::vector<float>();

		for(int i = 0; i < n_weights; i++) {
			weights.push_back(m_random_float());
		}


		return neuron{ weights, m_random_float() };
	}


	// Calculate neuron activation for an input
	float activate(const neuron& neuron, const std::vector<float>& inputs) {
		float activation = neuron.bias;

		for (int i = 0; i < neuron.weights.size(); i++)
		{
			activation += neuron.weights[i] * inputs[i];
		}

		return activation;
	}

	// Transfer neuron activation simoid
	float transfer(const float& activation) {
		return 1.0f / (1.0f + exp(-activation));
	}
}