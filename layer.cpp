#include "functions.cpp"


using namespace std;

class Layer{
    public:
        Matrix nodes;
        Matrix bias;
        int layer_function;
        long long next_layer_num;
        Layer(){

        };
        Layer(long long number_of_nodes,long long next_layer,int layer_func=1, bool randomize=true){
            nodes.resize1(next_layer,number_of_nodes,randomize);
            bias.resize1(next_layer,1,true);
            next_layer_num=next_layer;
            layer_function=layer_func;
        };
        Matrix getoutput(Matrix t_inputs){
            // cerr<<"Output\n";
            Matrix output=Matrix::multiply(nodes,t_inputs);
            output.add(bias);
            switch(layer_function){
                case 1:
                    output.map1(sigmoid);
                    break;
                case 2:
                    output.map1(tanh);
                    break;
                case 3:
                    output.map1(linear);
                    break;
                case 4:
                    output.map1(dGCU);// error nan
                    break;
                case 5:
                    output.map1(relu);// error nan
                    break;
            }
            return output;
        }
        void exportl(){
            nodes.print();
            bias.print();
            cout<<layer_function<<" "<<next_layer_num<<endl;
        }
        void importl(){
            nodes=Matrix::import1();
            bias=Matrix::import1();
            cin>>layer_function>>next_layer_num;
        }
};