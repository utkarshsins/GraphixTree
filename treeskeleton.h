#ifndef TREESKELETON_H
#define TREESKELETON_H

#include "branch.h"

class TreeSkeleton
{
    public:
        Branch* branches;
        int total_branches;
        int depth_count, flat_count;
        double branch_length, branch_thickness;

        TreeSkeleton();
        TreeSkeleton(int , int );
        TreeSkeleton(int , int , double , double );
        void makeTree();
        void paint();
};

#endif // TREESKELETON_H
