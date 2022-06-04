#pragma once
#include <fstream>
#include "mnist_reader.hpp"

// https://github.com/wichtounet/mnist/blob/master/example/main.cpp

#define TRAINSET_FOLDER ".."

namespace MINISTUtils {

	mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset;

	void loadDataset(std::string filePath = TRAINSET_FOLDER) {
		std::cout << "Loading training dataset..." << std::endl;
		dataset = mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>(filePath);
		
	}

}