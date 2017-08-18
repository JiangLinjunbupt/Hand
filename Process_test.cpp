#include "Process.hpp"

using namespace kurff;

int main(){
    unique_ptr<Process> p = ProcessRegistry()->Create("Mean");
    

    return 0;
}