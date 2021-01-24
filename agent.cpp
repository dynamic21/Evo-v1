#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#define boardSize 3

using namespace std;

double randDouble()
{
	return ((double)rand() / RAND_MAX) * 2 - 1;
}

class agent
{
public:
	int score;
	int structureLengthMinusOne;
	vector<int> structure;
	vector<vector<double>> brain;
	vector<vector<double>> brainActivity;
	vector<vector<double>> bias;
	vector<vector<vector<double>>> weights;

	void initialize(vector<int> blueprint)
	{
		int i, j, l;
		score = 0;
		for (i = 0; i < blueprint.size(); i++)
		{
			structure.push_back(blueprint[i]);
			brain.push_back({});
			brainActivity.push_back({});
			for (j = 0; j < blueprint[i]; j++)
			{
				brain[i].push_back(0);
				brainActivity[i].push_back(true);
			}
		}
		structureLengthMinusOne = structure.size() - 1;
		for (i = 0; i < structureLengthMinusOne; i++)
		{
			bias.push_back({});
			for (j = 0; j < structure[i + 1]; j++)
			{
				bias[i].push_back(0);
			}
			weights.push_back({});
			for (j = 0; j < structure[i]; j++)
			{
				weights[i].push_back({});
				for (l = 0; l < structure[i + 1]; l++)
				{
					weights[i][j].push_back(0);
				}
			}
		}
	}

	agent copy()
	{
		agent joe;
		int i, j, l;
		joe.score = score;
		for (i = 0; i < structure.size(); i++)
		{
			joe.structure.push_back(structure[i]);
			joe.brain.push_back({});
			joe.brainActivity.push_back({});
			for (j = 0; j < structure[i]; j++)
			{
				joe.brain[i].push_back(brain[i][j]);
				joe.brainActivity[i].push_back(brainActivity[i][j]);
			}
		}
		joe.structureLengthMinusOne = structure.size() - 1;
		for (i = 0; i < structureLengthMinusOne; i++)
		{
			joe.bias.push_back({});
			for (j = 0; j < structure[i + 1]; j++)
			{
				joe.bias[i].push_back(bias[i][j]);
			}
			joe.weights.push_back({});
			for (j = 0; j < structure[i]; j++)
			{
				joe.weights[i].push_back({});
				for (l = 0; l < structure[i + 1]; l++)
				{
					joe.weights[i][j].push_back(weights[i][j][l]);
				}
			}
		}
		return joe;
	}

	void mutate(double rate)
	{
		int i, j, l;
		for (i = 0; i < structureLengthMinusOne; i++)
		{
			for (j = 0; j < structure[i + 1]; j++)
			{
				bias[i][j] += randDouble() * rate;
			}
			for (j = 0; j < structure[i]; j++)
			{
				for (l = 0; l < structure[i + 1]; l++)
				{
					weights[i][j][l] += randDouble() * rate;
				}
			}
		}
	}

	vector<double> evaluate(vector<int> input)
	{
		int i, j, l;
		double sum;
		double presum;
		bool isActive;
		for (i = 0; i < input.size(); i++)
		{
			brain[0][i] = input[i];
		}
		for (i = 0; i < structureLengthMinusOne; i++)
		{
			for (j = 0; j < structure[i + 1]; j++)
			{
				isActive = false;
				sum = 0;
				for (l = 0; l < structure[i]; l++)
				{
					if (brainActivity[i][l])
					{
						presum = weights[i][l][j] * brain[i][l];
						if (presum >= 0)
						{
							sum += presum;
							isActive = true;
						}
					}
				}
				if (isActive)
				{
					brain[i + 1][j] = sum + bias[i][j];
				}
				brainActivity[i + 1][j] = isActive;
			}
		}
		vector<double> output;
		for (i = 0; i < structure[structureLengthMinusOne]; i++)
		{
			if (brainActivity[structureLengthMinusOne][i] == 0)
			{
				output.push_back(0);
			}
			else
			{
				output.push_back(brain[structureLengthMinusOne][i]);
			}
		}
		return output;
	}

