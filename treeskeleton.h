#ifndef TREESKELETON_H
#define TREESKELETON_H

#include "branch.h"
#include "betanoise.h"
#include <cmath>

class TreeSkeleton
{
    public:
        Branch* branches;
        int total_branches;
        int depth_count, flat_count;
        double branch_length, branch_thickness, branch_delta;

        TreeSkeleton();
        TreeSkeleton(int , int );
        TreeSkeleton(int , int , double , double );
        void makeTree();
        void paint(long long);
		void nextFrame();
		void wind_listener(long long, Wind);
};

#endif // TREESKELETON_H
