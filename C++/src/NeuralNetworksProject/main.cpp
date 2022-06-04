#pragma once
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

#include "NeuralNetwork.h"
#include "Renderer.h"
#include "Time.h"
#include "InputManager.h"
#include "SDLUtils.h"
#include "StreamBuffer.h"
#include "MINISTUtils.h"

#include "network.h"
#include "savenn.h"
#include "Training.h"
#include "stochasticgd.h"

#define NET_FILENAME "../net.nn"

std::vector<std::vector<std::vector<float>>> stochastic_gd_batch(int batch_size, bool use_test_dataset = false) {

	auto bank_images = use_test_dataset? MINISTUtils::dataset.test_images : MINISTUtils::dataset.training_images;
	auto bank_labels = use_test_dataset? MINISTUtils::dataset.test_labels: MINISTUtils::dataset.training_labels;

	auto batch = std::vector<std::vector<std::vector<float>>>();

	repeat(batch_size) {
		const int i_input = 0;
		const int i_output = 1;
		batch.push_back(std::vector < std::vector<float>>(2, std::vector<float>()));

		// FORMAT INPUTS
		const auto& img = bank_images[ann::stochastic_gd_index];
		for (const auto& a : img) {
			float activation = (((float)a) * (1.f / 255.f));
			batch[i][i_input].push_back(activation);
		}

		// FORMAT OUTPUTS
		const auto& expected_number = bank_labels[ann::stochastic_gd_index];
		repeatj(10) batch[i][i_output].push_back(0.0f);
		batch[i][i_output][expected_number] = 1.f;

		ann::stochastic_gd_index = ++ann::stochastic_gd_index % bank_labels.size();
	}

	return batch;
}


void loadBatch(NeuralNetwork*& net, int* & index) {
	
	*index = rand() % MINISTUtils::dataset.test_images.size();

	net->batchActivations.push_back(std::vector<float>(net->columns[0].neurons.size()));
	int id = 0;
	for (auto& a : MINISTUtils::dataset.test_images[*index]) {
		float activation = (((float)a) * (1.f / 255.f));
	}

	int number = MINISTUtils::dataset.test_labels[*index];

	net->expectedOutputs.push_back(std::vector<float>(10));

	int i = 0;
	for (auto& o : net->expectedOutputs[net->expectedOutputs.size() - 1]) {
		if (i++ == number)
			o = 1.f;
		else
			o = 0.f;
	}
	

}



