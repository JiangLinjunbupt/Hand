#ifndef __MODEL_HPP__
#define __MODEL_HPP__
#include <string>
using namespace std;


namespace kurff{

class Model{
    public:
        Model(){

        }
        ~Model(){

        }

        virtual void init(string model_file, string model_weight) = 0;
        virtual void run() = 0;
    protected:
        


};

class Caffe2Model: public Model{
    public:
        Caffe2Model(){

        }
        ~Caffe2Model(){

        }
        void init(string model_file, string model_weight){

            

        }

        void run(){

        }
    protected:



};









}





#endif