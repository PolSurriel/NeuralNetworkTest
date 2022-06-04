#include "NeuralNetwork.h"
#include <iostream>

NeuralNetwork::NeuralNetwork(int columnsNumber, int neuronsPerColumn[]) {

	this->columnsNumber = columnsNumber;
	this->neuronsPercolumnNumber = neuronsPerColumn;

	columns = std::vector<NeuronColumn>(columnsNumber);

	expectedOutputs = std::vector<std::vector<float>>(100);
	batchActivations = std::vector<std::vector<float>>(100);

	for (int i = 0; i < columnsNumber; i++)
	{
		columns[i] = NeuronColumn();
		columns[i].neurons = std::vector<Neuron>(neuronsPerColumn[i]);

		if (i != 0) {
			auto prev = columns[i - 1];
			for (Neuron& n : columns[i].neurons) {
				n.prevWeights = std::vector<float>(neuronsPerColumn[i-1]);
			}
		}

		
	}

}


void NeuralNetwork::Process()
{
	for (int i = 1; i < columnsNumber; i++)
	{
		for (Neuron & neuron : columns[i].neurons) {
			neuron.activation = 0.f;
			int prevNeuronsCount = 0;
			
			for (int w = 0; w < neuron.prevWeights.size(); w++)
			{
				neuron.activation += columns[i-1].neurons[w].activation * neuron.prevWeights[w];
			}

			neuron.z = neuron.activation;
			neuron.activation = MathUtils::reLU(neuron.activation + neuron.bias);
		}

	}
}

float NeuralNetwork::LastColumnCost(const std::vector<float> & expectedOutput)
{

	int columnIndex = columnsNumber - 1;
	float result = 0.f;

	for (int i = 0; i < neuronsPercolumnNumber[columnIndex]; i++)
	{
		result += columns[columnIndex].neurons[i].activation - expectedOutput[i];
		result = pow(result, 2.f);
	}

	return result;
}

void NeuralNetwork::SetRandomValues()
{
	for (int i = 1; i < columnsNumber; i++)
	{
		auto prev = columns[i - 1];
		for (Neuron& n : columns[i].neurons) {
			n.bias = -1.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f - (-1.f))));
			for (auto& w : n.prevWeights) {
				w = -1.f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.f - (-1.f))));
				w = MathUtils::sigmoid(w);
			}
		}


	}
}

void NeuralNetwork::RestoreValuesToZero() {
	
		for (int L = 1; L < columns.size(); L++)
		{
			for (int n = 0; n < columns[L].neurons.size(); n++) {

				columns[L].neurons[n].bias = 0.f;
				for (int w = 0; w < columns[L].neurons[n].prevWeights.size(); w++)
				{
					columns[L].neurons[n].prevWeights[w] = 0.f;
				}
			}

		}
}

void NeuralNetwork::BackPropagation()
{
	
	
	std::vector<NeuralNetwork> gradientDescents = std::vector<NeuralNetwork>(expectedOutputs.size(), *this);
	
	// BACKPROPAGATION
	for (int outputIndex = 0; outputIndex < expectedOutputs.size(); outputIndex++)
	{


		std::cout << outputIndex << std::endl;

		gradientDescents[outputIndex].RestoreValuesToZero();
		// SET INPUT ACTIVATION
		int inputIndex = 0;
		for (auto& input : batchActivations[outputIndex]) {
			columns[0].neurons[inputIndex++].activation = input;
		}
		
		// PROCESS
		Process();
		return;

		// C0: cost
		float c0 = LastColumnCost(expectedOutputs[outputIndex]);
		
		for (int L = columnsNumber-1; L > 0; L--)
		{
			for (int n = 0; n < columns[L].neurons.size(); n++) {


				auto copyNetb = *this;
				copyNetb.columns[L].neurons[n].bias = columns[L].neurons[n].bias * 0.9f;
				copyNetb.Process();
				float cb = copyNetb.LastColumnCost(expectedOutputs[outputIndex]);
				float difb = c0 - cb;
				gradientDescents[outputIndex].columns[L].neurons[n].bias = columns[L].neurons[n].bias + difb * 0.1f;

				//float bias = MathUtils::derivate_reLU(columns[L].neurons[n].z) * 2*(columns[L].neurons[n].activation - c0);
				//gradientDescents[outputIndex].columns[L].neurons[n].bias = bias;
				for (int w = 0; w < columns[L].neurons[n].prevWeights.size(); w++)
				{
					auto copyNet = *this;
					copyNet.columns[L].neurons[n].prevWeights[w] = columns[L].neurons[n].prevWeights[w] * 0.9f;
					copyNet.Process();
					float c = copyNet.LastColumnCost(expectedOutputs[outputIndex]);
					float dif =  c0 - c;
						gradientDescents[outputIndex].columns[L].neurons[n].prevWeights[w] = columns[L].neurons[n].prevWeights[w] + dif * 0.1f;


					//float weight = columns[L-1].neurons[w].activation * bias;
					//gradientDescents[outputIndex].columns[L].neurons[n].prevWeights[w] = weight;
				}

			}

		}

		// APPLY RESULT
		float AVGScalar = 1.f / (float)gradientDescents.size();
		for (auto& gd : gradientDescents) {
			for (int L = 1; L < columns.size(); L++)
			{
				for (int n = 0; n < columns[L].neurons.size(); n++) {
					
					columns[L].neurons[n].bias -= gd.columns[L].neurons[n].bias * AVGScalar;
					for (int w = 0; w < columns[L].neurons[n].prevWeights.size(); w++)
					{
						columns[L].neurons[n].prevWeights[w] -= gd.columns[L].neurons[n].prevWeights[w] * AVGScalar;
					}
				}
			}
		}



	}




	


}


//inline float firstWeightColumnCost(float prevActivation, float currentActivation, float weight, float bias, float desiredInput) {
//
//	return prevActivation * MathUtils::sigmoid(weight * prevActivation + bias) * 2 * (currentActivation - desiredInput);
//
//}
//
//inline float firstBiasColumnCost(float prevActivation, float currentActivation, float weight, float bias, float desiredInput) {
//
//	return MathUtils::sigmoid(weight * prevActivation + bias) * 2 * (currentActivation - desiredInput);
//
//}
//inline float weightColumnCost(float prevActivation, float currentActivation, float weight, float bias, float desiredInput) {
//
//	return weight * MathUtils::sigmoid(weight * prevActivation + bias) * 2 * (currentActivation - desiredInput);
//
//}





float NeuralNetwork::GradientDescent(const int columnsNumber, const float neuronsPerColumn[])
{

	const float delta = 0.001f;
	float f2 = 0.f;// ColumnCost(columnsNumber, neuronsPerColumn);

	// TODO: APPLY DELTA, maybe to the weights?
	

	float f1 = 0.f;// ColumnCost(columnsNumber, neuronsPerColumn);

	return (f1 - f2) / delta;

	//Solution[i] = Solution[i] - L * gradientDecent;
	

}

