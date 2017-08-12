#ifndef __TRANSFORM_HPP_
#define __TRANSFORM_HPP_
#include "util.hpp"
namespace kurff{
class Transform{
    Transform(float focal_length, float center_x, float center_y): 
    focal_length_(focal_length), center_x_(center_x), center_y_(center_y) {

    }
    ~Transform(){

    }

    void t3dto2d(const Point3d& point3d, Point2d& point2d){

    }
    void t2dto3d(const Point2d& point2d, Point3d& point3d){

    }

    protected:
        float focal_length_;
        float center_x_;
        float center_y_;
};
}




#endif