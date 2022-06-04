#pragma once

#define repeat(times) for (int i = 0; i < times; i++)
#define repeatj(times) for (int j = 0; j < times; j++)
#define m_random_float() static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
#define m_random_double() static_cast <double> (rand()) / static_cast <double> (RAND_MAX)