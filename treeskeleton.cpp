#include "treeskeleton.h"
#include <iostream>
TreeSkeleton::TreeSkeleton()
{
    flat_count = 2;
    depth_count = 5;
    branch_length = 1.0;
    branch_thickness = .2;
    branch_delta = (branch_thickness)/(depth_count+1.0);
    //ctor
}

TreeSkeleton :: TreeSkeleton(int dc, int fc)
{
    depth_count = dc;
    flat_count = fc;
    branch_length = 1.0;
    branch_thickness = .2;
    branch_delta = (branch_thickness)/(depth_count+1.0);
}

TreeSkeleton :: TreeSkeleton(int dc, int fc, double bl, double bt)
{
    depth_count = dc;
    flat_count = fc;
    branch_length = bl;
    branch_thickness = bt;
    branch_delta = (branch_thickness)/(depth_count*1.0);
}

void TreeSkeleton :: makeTree()
{
    total_branches = flat_count * ((int) pow(flat_count, depth_count) - 1)/(flat_count-1);
    branches = new Branch[total_branches];
    int start_index = 0;
    int prev_count = 1;
    int step_count = 1;

    std::cout << "[INFUN] Make Tree" << std::endl;

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
            for(int k = 0; k < (i==0? 1 : flat_count); k++)
            {
                int index = start_index + j*flat_count + k;

                std::cout   << "[FUNOP] " 
                            << i << "," << j << "," << k 
                            << " : " 
                            << index 
                            << std::endl;

                if(ref >= 0)
                {
                    vec3 ref_end;
                    ref_end.copy(branches[ref].end_points.second);
                    double refx = ref_end[0], refy = ref_end[1], refz = ref_end[2];
					double anglez = M_PI/4.0;
					double angley = 2.0*M_PI*(double) k/(double) flat_count;
					double base_length = branch_length*cos(anglez);
                    branches[index].set
                        (
                            ref_end, 
                            vec3
                                (
                                    refx+base_length*cos(angley), 
									refy+branch_length*sin(anglez), 
									refz+base_length*sin(angley)
                                ), 
                            branches[ref].endthickness,
                            branches[ref].endthickness - branch_delta,
                            i
                        );

                }
                else
                {
                    branches[0].set
                        (
                            vec3(0,0,0), 
                            vec3(0, branch_length, 0), 
                            branch_thickness, 
                            branch_thickness - branch_delta,
                            0
                        );
                }
                std::cout << std::endl;
            }
        }
        start_index += step_count;
        prev_count = step_count;
    }

    std::cout << "[OUTFN] Make Tree" << std::endl;

}

void TreeSkeleton :: paint()
{
    for(int i = 0; i < total_branches; i++)
    {
        branches[i].paint();
    }
}

