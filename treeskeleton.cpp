#include "treeskeleton.h"

TreeSkeleton::TreeSkeleton()
{
    flat_count = 2;
    depth_count = 5;
    branch_length = 1.0;
    branch_thickness = .2;
    //ctor
}

TreeSkeleton :: TreeSkeleton(int dc, int fc)
{
    depth_count = dc;
    flat_count = fc;
    branch_length = 1.0;
    branch_thickness = .2;
}

TreeSkeleton :: TreeSkeleton(int dc, int fc, double bl, double bt)
{
    depth_count = dc;
    flat_count = fc;
    branch_length = bl;
    branch_thickness = bt;
}

void TreeSkeleton :: makeTree()
{
    total_branches = flat_count * ((int) pow(flat_count, depth_count) - 1)/(flat_count-1);
    branches = new Branch[total_branches];
    int start_index = 0;
    int prev_count = 1;
    int step_count = 1;
    for(int i = 0; i < depth_count; i++)
    {
        if(i == 0)
        {
            step_count = 1;
        }
        else
        {
            step_count = prev_count * flat_count;
        }
        //branches[((int) pow(flat_count, i)) - 1 ] = new Branch[flat_count];
        //int step_count = (int) pow(flat_count, i);
        //int prev_count = step_count / flat_count;
        //int start_index = (i > 0 ? );
        for(int j = 0; j < prev_count; j++)
        {
            //int ref = ceil();
            //branches[start_index+j] = new Branch();
            int ref = start_index - (prev_count - j);
            for(int k = 0; k < (i==0? 1 : 3); k++)
            {
                int index = start_index + j*3 + k;
                if(ref >= 0)
                {
                    vec3 ref_end;
                    ref_end.copy(branches[ref].end_points.second);
                    double refx = ref_end[0], refy = ref_end[1], refz = ref_end[2];
                    double angle = (2*k+1)*1.0*M_PI / (2*flat_count*1.0);
                    branches[index].set(ref_end, vec3(refx+branch_length*cos(angle), refy+branch_length*sin(angle), refz), branches[ref].thickness);

                }
                else
                {
                    branches[0].set(vec3(0,0,0), vec3(0, branch_length, 0), branch_thickness);
                }
            }
        }
        start_index += step_count;
        prev_count = step_count;
    }

}

void TreeSkeleton :: paint()
{
    for(int i = 0; i < total_branches; i++)
    {
        branches[i].paint();
    }
}