	void info()
	{
		cout << "score = " << score << endl;
		cout << endl
			 << "structureLengthMinusOne = " << structureLengthMinusOne << endl;
		int i, j, l;
		cout << endl
			 << "structure = " << endl;
		for (i = 0; i < structure.size(); i++)
		{
			cout << " " << structure[i];
		}
		cout << endl;
		cout << endl
			 << "bias = " << endl;
		for (i = 0; i < bias.size(); i++)
		{
			for (j = 0; j < bias[i].size(); j++)
			{
				cout << " " << bias[i][j];
			}
			cout << endl;
		}
		cout << endl
			 << "weights = " << endl;
		for (i = 0; i < weights.size(); i++)
		{
			for (j = 0; j < weights[i].size(); j++)
			{
				for (l = 0; l < weights[i][j].size(); l++)
				{
					cout << " " << weights[i][j][l];
				}
				cout << endl;
			}
			cout << endl;
		}
		cout << "brain = " << endl;
		for (i = 0; i < brain.size(); i++)
		{
			for (j = 0; j < brain[i].size(); j++)
			{
				cout << " " << brain[i][j];
			}
			cout << endl;
		}
		cout << endl
			 << "brainActivity = " << endl;
		for (i = 0; i < brainActivity.size(); i++)
		{
			for (j = 0; j < brainActivity[i].size(); j++)
			{
				cout << " " << brainActivity[i][j];
			}
			cout << endl;
		}
		cout << endl;
	}
};

class game
{
public:
	int boardState[boardSize][boardSize];

	void initialize()
	{
		int i, j;
		for (i = 0; i < boardSize; i++)
		{
			for (j = 0; j < boardSize; j++)
			{
				boardState[i][j] = 0;
			}
		}
	}

	void info()
	{
		int i, j;
		for (i = 0; i < boardSize; i++)
		{
			for (j = 0; j < boardSize; j++)
			{
				if (boardState[i][j] == 1)
				{
					cout << "x ";
				}
				if (boardState[i][j] == -1)
				{
					cout << "O ";
				}
				if (boardState[i][j] == 0)
				{
					cout << "- ";
				}
			}
			cout << endl;
		}
		cout << endl;
	}

	vector<int> getState(int team)
	{
		int i, j;
		vector<int> input;
		for (i = 0; i < boardSize; i++)
		{
			for (j = 0; j < boardSize; j++)
			{
				input.push_back(boardState[i][j] * team);
			}
		}
		return input;
	}

	int gameOver(int mx, int my, int team)
	{
		int connectionDirections[4][2] = {{1, -1}, {1, 0}, {1, 1}, {0, 1}};
		int i;
		int forwardConnections, backwardConnections;
		int px, py;
		for (i = 0; i < 4; i++)
		{
			px = mx;
			py = my;
			forwardConnections = 0;
			while (true)
			{
				px += connectionDirections[i][0];
				py += connectionDirections[i][1];
				if (px == -1 || px == boardSize)
				{
					break;
				}
				if (py == -1 || py == boardSize)
				{
					break;
				}
				if (boardState[px][py] != team)
				{
					break;
				}
				forwardConnections++;
			}
			px = mx;
			py = my;
			backwardConnections = 0;
			while (true)
			{
				px -= connectionDirections[i][0];
				py -= connectionDirections[i][1];
				if (px == -1 || px == boardSize)
				{
					break;
				}
				if (py == -1 || py == boardSize)
				{
					break;
				}
				if (boardState[px][py] != team)
				{
					break;
				}
				backwardConnections++;
			}
			if (1 + forwardConnections + backwardConnections >= boardSize)
			{
				return true;
			}
		}
		return false;
	}

