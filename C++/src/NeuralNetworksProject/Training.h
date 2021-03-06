#pragma once
#include <vector>
#include "NeuronUtils.h"
#include "macros.h"
#include "forward_propagation.h"
#include "backpropagation.h"
#include <iostream>
#include <algorithm>

namespace ann {


	

	void update_weights(std::vector<std::vector<neuron>> & network, const std::vector<float>& row, float l_rate){
		repeat(network.size()) {
			auto inputs = row;

			if (i != 0) {
				inputs = std::vector<float>();
				for (const auto& m_neuron : network[i - 1]) {
					inputs.push_back(m_neuron.output);
				}

			}
			for (auto& m_neuron : network[i]) {
				repeatj(inputs.size()) {
					m_neuron.weights[j] -= l_rate * m_neuron.delta * inputs[j];
				}

				m_neuron.bias -= l_rate * m_neuron.delta;
			}


		}
	}

	int maxi(std::vector<float> vector) {

		float max = -99999.f;
		int max_i = -1;

		repeat(vector.size()) {
			if (vector[i] > max) {
				max_i = i;
				max = vector[i];
			}
		}

		return max_i;

	}

	float train_iteration(std::vector<std::vector<neuron>>& network, const std::vector<std::vector<std::vector<float>>>& train_dataset, float l_rate) {
		float sum_error = .0f;

		// random access:
		std::vector<int> unordered_dataset_indicies = std::vector<int>();
		repeat(train_dataset.size()) unordered_dataset_indicies.push_back(i);
		std::random_shuffle(unordered_dataset_indicies.begin(), unordered_dataset_indicies.end());


		for(const auto & rowindex : unordered_dataset_indicies) {
			sum_error = .0f;
			const auto& row = train_dataset[rowindex];

			// forward propagation
			auto output = forward_propagate(network, row[0]);

			// format expected output
			const auto& expected = row[1];

			// calculate error and back propagate
			repeat(expected.size()) sum_error += pow((expected[i] - output[i]), 2.0f);
			backward_propagate_error(network, expected);


			// apply gradient descent
			update_weights(network, row[0], l_rate);

		}

		return sum_error;
	}


	float train_network(std::vector<std::vector<neuron>>& network, const std::vector<std::vector<std::vector<float>>>& train_dataset, float l_rate, float error_target) {
		float sum_error = 0.0f;
		int epoch = 0;
		do {
			sum_error = train_iteration(network, train_dataset, l_rate);
			std::cout << "epoch=" << epoch++ << " error=";
			printf("%.4f", sum_error);
			std::cout << std::endl;


		} while (sum_error > error_target);

		return sum_error;
	}

	float train_network(std::vector<std::vector<neuron>>& network, const std::vector<std::vector<std::vector<float>>>& train_dataset, float l_rate, int n_epoch) {
		float sum_error = 0.0f;
		repeat(n_epoch) {
			float sum_error = train_iteration(network, train_dataset, l_rate);
			std::cout << "epoch=" << i << " error=";
			printf("%.4f", sum_error);
			std::cout << std::endl;

		}

		return sum_error;
	}

}