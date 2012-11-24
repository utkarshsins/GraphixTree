#include "branch.h"

const GLfloat Branch::mat_ambient[] =
    { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat Branch::mat_diffuse[] =
    { 0.55f, 0.25f, 0.09f, 1.0f };
const GLfloat Branch::mat_specular[] =
    { .55f, .25f, .09f, 1.0f };
const GLfloat Branch::high_shininess[] =
    { 100.0f };

Branch::Branch()
{
    end_points = make_pair(vec3(0,0,0), vec3(0,0,0));
    current_angle.set(0,0,0);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = .2;
    endthickness = .2;
    length = .5;
    elastic_modulus = E_MODULUS;
    bent_angle_changed = false;
    addleaves(0*MAX_LEAVES, false);

}

Branch :: Branch(vec3 start, vec3 end, int depth)
{
    end_points = make_pair(start, end);
    current_angle.set(0,0,0);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = .2;
    endthickness = .2;
    elastic_modulus = E_MODULUS;
    bent_angle_changed = false;
    length = dist(start, end);

    addleaves(depth*MAX_LEAVES, false);

}

Branch :: Branch(vec3 start, vec3 end, double startt, double endt, int depth)
{
    end_points = make_pair(start, end);
    current_angle.set(0,0,0);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = startt;
    endthickness = endt;
    length = dist(start, end);
    elastic_modulus = E_MODULUS;
    bent_angle_changed = false;
    addleaves(depth*MAX_LEAVES, false);

}

#ifdef DEBUG_LEAF_BETA
void Branch :: set(vec3 start, vec3 end, double startt, double endt, int depth, bool printbeta, Branch *branch)
#else
void Branch :: set(vec3 start, vec3 end, double startt, double endt, int depth, Branch *branch)
#endif
{
	#ifdef VERBOSE
		std::cout << "[VECOP] Start : " ;
		start.printvec();
		std::cout << std::endl;
		std::cout << "[VECOP] End : ";
		end.printvec();
		std::cout << std::endl;
	#endif

    end_points = make_pair(start, end);
    current_angle.set(0,0,0);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = startt;
    endthickness = endt;
    bent_angle_changed = false;
    length = dist(start, end);

	#ifdef DEBUG_LEAF_BETA
    addleaves(depth*MAX_LEAVES, printbeta);
	#else
	addleaves(depth*MAX_LEAVES, false);
	#endif

    elastic_modulus = E_MODULUS;

	betanoise = BetaNoise::librarynoise(ONEFBETA_2POW, ONEFBETA_BETA);
	betasize = betanoise.size();
}

void Branch :: addleaves(int leavesmax, bool printbeta)
{
    Leaf leaf;
    leaves.insert(leaves.begin(), leavesmax, leaf);
    for(int i=0; i<leavesmax; i++)
    {
        // leaves.push_back(leaf);
        leaves[i].set(length, i==0 && printbeta);
    }
}

void Branch :: wind_listener(Wind wind, long long program_time)
{
    double xload = wind.force_at(0,program_time);// * (1 + (rand()*1.0 / RAND_MAX));
    double zload = wind.force_at(1,program_time);// * (1 + (rand()*1.0 / RAND_MAX));
    double spring_constant = (elastic_modulus * endthickness * (startthickness-endthickness)) / (4*pow(length,3));
	//spring_constant = 5.0*(sin(spring_constant) + cos(spring_constant));
    if(spring_constant == 0)
    {
        spring_constant = .01;
    }
    double dx = xload/spring_constant;
    double dz = zload/spring_constant;
    if(abs(dx) >= length)
    {
        dx = (dx / abs(dx)) * length;
		#ifdef VERBOSE2
			cout << "[ERROR] |dx| > length" << endl;
		#endif
    }
    if(abs(dz) >= length)
    {
        dz = (dz / abs(dz)) * length;
		#ifdef VERBOSE2
			cout << "[ERROR] |dz| > length" << endl;
		#endif
    }
    double bent_angle0 = asin(dx/length);
    double bent_angle2 = asin(dz/length);
	#ifdef VERBOSE2
		cout << "XLoad : " << xload << endl;
		cout << "ZLoad : " << zload << endl;
        cout << "Spring constant " << spring_constant << endl;
		cout << "\tdx : " << dx << " dz : " << dz << " length : " << length << endl;
		cout << "Bent Angle : " ;
		bent_angle.printvec();
		cout << endl << endl;
		//int n;
		//cin >> n;
	#endif


    //for(int i = 0; i < 3; i++)
    //{
    if(fabs(bent_angle0) > max_bent_angle[0]*M_PI/180)
    {
        bent_angle0 = (bent_angle0/fabs(bent_angle0)) * max_bent_angle[0] * M_PI/180.0;
    }
    if(fabs(bent_angle2) > max_bent_angle[2]*M_PI/180)
    {
        bent_angle2 = (bent_angle2/fabs(bent_angle2)) * max_bent_angle[2] * M_PI/180.0;
    }

    if(bent_angle[0] == bent_angle0 && bent_angle[2] == bent_angle2)
    {
        bent_angle_changed = false;
    }
    else
    {
        bent_angle_changed = true;
        bent_angle[0] = bent_angle0;
        bent_angle[2] = bent_angle2;
    }
    //}
}

#ifdef DEBUG_SINGLE_LEAF
void Branch :: paint(double leaf_time, bool debug)
#else
void Branch :: paint(double leaf_time)
#endif
{
    double xn = end_points.second[0]-end_points.first[0], yn = end_points.second[1]-end_points.first[1], zn = end_points.second[2]-end_points.first[2];
    double angle = acos(yn/sqrt(xn*xn+yn*yn+zn*zn)) * 180/M_PI;

    glPushMatrix();
        glTranslated(end_points.first[0], end_points.first[1], end_points.first[2]);
        glRotated(-angle, (-length*zn), 0, length*xn);
        glRotated(-90, 1, 0, 0);

        Leaf::setMaterial();

		if(RENDER_LEAVES)
		{
			int istart = 0;

			#ifdef DEBUG_SINGLE_LEAF
				if(debug)
				{
					leaves[0].paint(leaf_time,true);
					istart = 1;
				}
			#endif

			for(int i=istart; i<leaves.size(); i++)
			{
				#ifdef DEBUG_SINGLE_LEAF
				leaves[i].paint(leaf_time,false);
				#else
				leaves[i].paint(leaf_time);
				#endif
			}
		}

        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


        glPushMatrix();

			if(RENDER_BRANCH)
			{
				glBegin(GL_POLYGON);
					GLUquadricObj* cylinder = gluNewQuadric();
					gluCylinder(cylinder, startthickness/2, endthickness/2, length, 8 , 10);
				glEnd();

				glTranslated(0, 0, length);
				glBegin(GL_POLYGON);
					GLUquadricObj* disc = gluNewQuadric();
					gluDisk(disc, 0, endthickness/2, 8, 10);
				glEnd();
			}

        glPopMatrix();
    glPopMatrix();

}
