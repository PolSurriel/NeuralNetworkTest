#pragma once
#include <vector>
#include "NeuronUtils.h"
#include <iostream>
#include "macros.h"


namespace ann {

    std::vector<std::vector<neuron>> initialize_network(int n_inputs, const std::vector<int>& n_hidden_layers, const int& n_outputs) {

        auto network = std::vector<std::vector<neuron>>();

        for (const int& n_hidden : n_hidden_layers) {
            auto hidden_layer = std::vector<neuron>();
            for (int i = 0; i < n_hidden; i++) {
                hidden_layer.push_back(random_neuron(n_inputs));
            }
    
            network.push_back(hidden_layer);
            n_inputs = n_hidden;
        }

        auto output_layer = std::vector<neuron>();
        repeat(n_outputs) output_layer.push_back(random_neuron(n_hidden_layers.back()));

        network.push_back(output_layer);

        return network;

    }

    void show_network(const std::vector<std::vector<neuron>> & network) {
        std::cout << "Network with " <<network.size() << " layers (exluding input)" << std::endl;
        std::cout << "Where:" << std::endl;

        std::cout << " --> "<< network[0][0].weights.size() << " inputs are expected from the first hidden layer." << std::endl;

        repeat(network.size()) {
            if (i == network.size() - 1) {
                std::cout << " --> In output layer there are "<< network[i].size() << " neurons" << std::endl;
            }
            else {
                std::cout << " --> In hidden layer"<< i <<" there are "<< network[i].size() << " neurons" << std::endl;
            }
        }


        std::cout << std::endl << std::endl;
    
    }


}
