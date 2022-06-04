#pragma once
#include <vector>
#include "NeuronUtils.h"


namespace ann {

	float transfer_derivative(const float& output) {
		return output * (1.0f - output);
	}

	bool is_output_layer(const int& i_layer, const int & len_network) {
		return i_layer == (len_network - 1);
	}

	void backward_propagate_error( std::vector<std::vector<neuron>> & network,  const std::vector<float> & expected) {

		for (int i_layer = network.size()-1; i_layer >= 0; i_layer--) {
			
			auto& layer = network[i_layer];

			auto errors = std::vector<float>();

			if (not is_output_layer(i_layer, network.size())) {
				for (int i_layer_neuron = 0; i_layer_neuron < layer.size(); i_layer_neuron++) {
					float error = .0f;

					for (const auto& next_neuron : network[i_layer + 1]) {
						error += next_neuron.weights[i_layer_neuron] * next_neuron.delta;
					}
					errors.push_back(error);

				}
			}
			else {
				for (int i_layer_neuron = 0; i_layer_neuron < layer.size(); i_layer_neuron++) {
					const auto& m_neuron = layer[i_layer_neuron];
					errors.push_back(m_neuron.output - expected[i_layer_neuron]);
				}
			}

			for (int i_layer_neuron = 0; i_layer_neuron < layer.size(); i_layer_neuron++) {
				auto& m_neuron = layer[i_layer_neuron];
				m_neuron.delta = errors[i_layer_neuron] * transfer_derivative(m_neuron.output);
			}

		}

	}

}