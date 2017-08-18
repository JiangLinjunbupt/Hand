#ifndef __PROCESS_HPP__
#define __PROCESS_HPP__
#include "registry.h"
namespace kurff{
class Process{
    public:
        Process(){

        }

        ~Process(){


        }

        virtual void run() {}
    protected:
};

DECLARE_REGISTRY(ProcessRegistry, Process);
DEFINE_REGISTRY(ProcessRegistry, Process);


class MedianProcess: public Process{
    public:
        MedianProcess(){

        }
        ~MedianProcess(){


        }

        void run(){


        }

    protected:

};

REGISTER_CLASS(ProcessRegistry, MedianProcess, MedianProcess);


class MeanProcess: public Process{
    public:
        MeanProcess(){

        }
        ~MeanProcess(){

        }

        void run(){


        }
    protected:



};

REGISTER_CLASS(ProcessRegistry,MeanProcess, MeanProcess);






}







#endif