#pragma once
#include <vector>
#include "NeuronUtils.h"

namespace ann {
	
	std::vector<float> forward_propagate(std::vector<std::vector<neuron>>& network, const std::vector<float>& row) {
		auto inputs = row;

		for (auto& layer : network) {
			auto new_inputs = std::vector<float>();

			for (auto& m_neuron : layer) {
				float activation = activate(m_neuron, inputs);
				m_neuron.output = transfer(activation);
				new_inputs.push_back(m_neuron.output);
			}

			inputs = new_inputs;

		}

		return inputs;

	}

}