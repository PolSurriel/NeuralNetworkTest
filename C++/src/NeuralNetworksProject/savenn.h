#pragma once
#include "StreamBuffer.h"
#include "Neuron.h"
#include "macros.h"

#define DEFAULT_NET_FILEPATH "../net.nn"

namespace ann {

	std::vector<std::vector<neuron>> load_ann(std::string path = DEFAULT_NET_FILEPATH) {

		auto network = std::vector<std::vector<neuron>>();

		std::vector<float>data;
		stream_buff::recover(data, path);


		auto dataIterator = data.begin();

		int n_layers = static_cast<int>(*(dataIterator++));

		repeat(n_layers) {
			int n_neurons = static_cast<int>(*(dataIterator++));
			network.push_back(std::vector<neuron>());
			
			repeatj(n_neurons) {
				int n_weights = static_cast<int>(*(dataIterator++));
				auto m_neuron = neuron();

				// recover weights
				m_neuron.weights = std::vector<float>();
				m_neuron.weights.insert(m_neuron.weights.begin(), dataIterator, dataIterator + n_weights);
				dataIterator += n_weights;
				
				// recover bias
				m_neuron.bias = *(dataIterator++);

				network[i].push_back(m_neuron);
			}

		}


		return network;
	}

	void save_ann(std::vector<std::vector<neuron>> network, std::string path = DEFAULT_NET_FILEPATH) {


		std::vector<float>data = std::vector<float>();
		
		// save number of layers
		data.push_back(network.size());

		for (const auto& layer : network) {
			// save number of neurons of each layer
			data.push_back(layer.size());
			for (const auto& m_neuron : layer) {
				// save number of weights of each neuron
				data.push_back(m_neuron.weights.size());

				//save weights
				data.insert(data.end(), m_neuron.weights.begin(), m_neuron.weights.end());

				//save bias
				data.push_back(m_neuron.bias);
			}
		}

		
		stream_buff::save(data, path, std::ios::trunc);
	}
}
