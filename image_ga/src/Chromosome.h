#pragma once

class Chromosome {
public:
	Chromosome(int _R, int _G, int _B, int _X, int _Y, int _Size) {
		R = _R;
		G = _G;
		B = _B;
		X = _X;
		Y = _Y;
		Size = _Size;
	}

	Chromosome() {}

	int R, G, B;
	int X, Y;
	int Size;
};
