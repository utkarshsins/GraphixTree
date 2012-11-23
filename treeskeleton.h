#ifndef TREESKELETON_H
#define TREESKELETON_H

#include "branch.h"
#include "betanoise.h"

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
		void nextFrame(double , Wind);
};

#endif // TREESKELETON_H