	void start(agent *firstPlayer, agent *secondPlayer, bool viewMatch)
	{
		initialize();
		int i, j, pos;
		int winner = 0;
		int team = 1;
		double max;
		agent &player1 = *firstPlayer;
		agent &player2 = *secondPlayer;
		vector<double> output;
		for (i = 0; i < 9; i++)
		{
			team = 0 - team;
			if (team == 1)
			{
				output = player1.evaluate(getState(1));
			}
			else
			{
				output = player2.evaluate(getState(-1));
			}
			pos = -1;
			for (j = 0; j < boardSize * boardSize; j++)
			{
				if (boardState[j / boardSize][j % boardSize] == 0)
				{
					if (pos == -1 || output[j] > max)
					{
						max = output[j];
						pos = j;
					}
				}
			}
			boardState[pos / boardSize][pos % boardSize] = team;
			if (viewMatch)
			{
				info();
			}
			if (gameOver(pos / boardSize, pos % boardSize, team))
			{
				winner = team;
				break;
			}
		}
		player1.score += winner + 1;
		player2.score += 1 - winner;
	}
};

vector<agent> allAgents;

vector<agent *> allAgentPointers;

void collectAllAgents(int num, vector<int> blueprint)
{
	int i;
	for (i = 0; i < num; i++)
	{
		agent joe;
		joe.initialize(blueprint);
		joe.mutate(0.1);
		allAgents.push_back(joe);
	}
}

void collectAllAgentPointers()
{
	int i;
	allAgentPointers.clear();
	for (i = 0; i < allAgents.size(); i++)
	{
		allAgents[i].score = 0;
		allAgentPointers.push_back(&allAgents[i]);
	}
}

void matchMakeGlobalPopulation(double percent)
{
	percent *= 0.5;
	int i, j, tempRand;
	int totalAgents = allAgentPointers.size();
	int numberOfBattles = (int)(totalAgents * percent) + 1;
	agent *temp;
	for (i = 0; i < totalAgents; i++)
	{
		temp = allAgentPointers[i];
		tempRand = rand() % totalAgents;
		allAgentPointers[i] = allAgentPointers[tempRand];
		allAgentPointers[tempRand] = temp;
	}
	for (i = 0; i < totalAgents; i++)
	{
		for (j = 0; j < numberOfBattles; j++)
		{
			game match;
			match.start(allAgentPointers[i], allAgentPointers[(i + j + 1) % totalAgents], false);
			match.start(allAgentPointers[(i + j + 1) % totalAgents], allAgentPointers[i], false);
		}
	}
}

bool operator<(agent a1, agent a2)
{
	return a1.score > a2.score;
}

void allAgentSelection(double percent)
{
	sort(allAgents.begin(), allAgents.begin() + allAgents.size());
	int i;
	int top = (int)(allAgents.size() * percent) + 1;
	for (i = top; i < allAgents.size(); i++)
	{
		allAgents[i] = allAgents[i % top].copy();
		allAgents[i].mutate(0.1);
	}
}

void timeFunction()
{
	clock_t start = clock();
	int i;
	for (i = 0; i < 100; i++)
	{
		collectAllAgentPointers();
		matchMakeGlobalPopulation(0.3);
		allAgentSelection(0.1);
	}
	cout << "Time: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
}

int main()
{
	srand(time(NULL));
	randDouble();

	collectAllAgents(100, {9, 4, 9});
	int i;
	timeFunction();
	agent preBest = allAgents[0].copy();
	for (i = 0; i < 1000; i++)
	{
		collectAllAgentPointers();
		matchMakeGlobalPopulation(0.3);
		allAgentSelection(0.1);
		if (i % 10 == 0)
		{
			cout << (double)i / 1000 << endl;
		}
	}
	agent best = allAgents[0].copy();
	best.info();
	preBest.info();
	game match;
	best.score = 0;
	preBest.score = 0;
	match.start(&preBest, &best, true);
	cout << "Best's score: " << best.score << " and PreBest's score: " << preBest.score << endl;
	best.score = 0;
	preBest.score = 0;
	match.start(&best, &preBest, true);
	cout << "Best's score: " << best.score << " and PreBest's score: " << preBest.score << endl;

	// for (i = 0; i < allAgents.size(); i++)
	// {
	// 	cout << allAgents[i].score << endl;
	// }
	// agent joe;
	// joe.initialize({9, 9, 9});
	// joe.mutate(1.0);
	// joe.evaluate({1, 2, 3, 4, 5, 6, 7, 8, 9});
	// joe.info();
}