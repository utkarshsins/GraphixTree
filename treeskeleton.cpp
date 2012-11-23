#include "treeskeleton.h"
#include <iostream>
#include <stack>

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
    if(flat_count > 1)
    {
        total_branches = ((int) pow(flat_count, depth_count) - 1)/(flat_count-1);
    }
    else
    {
        total_branches = flat_count*depth_count;
    }
    branches = new Branch[total_branches];
    int start_index = 0;
    int prev_count = 1;
    int step_count = 1;

	#ifdef VERBOSE
		std::cout << "[INFUN] Make Tree" << std::endl;
	#endif

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
			double anglez_random = 360.0/(double) flat_count*BetaNoise::gauss();
            for(int k = 0; k < (i==0? 1 : flat_count); k++)
            {
                int index = start_index + j*flat_count + k;

				#ifdef VERBOSE
					std::cout   << "[FUNOP] "
								<< i << "," << j << "," << k
								<< " : "
								<< index
								<< " child of : "
								<< ref
								<< std::endl;
				#endif

                if(ref >= 0)
                {
                    // vec3 ref_end;
                    // ref_end.copy(branches[ref].end_points.second);
                    // double refx = ref_end[0], refy = ref_end[1], refz = ref_end[2];
					double angley = 180.0/4.0;
					double anglez = 360.0*(double) k/(double) flat_count + anglez_random;
					double base_length = branch_length*cos(anglez);

                    branches[index].set
                        (
							vec3(0, angley, anglez),
							branch_length,
							i,
							branches[ref].endthickness,
							branches[ref].endthickness - branch_delta
						);
                }
                else
                {
                    branches[0].set
                        (
                            vec3(0,0,0),
							branch_length,
							0,
                            branch_thickness,
                            branch_thickness - branch_delta
                        );
                }
            }
        }
        start_index += step_count;
        prev_count = step_count;
    }

	#ifdef VERBOSE
		std::cout << "[OUTFN] Make Tree" << std::endl;
	#endif

}

void TreeSkeleton :: nextFrame(double time, Wind wind)
{
    for(int i = 1; i < total_branches; i++)
    {
		#ifdef VERBOSE2
			std::cout << "BRANCH [" << i << "]" << std::endl;
		#endif
        branches[i].wind_listener(wind, time);
    }
}

void TreeSkeleton :: paint()
{
	#ifdef VERBOSE
		std::cout << "[INFUN] Paint Tree" << std::endl;
	#endif

	// Iterative DFS - Stack Algorithm
	// Make tree was done using BFS
	
	int push = 0;
	std::stack<int> dfs;
	dfs.push(0);	// stem
	while(!dfs.empty())
	{
		int index = dfs.top();

		push++;
		glPushMatrix();
		branches[index].paint();
		
		#ifdef VERBOSE
			std::cout << "[FUNOP] Painting : " << index << "\t";
			/*float matrix[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
			for(int i = 0; i < 16; i++)
				std::cout << matrix[i] << "\t";
			std::cout << std::endl;*/
		#endif

		dfs.pop();
		int offset = index * flat_count;
		if(offset < total_branches - 1)
			for(int k = flat_count; k > 0; k--)
			{
				dfs.push(offset + k);
			}
		else
		{
			push--;
			glPopMatrix();
		}

		if(!dfs.empty())
			if(dfs.top() < index)
			{
				push--;
				glPopMatrix();
			}
	}

	// Stack DFS ends without Popping all Matrices
	for(int i=0; i<push; i++)
	{
		glPopMatrix();
	}
		
	#ifdef VERBOSE
		std::cout << "[OUTFN] Paint Tree" << std::endl;
	#endif
}

