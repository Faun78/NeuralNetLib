#include <algorithm>
#include "matrix.cpp"


using namespace std;

double sigmoid(double x){
    return (1/(1+exp(-x)));
}
double dsigmoid2(double y){
    //return sigmoid(x)*(1-sigmoid(x));
    return y*(1-y);
}
//tanh I have from cmath
double dtanh(double y){
    return 1 - (y * y);
}

double relu(double x){
    if(x>0){
        return x*0.01;
    }else{
        return x;
    }
}
double drelu(double y){
    if(y>0){
        return 0.01;
    }else{
        return 1;
    }
}
double GCU(double x){
    return x*cosh(x); // y= x*cos(x) 
}
double dGCU(double y){
    double x=sqrt(y/(acosh(y)/y));
    return cosh(x) - sinh(x)*x;
}
double linear(double x){
    return x;
}
double dlinear(double y){
    return 1;
}
double randval(int min=0,int max=1){
    static std::uniform_real_distribution<> dis99(min, max); // rage 0 - 1
    return dis99(mt2);
}
double mutate_in(double x){
    if(dis00(mt2)<0.1){
        return x+randval(-1,1);
    }
    return x;
}
double quadra(double x){
    if(x>0){
    return x*x;
    }
    return (x*x)*-1;
}
