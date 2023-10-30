#include "layer.cpp"

using namespace std;

class Neural_Network
{
public:
    long long input_nodes;
    long long hidden_layers;
    long long hidden_nodes;
    long long output_nodes;
    double learning_rate = 0.01;
    vector<Layer> layers;
    Neural_Network()
    {
    }
    Neural_Network(long long i_input_nodes, long long i_hidden_nodes, long long i_output_nodes, long long i_hidden_layers = 1, bool mutate=true)
    {
        input_nodes = i_input_nodes;
        hidden_layers = i_hidden_layers;
        hidden_nodes = i_hidden_nodes;
        output_nodes = i_output_nodes;
        layers.resize(1 + i_hidden_layers);
        layers[0] = Layer(input_nodes, hidden_nodes);
        layers[layers.size() - 1] = Layer(hidden_nodes, output_nodes,1,mutate);
        for (int i = 1; i < layers.size() - 1; i++)
        {
            layers[i] = Layer(hidden_nodes, hidden_nodes, 1,mutate);
        }
    }
    double *feedforward(double input[], double arr[])
    {
        Matrix temp = Matrix::fromarray(input, input_nodes);
        for (int i = 0; i < layers.size(); i++)
        {
            temp = layers[i].getoutput(temp);
        }
        return temp.toarray(arr);
    }
    Matrix feedforward(double input[], int node)
    {
        Matrix temp = Matrix::fromarray(input, input_nodes);
        for (int i = 0; i < node; i++)
        {
            temp = layers[i].getoutput(temp);
        }
        return temp;
    }
    void train(double input[], double target[])
    {
        Matrix inputs = Matrix::fromarray(input, input_nodes);
        Matrix output = feedforward(input, layers.size());
        Matrix targets = Matrix::fromarray(target, output_nodes);
        Matrix errorofuotput = Matrix::subtract(targets, output);
        errorofuotput.map1(quadra);
        Matrix gradients; 
        switch (layers[layers.size() - 1].layer_function)
        {
        case 1:
            gradients = Matrix::map1(output, dsigmoid2);
            break;
        case 2:
            gradients = Matrix::map1(output, dtanh);
            break;
        case 3:
            gradients = Matrix::map1(output, dlinear);
            break;
        case 4:
            gradients = Matrix::map1(output, dGCU);
            break;
        case 5:
            gradients = Matrix::map1(output, drelu);
            break;
        }
        gradients.multiply(errorofuotput);
        gradients.multiply(learning_rate);

        Matrix hidden_T = Matrix::transpose(feedforward(input, layers.size() - 1));
        Matrix weight_ho_deltas = Matrix::multiply(gradients, hidden_T);
        Matrix who_t = Matrix::transpose(layers[layers.size() - 1].nodes);
        Matrix hidden_errors = Matrix::multiply(who_t, errorofuotput);
        layers[layers.size() - 1].nodes.add(weight_ho_deltas);
        layers[layers.size() - 1].bias.add(gradients);

        for (int i = layers.size() - 2; i >= 1; i--)
        {                           // check if i must be >1
            Matrix hidden_gradient; // TODO: add switch
            switch (layers[i].layer_function)
            {
            case 1:
                hidden_gradient = Matrix::map1(feedforward(input, i + 1), dsigmoid2);
                break;
            case 2:
                hidden_gradient = Matrix::map1(feedforward(input, i + 1), dtanh);
                break;
            case 3:
                hidden_gradient = Matrix::map1(feedforward(input, i + 1), dlinear);
                break;
            case 4:
                hidden_gradient = Matrix::map1(feedforward(input, i + 1), dGCU);
                break;
            case 5:
                hidden_gradient = Matrix::map1(feedforward(input, i + 1), drelu);
                break;
            }
            hidden_gradient.multiply(hidden_errors);
            hidden_gradient.multiply(learning_rate);
            Matrix hidden_Trans = Matrix::transpose(feedforward(input, i));
            Matrix weight_hh_deltas = Matrix::multiply(hidden_gradient, hidden_Trans);
            Matrix whh_t = Matrix::transpose(layers[i].nodes);
            hidden_errors = Matrix::multiply(whh_t, hidden_errors);
            layers[i].nodes.add(weight_hh_deltas);
            layers[i].bias.add(hidden_gradient);
        }
        Matrix input_gradient; // TODO: add switch
        switch (layers[0].layer_function)
        {
        case 1:
            input_gradient = Matrix::map1(feedforward(input, 1), dsigmoid2);
            break;
        case 2:
            input_gradient = Matrix::map1(feedforward(input, 1), dtanh);
            break;
        case 3:
            input_gradient = Matrix::map1(feedforward(input, 1), dlinear);
            break;
        case 4:
            input_gradient = Matrix::map1(feedforward(input, 1), dGCU);
            break;
        case 5:
            input_gradient = Matrix::map1(feedforward(input, 1), drelu);
            break;
        }
        input_gradient.multiply(hidden_errors);
        input_gradient.multiply(learning_rate);

        Matrix inputs_t = Matrix::transpose(inputs);
        Matrix weight_ih_deltas = Matrix::multiply(input_gradient, inputs_t);

        layers[0].nodes.add(weight_ih_deltas);
        layers[0].bias.add(input_gradient);
    }
    void exportBrain(){
        cout<<input_nodes<<" "<<hidden_nodes<<" "<<output_nodes<<" "<<hidden_layers<<" "<<learning_rate<<"\n";
        for(auto layer1: layers ){
            layer1.exportl();
        }
        return;
    }
    static Neural_Network importbrain(){
        double learning_r1;
        long long input1,hiden1,output1,hidden_l1;cin>>input1>>hiden1>>output1>>hidden_l1>>learning_r1;
        Neural_Network out(input1,hiden1,output1,hidden_l1,false);
        out.learning_rate=learning_r1;
        for(int i=0;i<out.layers.size();i++){
            out.layers[i].importl();
        }
        return out;
    }
    void mutate(){
        for(auto layer1: layers ){
            layer1.nodes.map1(mutate_in);
            layer1.bias.map1(mutate_in);
        }
    }
    static Neural_Network mutate2(Neural_Network a,Neural_Network b){
        Neural_Network child(a.input_nodes,a.hidden_nodes,a.output_nodes,a.hidden_layers);
        child.learning_rate=a.learning_rate;
        for(int i =0;i<a.layers.size();i++){
            for(int ii=0;ii<a.layers[i].nodes.data.size();ii++){
                for(int jj=0;jj<a.layers[i].nodes.data[ii].size();jj++){
                    if(randval()<0.4){
                        child.layers[i].nodes.data[ii][jj]=a.layers[i].nodes.data[ii][jj]+b.layers[i].nodes.data[ii][jj];
                    }
                }
            }
            for(int ii=0;ii<a.layers[i].bias.data.size();ii++){
                for(int jj=0;jj<a.layers[i].bias.data[ii].size();jj++){
                    if(randval()<0.4){
                        child.layers[i].bias.data[ii][jj]=a.layers[i].bias.data[ii][jj]+b.layers[i].bias.data[ii][jj];
                    }
                }
            }
            child.layers[i].layer_function=a.layers[i].layer_function;
        }
        return child;

    }

};
