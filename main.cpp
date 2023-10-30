#include <iostream>
#include <vector>
#include "nn1.cpp"
using namespace std;

int main()
{
    Neural_Network brain(2, 10, 1, 4);
    brain.layers[1].layer_function = 3; // change activasion of hidden layer
    double input0[] = {0, 0};
    double target0[] = {0};
    double input1[] = {0, 1};
    double target1[] = {1};
    double input2[] = {1, 0};
    double target2[] = {1};
    double input3[] = {1, 1};
    double target3[] = {0};
    int c0 = 0;
    int c1 = 0;
    int c2 = 0;
    int c3 = 0;
    double arr[1];

    cerr << brain.feedforward(input0, arr)[0] << "\n";
    cerr << brain.feedforward(input1, arr)[0] << "\n";
    cerr << brain.feedforward(input2, arr)[0] << "\n";
    cerr << brain.feedforward(input3, arr)[0] << "\n"<<"\n";
    for (int i; i < 5000; i++)
    {
        int random = mt2() % (4 - 1 + 1) + 1;
        if (random == 4)
        {
            brain.train(input0, target0);
            c0++;
        }
        else if (random == 1)
        {
            brain.train(input1, target1);
            c1++;
        }
        else if (random == 2)
        {
            brain.train(input2, target2);
            c2++;
        }
        else if (random == 3)
        {
            brain.train(input3, target3);
            c3++;
        }
    }
    cerr << brain.feedforward(input0, arr)[0] << "\n";
    cerr << brain.feedforward(input1, arr)[0] << "\n";
    cerr << brain.feedforward(input2, arr)[0] << "\n";
    cerr << brain.feedforward(input3, arr)[0] << "\n"<<"\n";
    brain.exportBrain();
    return 0;
}