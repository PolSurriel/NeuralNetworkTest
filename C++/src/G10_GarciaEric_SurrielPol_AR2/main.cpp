#pragma once
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

//#include "NeuralNetwork.h"
//#include "Renderer.h"
//#include "Time.h"
//#include "InputManager.h"
//#include "SDLUtils.h"
//#include "StreamBuffer.h"
//#include "MINISTUtils.h"

#include "network.h"
#include "Training.h"

//#define NET_FILENAME "../net.kn"
//#define TRAINSET_FOLDER ".."
//
//
//void loadDataset() {
//	MINISTUtils::loadDataset(TRAINSET_FOLDER);
//}
//
//void loadBatch(NeuralNetwork*& net, int* & index) {
//	
//	*index = rand() % MINISTUtils::dataset.test_images.size();
//
//	net->batchActivations.push_back(std::vector<float>(net->columns[0].neurons.size()));
//	int id = 0;
//	for (auto& a : MINISTUtils::dataset.test_images[*index]) {
//		net->batchActivations[net->batchActivations.size()-1][id++] = (((float)a) * (1.f / 255.f));
//	}
//
//	int number = MINISTUtils::dataset.test_labels[*index];
//
//	net->expectedOutputs.push_back(std::vector<float>(10));
//
//	int i = 0;
//	for (auto& o : net->expectedOutputs[net->expectedOutputs.size() - 1]) {
//		if (i++ == number)
//			o = 1.f;
//		else
//			o = 0.f;
//
//
//	}
//	
//
//}
//
//void loadAI(NeuralNetwork* &net) {
//
//	std::ifstream ifs;
//	ifs.open(NET_FILENAME);
//	if (ifs.fail() || true) {
//
//		// START AI PIPELINE
//		int columnsNumber = 4;
//		int* columsNeuronNumbers = new int[columnsNumber];
//		columsNeuronNumbers[0] = 784; // image is 28x28 = 784 pixels
//		columsNeuronNumbers[1] = 16;
//		columsNeuronNumbers[2] = 16;
//		columsNeuronNumbers[3] = 10;
//
//		net = new NeuralNetwork(columnsNumber, columsNeuronNumbers);
//		net->SetRandomValues();
//		
//		std::cout << "NET CONSTRUCTED WITH RANDOM VALUES" << std::endl;
//		delete[] columsNeuronNumbers;
//	}
//	else {
//		std::vector<float> data;
//		stream_buff::recover(data, NET_FILENAME);
//
//		int dataIndex = 0;
//		int columnsCount = data[dataIndex++];
//		int* columsNeuronNumbers = new int[columnsCount];
//		
//		for (int i = 0; i < columnsCount; i++)
//		{
//			columsNeuronNumbers[i] = data[dataIndex++];
//		}
//		net = new NeuralNetwork(columnsCount, columsNeuronNumbers);
//
//		// NEURON BIAS AND PREVWEIGHTS
//		for (int i = 1; i < net->columns.size(); i++)
//		{
//			for (auto& n : net->columns[i].neurons) {
//				n.bias = data[dataIndex++];
//				for (auto& w : n.prevWeights) {
//					w = data[dataIndex++];
//				}
//			}
//		}
//
//		std::cout << "NET CONSTRUCTED FROM FILE" << std::endl;
//		delete[] columsNeuronNumbers;
//	}
//
//	
//
//
//
//}
//
//void saveAIState(NeuralNetwork* &net) {
//	//TODO
//
//	//the file will contain 
//	//[columnscount, c0 neurons count, c1 neurons count...
//	// c1n0 bias, c1n0w0, c2n0w1, c2n0w1...  
//	// c1n1 bias, c1n1w0, c1n1w1, c1n1w2...]
//
//	int floatCount = 1 + net->columns.size();
//	for (int i = 1; i < net->columns.size(); i++)
//	{
//		int weights = net->columns[i].neurons.size() * net->columns[i-1].neurons.size();
//		int bias = net->columns[i].neurons.size();
//
//		floatCount += weights + bias;
//	}
//
//	int dataIndex = 0;
//	std::vector<float> data = std::vector<float>(floatCount);
//	// NUMBER OF COLUMNS
//	data[dataIndex++] = net->columns.size();
//
//	// NUMBER OF NEURONS PER COLUMN
//	for (auto& column : net->columns) {
//		data[dataIndex++] = column.neurons.size();
//	}
//
//	// NEURON BIAS AND PREVWEIGHTS
//	for (int i = 1; i < net->columns.size(); i++)
//	{
//		for (auto& n : net->columns[i].neurons) {
//			data[dataIndex++] = n.bias;
//			for (auto& w : n.prevWeights) {
//				data[dataIndex++] = w;
//			}
//		}
//	}
//
//
//	
//
//
//
//	stream_buff::save(data, NET_FILENAME, std::ios::trunc);
//
//}
//
//void AIMain(NeuralNetwork* &net, bool* endProgram, int* displayingTrainsetIndex, bool*& canDraw, bool*& wantToDraw) {
//
//	loadDataset();
//	loadAI(net);
//
//	// TRAIN LOOP
//	while (true)
//	{
//		// APP STATE CONTROL
//		if (*wantToDraw) {
//			*canDraw = true;
//			while (*wantToDraw && !*endProgram) {}
//			*canDraw = false;
//		}
//
//		if (*endProgram)
//			break;
//
//		
//		// LOAD BATCHES
//		net->expectedOutputs.clear();
//		net->batchActivations.clear();
//		const int batches = 10;
//		for (int i = 0; i < batches; i++)
//			loadBatch(net, displayingTrainsetIndex);
//		
//
//
//		// BACKPROPAGATION
//		net->BackPropagation();
//
//
//	}
//
//	saveAIState(net);
//
//}
//
//void old_main() {
//
//	srand((unsigned)time(NULL));
//
//	
//	NeuralNetwork* net = nullptr;
//	bool* endProgram = new bool;
//	bool* wantToDraw = new bool;
//	bool* canDraw = new bool;
//	*endProgram = false;
//	int displayingTrainsetIndex = 1;
//
//	std::thread ai_thread([&] {
//		AIMain(net, endProgram, &displayingTrainsetIndex, canDraw, wantToDraw);
//	});
//
//
//
//	// VISUAL / INPUT PROGRAM
//	auto* rend = Renderer::Instance();
//
//	FPSTime::Init();
//	InputManager inputManager;
//
//	const char* fontPath = "../../res/ttf/arial.ttf";
//	SDLUtils::SetFont(fontPath);
//	while (true)
//	{
//		FPSTime::Update();
//		if (!FPSTime::HasToUpdateFrame() || net == nullptr) continue;
//
//
//		*wantToDraw = true;
//		while (!*canDraw) { }
//
//		inputManager.UpdateInput();
//
//		// IF USER EXIT APP
//		if (inputManager.closeWindow || inputManager.keysDown[(int)InputManager::Keys::ESC]) {
//			*endProgram = true;
//			break;
//		}
//
//		SDLUtils::SetColor(COLOR_BLACK);
//		rend->Clear();
//		
//		int di = displayingTrainsetIndex;
//
//		SDLUtils::DrawImage(600, 600, MINISTUtils::dataset.training_images[di], 28, 28);
//		
//		auto t = std::to_string(MINISTUtils::dataset.training_labels[di]);
//		SDLUtils::DrawText(640, 600, t.c_str(), 2.f);
//
//		SDLUtils::SetColor(COLOR_WHITE);
//		bool firstColumn = true;
//		int cIndex = 0;
//
//		const int xIncrement = 250;
//		const int yIncrement = 40;
//		const int yFirstValue = 30;
//		const int xFirstValue = 10;
//		const int neuronRadius = 10;
//		
//		int x = xFirstValue;
//		for (const auto& column : net->columns) {
//			int y = yFirstValue;
//			
//			for (const auto& n : column.neurons) {
//				SDLUtils::SetColor(COLOR_WHITE);
//				SDLUtils::DrawCircle(x, y, neuronRadius);
//
//				int32_t a = (int32_t)(n.activation * 255.f);
//				SDLUtils::SetColor(a,a,a);
//				SDLUtils::DrawFilledCircle(x, y, neuronRadius-2);
//
//
//				if (!firstColumn) {
//					int wy = yFirstValue;
//					for (int w = 0; w < n.prevWeights.size(); w++)
//					{
//						float t = n.prevWeights[w];
//						float r = t * 255.f;
//						float g = (1.f-t) * 255.f;
//						float a = abs(t) * 255.f;
//
//						SDLUtils::SetColor(r,g, 0, a);
//
//						SDLUtils::DrawLine(x - neuronRadius,y, x - xIncrement + neuronRadius, wy);
//						wy += yIncrement;
//					}
//				}
//				y += yIncrement;
//			}
//			
//			
//
//
//			firstColumn = false;
//			x += xIncrement;
//			cIndex++;
//
//
//		}
//
//		// DRAW EXPECTED OUTPUT
//		x = xIncrement * (net->columns.size()-1) + 70;
//		int y = yFirstValue;
//		SDLUtils::SetColor(COLOR_WHITE);
//		for (int i = 0; i < net->columns[net->columns.size()-1].neurons.size(); i++)
//		{
//			SDLUtils::DrawCircle(x, y, neuronRadius);
//
//			if (i == (int)MINISTUtils::dataset.training_labels[displayingTrainsetIndex]) {
//				SDLUtils::DrawFilledCircle(x, y, neuronRadius);
//			}
//			y += yIncrement;
//		}
//
//
//		rend->Render();
//
//		*wantToDraw = false;
//
//	}
//
//	// Wait for ai thread to save its current state.
//	ai_thread.join();
//
//	delete net;
//	delete endProgram;
//	delete wantToDraw;
//	delete canDraw;
//	
//	
//
//}
//



