#pragma once
#include <vector>
#include <math.h>
#include "macros.h"

namespace ann {

	struct neuron
	{
		std::vector<float> weights;
		float bias;
		float output;
		float delta;

	};


}