#pragma once
#include <vector>

struct Neuron {
	float bias;
	float activation;
	std::vector<float> prevWeights;
	float z;
};

struct NeuronColumn {
	std::vector<Neuron>neurons;
};


struct MathUtils {


	static float sigmoid(float n) {
		const float e = 2.71828182845904523536f;
		
		return 1.f / (1.f + pow(e, -n));
	}

	static float reLU(float n) {
		if (n < 0.f)
			return 0.f;
		return n;
	}

	static float derivate_reLU(float n) {
		return n > 0.f ? 1.f : 0.f;
	}


	static float derivate_sigmoid(float n) {
		float sigx = sigmoid(n);
		return sigx * (1.f - sigx);
	}
};

class NeuralNetwork
{
	int columnsNumber;
	int* neuronsPercolumnNumber;

public:
	void RestoreValuesToZero();
	NeuralNetwork* backpropagation_output;
	std::vector<NeuronColumn> columns;
	std::vector<std::vector<float>> expectedOutputs;
	std::vector<std::vector<float>> batchActivations;
	
	NeuralNetwork(int columnsNumber, int neuronsPerColumn[]);

	float GradientDescent(const int columnsNumber, const float neuronsPerColumn[]);
	void Process();

	float LastColumnCost(const std::vector<float>& expectedOutput);

	void SetRandomValues();
	
	void BackPropagation();
	
};