void main() {
	srand((unsigned)time(NULL));


    //# Dataset values : [0] -- > INPUTS[1] -- > EXPECTED OUTPUTS
    auto dataset = std::vector<std::vector<std::vector<float>>>{
        {{2.7810836, 2.550537003},{0.0f, 1.0}},
        {{1.465489372, 2.362125076 },{0.0, 1.0}},
        {{3.396561688, 4.400293529 },{0.0, 1.0}},
        {{1.38807019, 1.850220317  },{0.0, 1.0}},
        {{3.06407232, 3.005305973  },{0.0, 1.0}},
        {{7.627531214, 2.759262235 },{1.0, 0.0}},
        {{5.332441248, 2.088626775 },{1.0, 0.0}},
        {{6.922596716, 1.77106367  },{1.0, 0.0}},
        {{8.675418651, -0.242068655},{1.0, 0.0}},
        {{7.673756466, 3.508563011 },{1.0, 0.0}}
    };

	int n_inputs = dataset[0][0].size();
	int n_ouptuts = dataset[0][1].size();
	auto n_hidden_layers = std::vector<int>{ 2 };

    float l_step = 0.5f;
    
	auto network = ann::initialize_network(n_inputs, n_hidden_layers, n_ouptuts);

    ann::train_network(network, dataset, l_step, 0.001f);
	
    ann::show_network(network);

}