void test(std::vector<std::vector<ann::neuron>> network) {

	int displayingTrainsetIndex = 1;

	// VISUAL / INPUT PROGRAM
	auto* rend = Renderer::Instance();

	FPSTime::Init();
	InputManager inputManager;

	const char* fontPath = "../../res/ttf/arial.ttf";
	SDLUtils::SetFont(fontPath);

	float timeCounter = 0.f;
	const float REDRAW_EACH = 1.f;

	while (true)
	{
		FPSTime::Update();
		if (!FPSTime::HasToUpdateFrame()) continue;

		float deltaTime =  FPSTime::deltaTimeInMilliseconds * 0.001f;

		inputManager.UpdateInput();

		// IF USER EXIT APP
		if (inputManager.closeWindow || inputManager.keysDown[(int)InputManager::Keys::ESC]) {
			return;
		}

		
		timeCounter += deltaTime;
		if (timeCounter < REDRAW_EACH) {
			continue;
		}
		else {
			timeCounter = 0.f;

		}


		displayingTrainsetIndex = (displayingTrainsetIndex+1) % MINISTUtils::dataset.test_images.size();

		// FORWARD PROPAGATE
		
		std::vector<float> activations = std::vector<float>();
		// FORMAT INPUTS
		const auto& img = MINISTUtils::dataset.test_images[displayingTrainsetIndex];
		for (const auto& a : img) {
			float activation = (((float)a) * (1.f / 255.f));
			activations.push_back(activation);
		}

		auto output = ann::forward_propagate(network, activations);


		// DRAW
		SDLUtils::SetColor(COLOR_BLACK);
		rend->Clear();
		
		int di = displayingTrainsetIndex;

		//Show input image and expected result
		SDLUtils::DrawImage(600, 600, MINISTUtils::dataset.test_images[di], 28, 28);
		auto t = std::to_string(MINISTUtils::dataset.test_labels[di]);
		SDLUtils::DrawText(640, 600, t.c_str(), 2.f);

		// DRAW SETTINGS
		const int neuronRadius = 10;
		
		// DRAW INPUT
		const int initial_y = 40;
		const int offset_x = 100;
		const int offset_y = 30;
		const int initial_x = 50;
		int x = initial_x;
		int y = initial_y;


		// DRAW CONNECTIONS

		for (const auto& layer : network) {
			x += offset_x;
			y = initial_y;

			for (const auto& node : layer) {
				
				int wy = initial_y;
				y += offset_y;
				for (const auto& w : node.weights) {
					wy += offset_y;

					float t = w;
					float r = t * 255.f;
					float g = (1.f-t) * 255.f;
					float a = abs(t) * 255.f;

					SDLUtils::SetColor(r,g, 0, a);
					SDLUtils::DrawLine(x - neuronRadius,y, x - offset_x + neuronRadius, wy);
				
				}
			}
		}

		x = initial_x;
		y = initial_y;




		for (const auto& activation : activations) {
			y += offset_y;
			SDLUtils::SetColor(COLOR_WHITE);
			SDLUtils::DrawCircle(x, y, neuronRadius);

			int32_t a = (int32_t)(activation * 255.f);

			SDLUtils::SetColor(a, a, a);
			SDLUtils::DrawFilledCircle(x, y, neuronRadius - 2);
		}

		


		
		
		// DRAW HIDDEN
		int i_layer = 0;
		for (const auto& layer : network) {
			x += offset_x;
			y = initial_y;

			
			if (++i_layer == network.size())
				continue;

			for (const auto& node : layer) {
				y += offset_y;
				SDLUtils::SetColor(COLOR_WHITE);
				SDLUtils::DrawCircle(x, y, neuronRadius);

				int32_t a = (int32_t)(node.output * 255.f);

				SDLUtils::SetColor(a, a, a);
				SDLUtils::DrawFilledCircle(x, y, neuronRadius - 2);
			}
		}
		y = initial_y;


		
		// DRAW OUTPUT
		int i_out = 0;
		float maxOut = -10.f;
		int maxOutIndex = 0;
		for (float& out : output) {
			y += offset_y;
			SDLUtils::SetColor(COLOR_WHITE);
			SDLUtils::DrawCircle(x, y, neuronRadius);

			int32_t a = (int32_t)(out * 255.f);
			
			SDLUtils::SetColor(a,a,a);
			SDLUtils::DrawFilledCircle(x, y, neuronRadius-2);
		
			if (out > maxOut) {
				maxOutIndex = i_out;
				maxOut = out;
			}
		}

		t = std::to_string(maxOutIndex);
		SDLUtils::DrawText(640+40, 600, t.c_str(), 2.f);


		// draw expected output
		x += offset_x;
		y = initial_y;
		repeat(10) {
			y += offset_y;
			SDLUtils::SetColor(COLOR_WHITE);
			SDLUtils::DrawCircle(x, y, neuronRadius);

			int32_t a = (int32_t)(MINISTUtils::dataset.test_labels[di] == i ? 255.f : 0.f);

			SDLUtils::SetColor(a, a, a);
			SDLUtils::DrawFilledCircle(x, y, neuronRadius - 2);

		}

		rend->Render();

	}

	
}


void main() {
		
	srand((unsigned)time(NULL));
	MINISTUtils::loadDataset();

	std::cout << MINISTUtils::dataset.training_images.size() << std::endl;

	int selected_option = -1;

	std::vector<std::vector<ann::neuron>> network;
	std::vector<std::vector<std::vector<float>>> dataset = stochastic_gd_batch(1);

	int n_inputs = dataset[0][0].size();
	int n_outputs = dataset[0][1].size();

	float l_rate = 0.01f;


	while (selected_option != 0) {
		std::cout << "Options: (0) close, (1)load network, (2)generate network, (3)train, (4) test, (5) save network" << std::endl;
		std::cin >> selected_option;
		
		switch (selected_option)
		{
		case 1:
			network = ann::load_ann();
			std::cout << "Neural network loaded!" << std::endl;
			ann::show_network(network);
			break;
		case 2:
			network = ann::initialize_network(n_inputs, {16, 16}, n_outputs);
			std::cout << "Neural network generated!" << std::endl;
			ann::show_network(network);
			break;
		case 3:
		{

			while (true) {
				std::cout << "Loading batch" << std::endl;
				dataset = stochastic_gd_batch(10000);
				ann::train_network(network, dataset, l_rate, 100);
				ann::save_ann(network);
			}

			
			break;
		}
		case 4: {

			std::cout << "Starting ANN test with loaded neural network" << std::endl;
			test(network);
			break;
		}
		case 5:
			ann::save_ann(network);
			break;
		default:
			break;
		}
	}
		

}