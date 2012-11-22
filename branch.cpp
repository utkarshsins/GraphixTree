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
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = .2;
    endthickness = .2;
    length = .5;
    elastic_modulus = E_MODULUS;
    addleaves(0*MAX_LEAVES);

}

Branch :: Branch(vec3 start, vec3 end, int depth)
{
    end_points = make_pair(start, end);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = .2;
    endthickness = .2;
    elastic_modulus = E_MODULUS;
    length = dist(start, end);

    addleaves(depth*MAX_LEAVES);

}

Branch :: Branch(vec3 start, vec3 end, double startt, double endt, int depth)
{
    end_points = make_pair(start, end);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = startt;
    endthickness = endt;
    length = dist(start, end);
    elastic_modulus = E_MODULUS;
    addleaves(depth*MAX_LEAVES);

}

void Branch :: copy(Branch b)
{
    end_points = make_pair(b.end_points.first, b.end_points.second);
    bent_angle.copy(b.bent_angle);
    max_bent_angle.copy(b.max_bent_angle);
    startthickness = b.startthickness;
    endthickness = b.endthickness;
    length = b.length;
    elastic_modulus = b.elastic_modulus;

}

void Branch :: set(vec3 start, vec3 end, double startt, double endt, int depth, Branch *branch)
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
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = startt;
    endthickness = endt;
    length = dist(start, end);
    addleaves(depth*MAX_LEAVES);
    elastic_modulus = E_MODULUS;

}

void Branch :: addleaves(int leavesmax)
{
    Leaf leaf;
    leaves.insert(leaves.begin(), leavesmax, leaf);
    for(int i=0; i<leavesmax; i++)
    {
        // leaves.push_back(leaf);
        leaves[i].set(length);
    }
}

void Branch :: wind_listener(Wind wind, double program_time)
{
    srand(time(NULL));
    double xload = wind.force_at(program_time, 0, end_points.first[0]);// * (1 + (rand()*1.0 / RAND_MAX));
    double zload = wind.force_at(program_time, 2, end_points.first[2]);// * (1 + (rand()*1.0 / RAND_MAX));
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
        dx = 0;
		#ifdef VERBOSE2
			cout << "[ERROR] |dx| > length" << endl;
		#endif
    }
    if(abs(dz) >= length)
    {
        dz = 0;
		#ifdef VERBOSE2
			cout << "[ERROR] |dz| > length" << endl;
		#endif
    }
    bent_angle[0] += asin(dx/length);
    bent_angle[2] += asin(dz/length);
		
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

    for(int i = 0; i < 3; i++)
    {
        if(abs((long) bent_angle[i]) > max_bent_angle[i]*M_PI/180)
        {
            bent_angle[i] = (bent_angle[i]/abs((long) bent_angle[i])) * max_bent_angle[i] * M_PI/180.0;
        }
    }
}

void Branch :: paint()
{
    double xn = end_points.second[0]-end_points.first[0], yn = end_points.second[1]-end_points.first[1], zn = end_points.second[2]-end_points.first[2];
    double angle = acos(yn/sqrt(xn*xn+yn*yn+zn*zn)) * 180/M_PI;

    glPushMatrix();
        glTranslated(end_points.first[0], end_points.first[1], end_points.first[2]);
        glRotated(-angle, (-length*zn), 0, length*xn);
        glRotated(-90, 1, 0, 0);

        Leaf::setMaterial();

		if(RENDER_LEAVES)
			for(int i=0; i<leaves.size(); i++)
			{
				leaves[i].paint();
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
