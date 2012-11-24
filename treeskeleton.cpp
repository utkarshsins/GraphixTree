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
			double angley_random = 2.0*M_PI/(double) flat_count*BetaNoise::gauss();
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
                    vec3 ref_end;
                    ref_end.copy(branches[ref].end_points.second);
                    double refx = ref_end[0], refy = ref_end[1], refz = ref_end[2];
					double anglez = M_PI/4.0;
					double angley = 2.0*M_PI*(double) k/(double) flat_count + angley_random;
					double base_length = branch_length*cos(anglez);

					#ifdef VERBOSE
                        std::cout << "end points : ";
                        ref_end.printvec();
                        std::cout << std::endl;
                        std::cout << refx+base_length*cos(angley)
                                  << " , "
								  << refy+branch_length*sin(anglez)
								  << " , "
								  << refz+base_length*sin(angley);
                        std::cout << std::endl;
					#endif

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
							#ifdef DEBUG_LEAF_BETA
								index==1,
							#endif
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
							#ifdef DEBUG_LEAF_BETA
								false,
							#endif
                            NULL
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

void TreeSkeleton :: nextFrame()
{
    for(int i = 1; i < total_branches; i++)
    {
		#ifdef VERBOSE2
			std::cout << "BRANCH [" << i << "]" << std::endl;
		#endif
    	branches[i].current_angle.increase(branches[i].bent_angle, vec3(BRANCH_ROTATION_PER_FRAME, BRANCH_ROTATION_PER_FRAME, BRANCH_ROTATION_PER_FRAME));
    }
}

void TreeSkeleton :: wind_listener(long long time, Wind wind)
{
	for(int i = 1; i<total_branches; i++)
	{
		branches[i].wind_listener(wind, time);
	}
}

void TreeSkeleton :: paint(long long now)
{
    int index = 0, level = 1;
    int prev_index = -1;

	double val = std::fmod((long double) now / 1000.0, (long double)ONEFBETA_SCALE) / ONEFBETA_SCALE;
	#ifdef DEBUG_SINGLE_LEAF
	std::cout << "VAL : " << val << "\t\t\tTIME : " << (now / 1000) % 1000 << "\t\t\t";
	#endif
	std::vector<bool> rendered(total_branches,false);
    for(int i = 0; i < total_branches; i++)
    {
        rendered[i] = false;
    }

    glPushMatrix();
        glTranslated(branches[0].end_points.first[0], branches[0].end_points.first[1], branches[0].end_points.first[2]);
        //glRotated(branches[0].current_angle[0] * (180.0/M_PI), 1, 0, 0);
        //glRotated(branches[0].current_angle[2] * (180.0/M_PI), 0, 0, 1);
        glTranslated(-branches[0].end_points.first[0], -branches[0].end_points.first[1], -branches[0].end_points.first[2]);
		#ifdef DEBUG_SINGLE_LEAF
		branches[0].paint(val, false);
		#else
		branches[0].paint(val);
		#endif
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
        int temp_no;
        if(f > 1)
        {
            temp_no = (pow(f,level-1) - 1) / (f-1);
        }
        else
        {
            temp_no = level - 1;
        }
        if(!rendered[index])
        {
            glPushMatrix();
                glTranslated(branches[index].end_points.first[0], branches[index].end_points.first[1], branches[index].end_points.first[2]);
				double valindex = val * ((double) (branches[index].betasize-1));
				double linearinterpol = branches[index].betanoise[floor(valindex)] * (1.0 - fmod(valindex, 1.0)) + branches[index].betanoise[ceil(valindex)] * fmod(valindex, 1.0);
				glRotated(branches[index].current_angle[0] * (180.0/M_PI) + BRANCH_BETA_ROTATION_LIMIT_X * linearinterpol, 1, 0, 0);
                glRotated(branches[index].current_angle[2] * (180.0/M_PI) + BRANCH_BETA_ROTATION_LIMIT_Z * linearinterpol, 0, 0, 1);
                glTranslated(-branches[index].end_points.first[0], -branches[index].end_points.first[1], -branches[index].end_points.first[2]);
				#ifdef DEBUG_SINGLE_LEAF
				branches[index].paint(val,index==1);
				#else
                branches[index].paint(val);
				#endif
            rendered[index] = true;

            if(f > 1)
            {
                next_index = ((pow(f,level)-1) / (f-1)) + f*((index - temp_no));
                if(next_index >= total_branches)
                {
                    if(((index - temp_no + 1) % f > 0) || index-prev_index > 1)
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
                index++;
                prev_index = index - 1;
                level++;
                continue;
            }
        }
        else
        {

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

    glPopMatrix();

	//std::cout	<< "====================================================================================="
	//			<< std::endl;
}

