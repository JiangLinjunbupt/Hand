#ifndef __IO_HPP__
#define __IO_HPP__
#include "opencv2/opencv.hpp"
using namespace cv;
#include <string>
#include <vector>
using namespace std;
#include "registry.h"
namespace kurff{
  class IO{
      public:
        IO(string path, string anno_path):path_(path), anno_path_(anno_path){

        }
        ~IO(){


        }

        virtual void get_all() = 0 ;
        virtual void read_image(int idx, Mat& image) = 0;
        virtual void write_image(int idx, const Mat& image) = 0;

      protected:
        string path_;
        string anno_path_;
        string extra_path_;
        vector<string> file_name_;
  };


DECLARE_REGISTRY(IORegistry, IO);
DEFINE_REGISTRY(IORegistry, IO);


  class IVCLIO: public IO{
      public:
        IVCLIO(){

        }
        ~IVCLIO(){

        }

        void get_all(){

        }
        void read_image(int idx, Mat& image){
            image = imread(this->path_ + "/" + file_name_[idx]);
            


        }
        void write_image(int idx, const Mat& image){

        }

    
  };

  REGISTER_CLASS(IORegistry,IVCLIO, IVCLIO);







}






#endif