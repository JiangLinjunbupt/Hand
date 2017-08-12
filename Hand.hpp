#ifndef __HAND_H__
#define __HAND_H__
#include <vector>
#include "Model.hpp"
#include "util.hpp"
// 3D hand keypoints regression using caffe2
// 
using namespace std;
namespace kurff{


    class Hand{
        public:
            Hand(){

            }
            ~Hand(){


            }
            void init(){

                
            }
            void run(){



            }

            vector<Point3d>& get_keypoints(){
                return keypoints_;
            }

        protected:
            shared_ptr<Model> model_;
            vector<Point3d> keypoints_;
            shared_ptr<float> result_;




    };





}








#endif