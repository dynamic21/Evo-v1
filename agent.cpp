#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define boardSize 3

using namespace std;

double randDouble() {
	return ((double)rand() / RAND_MAX) * 2 - 1;
}

class ticTacToeState {
public:
	int board[boardSize][boardSize];

	void initialize() {
		int i, j;
		for (i = 0; i < boardSize; i++) {
			for (j = 0; j < boardSize; j++) {
				board[i][j] = 0;
			}
		}

	}
};

class agent {
public:
	int score;
	int structureLengthMinusOne;
	vector<int> structure;
	vector<vector<double>> brain;
	vector<vector<double>> brainActivity;
	vector<vector<double>> bias;
	vector<vector<vector<double>>> weights;

	void initialize(vector<int> blueprint) {
		int i, j, l;
		for (i = 0; i < blueprint.size(); i++) {
			structure.push_back(blueprint[i]);
			brain.push_back({});
			brainActivity.push_back({});
			for (j = 0; j < blueprint[i]; j++) {
				brain[i].push_back(0);
				brainActivity[i].push_back(true);
			}
		}
		structureLengthMinusOne = structure.size() - 1;
		for (i = 0; i < structureLengthMinusOne; i++) {
			bias.push_back({});
			for (j = 0; j < structure[i + 1]; j++) {
				bias[i].push_back(randDouble());
			}
			weights.push_back({});
			for (j = 0; j < structure[i]; j++) {
				weights[i].push_back({});
				for (l = 0; l < structure[i + 1]; l++) {
					weights[i][j].push_back(randDouble());
				}
			}
		}
	}
	
	void evaluate(ticTacToeState state) {
		int i, j, l;
		double sum;
		double presum;
		bool isActive;
		for (i = 0; i < boardSize; i++) {
			for (j = 0; j < boardSize; j++) {
				brain[0][j + boardSize * i] = state.board[i][j];
			}
		}
		for (i = 0; i < structureLengthMinusOne; i++) {
			for (j = 0; j < structure[i + 1]; j++) {
				isActive = false;
				sum = 0;
				for (l = 0; l < structure[i]; l++) {
					if (brainActivity[i][l]) {
						presum = weights[i][l][j] * brain[i][l];
						if (presum >= 0) {
							sum += presum;
							isActive = true;
						}
					}
				}
				if (isActive) {
					brain[i + 1][j] = sum + bias[i][j];
				}
				brainActivity[i + 1][j] = isActive;
			}
		}
	}

	void info() {
		cout << "structureLengthMinusOne = " << structureLengthMinusOne << endl;
		int i, j, l;
		cout << endl << "structure = " << endl;
		for (i = 0; i < structure.size(); i++) {
			cout << " " << structure[i];
		}
		cout << endl;
		cout << endl << "bias = " << endl;
		for (i = 0; i < bias.size(); i++) {
			for (j = 0; j < bias[i].size(); j++) {
				cout << " " << bias[i][j];
			}
			cout << endl;
		}
		cout << endl << "weights = " << endl;
		for (i = 0; i < weights.size(); i++) {
			for (j = 0; j < weights[i].size(); j++) {
				for (l = 0; l < weights[i][j].size(); l++) {
					cout << " " << weights[i][j][l];
				}
				cout << endl;
			}
			cout << endl;
		}
		cout << "brain = " << endl;
		for (i = 0; i < brain.size(); i++) {
			for (j = 0; j < brain[i].size(); j++) {
				cout << " " << brain[i][j];
			}
			cout << endl;
		}
		cout << endl << "brainActivity = " << endl;
		for (i = 0; i < brainActivity.size(); i++) {
			for (j = 0; j < brainActivity[i].size(); j++) {
				cout << " " << brainActivity[i][j];
			}
			cout << endl;
		}
	}
};

int main()
{
	srand(time(NULL));
	randDouble();

	agent bill;
	bill.initialize({ 9, 9, 9});
	ticTacToeState game;
	game.initialize();
	//clock_t start = clock();
	//for (int i = 0; i < 1000000; i++) {
	bill.evaluate(game);
	//}
	//cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
	bill.info();
}