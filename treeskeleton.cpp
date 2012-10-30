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
    total_branches = ((int) pow(flat_count, depth_count) - 1)/(flat_count-1);
    branches = new Branch[total_branches];
    int start_index = 0;
    int prev_count = 1;
    int step_count = 1;

	if(VERBOSE)
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
        for(int j = 0; j < prev_count; j++)
        {
            int ref = start_index - (prev_count - j);
            for(int k = 0; k < (i==0? 1 : flat_count); k++)
            {
                int index = start_index + j*flat_count + k;

				if(VERBOSE)
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
                            i,
                            &branches[ref]
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
                            0,
                            NULL
                        );
                }
                std::cout << std::endl;
            }
        }
        start_index += step_count;
        prev_count = step_count;
    }

	if(VERBOSE)
		std::cout << "[OUTFN] Make Tree" << std::endl;

}

void TreeSkeleton :: paint(double time, Wind wind)
{

    for(int i = 0; i < total_branches; i++)
    {
        branches[i].wind_listener(wind, time);
    }
    int index = 0, level = 1;
    int prev_index = -1;
	std::vector<bool> rendered(total_branches,false);
    for(int i = 0; i < total_branches; i++)
    {
        rendered[i] = false;
    }

    if(VERBOSE || VERBOSE2)
    {
        cout << "pushing index: 0 " << endl;

    }
    glPushMatrix();
        glTranslated(branches[0].end_points.first[0], branches[0].end_points.first[1], branches[0].end_points.first[2]);
        glRotated(branches[0].bent_angle[0], 1, 0, 0);
        glRotated(branches[0].bent_angle[2], 0, 0, 1);
        glTranslated(-branches[0].end_points.first[0], -branches[0].end_points.first[1], -branches[0].end_points.first[2]);
        branches[0].paint(time, wind);
    rendered[0] = true;
    index++;
    prev_index = 0;
    level++;
    int f = flat_count;
    int next_index;
    while(index < total_branches && index > 0)
    {
        //prev_index = index
        //int level = floor(log(index*(f-1) + 1) / log(flat_count)) + 1;
        int temp_no = (pow(f,level-1) - 1) / (f-1);
        if(VERBOSE || VERBOSE2)
        {
            cout<<"index : " << index << " level : " << level << endl;
        }
        if(!rendered[index])
        {
            if(VERBOSE || VERBOSE2)
            {
                cout << "pushing " << endl;
            }
            glPushMatrix();
                glTranslated(branches[index].end_points.first[0], branches[index].end_points.first[1], branches[index].end_points.first[2]);
                glRotated(branches[index].bent_angle[0], 1, 0, 0);
                glRotated(branches[index].bent_angle[2], 0, 0, 1);
                glTranslated(-branches[index].end_points.first[0], -branches[index].end_points.first[1], -branches[index].end_points.first[2]);
                branches[index].paint(time, wind);
            rendered[index] = true;
            next_index = ((pow(f,level)-1) / (f-1)) + f*((index - temp_no));
            if(next_index >= total_branches)
            {
                if(((index - temp_no + 1) % f > 0) || index-prev_index > 1)
                {
                    prev_index = index;
                    index++;
                    if(VERBOSE || VERBOSE2)
                    {
                        cout << "popping " << endl;
                    }

                    glPopMatrix();
                    continue;
                }
                else
                {
                    int prev_level_at = ceil((index - temp_no + 1) / f);
                    prev_index = index;
                    index = ( (pow(f,level-2)-1)/(f-1) ) + prev_level_at - 1;
                    level--;
                    if(VERBOSE || VERBOSE2)
                    {
                        cout << "popping " << endl;
                    }
                    glPopMatrix();
                    continue;
                }
            }
            else
            {
                prev_index = index;
                index = next_index;
                level++;
                continue;
            }
        }
        else
        {
            if(VERBOSE || VERBOSE2)
            {
                cout << "popping " << endl;
            }

            if(((index - temp_no + 1) % f > 0) && prev_index-index > 1)
            {
                prev_index = index;
                index++;
                glPopMatrix();
                continue;
            }
            else
            {
                int prev_level_at = ceil((index - temp_no + 1) / f);
                prev_index = index;
                index = ( (pow(f,level-2)-1)/(f-1) ) + prev_level_at - 1;
                level--;
                glPopMatrix();
                continue;
            }
        }

    }
    if(VERBOSE || VERBOSE2)
    {
        cout << "Final popping " << endl;
    }

    glPopMatrix();

}

