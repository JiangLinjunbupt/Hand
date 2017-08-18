#ifndef __DATA_HPP__
#define __DATA_HPP__
#include "io.hpp"
#include <string>
using namespace std;
namespace kurff{
    class Data{
        public:
            Data(string name){
                io_ = IORegistry()->Create(name);

                
            }

            ~Data(){


            }

            void write_db(string name){
                 


            }

        protected:
            unique_ptr<IO> io_;



    };






}









#endif