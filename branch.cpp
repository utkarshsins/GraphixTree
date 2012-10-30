#include "branch.h"

const GLfloat Branch::mat_ambient[] =
    { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat Branch::mat_diffuse[] =
    { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat Branch::mat_specular[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat Branch::high_shininess[] =
    { 100.0f };

Branch::Branch()
{
    end_points = make_pair(vec3(0,0,0), vec3(0,0,0));
    //parent_branch = NULL;
    //new_end_points = make_pair(vec3(0,0,0), vec3(0,0,0));
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = .2;
    endthickness = .2;
    length = .5;
    elastic_modulus = 1;
    addleaves(0*MAX_LEAVES);
    //parent_branch = (Branch*) malloc(sizeof(Branch));

}

Branch :: Branch(vec3 start, vec3 end, int depth)
{
    end_points = make_pair(start, end);
    //parent_branch = NULL;
    //new_end_points = make_pair(start, end);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = .2;
    endthickness = .2;
    elastic_modulus = 1;
    length = dist(start, end);

    addleaves(depth*MAX_LEAVES);
    //parent_branch = (Branch*) malloc(sizeof(Branch));

}

Branch :: Branch(vec3 start, vec3 end, double startt, double endt, int depth)
{
    end_points = make_pair(start, end);
    //parent_branch = NULL;
    //new_end_points = make_pair(start, end);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = startt;
    endthickness = endt;
    length = dist(start, end);
    elastic_modulus = 1;
    addleaves(depth*MAX_LEAVES);
    //parent_branch = (Branch*) malloc(sizeof(Branch));
}

void Branch :: copy(Branch b)
{
    end_points = make_pair(b.end_points.first, b.end_points.second);
    //new_end_points = make_pair(b.first, b.second);
    bent_angle.copy(b.bent_angle);
    max_bent_angle.copy(b.max_bent_angle);
    startthickness = b.startthickness;
    endthickness = b.endthickness;
    length = b.length;
    elastic_modulus = b.elastic_modulus;

}

void Branch :: set(vec3 start, vec3 end, double startt, double endt, int depth, Branch *branch)
{
	if(VERBOSE)
	{
		std::cout << "[VECOP] Start : " ;
		start.printvec(VERBOSE);
		std::cout << std::endl;
		std::cout << "[VECOP] End : ";
		end.printvec(VERBOSE);
		std::cout << std::endl;
	}

    end_points = make_pair(start, end);
    //new_end_points = make_pair(start, end);
    bent_angle.set(0,0,0);
    max_bent_angle.set(MAX_BENT_X, MAX_BENT_Y, MAX_BENT_Z);
    startthickness = startt;
    endthickness = endt;
    length = dist(start, end);
    addleaves(depth*MAX_LEAVES);
    parent_branch = branch;
    if(VERBOSE || VERBOSE2 && parent_branch != NULL)
    {
        //cout<< "Checking parent branch in function: ";
        //parent_branch->end_points.first.printvec(VERBOSE || VERBOSE2);
        //cout << endl;
        //parent_branch->end_points.second.printvec(VERBOSE || VERBOSE2);

    }
    elastic_modulus = 1;

}

void Branch :: addleaves(int leavesmax)
{
    // leaves = new Leaf[leavesmax];
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
    //new_end_points = make_pair(parent_branch.new_end_points.second, end_points.second);
    //bent_angle = bent_angle + parent_branch.bent_angle;
    srand(time(NULL));
    double xload = wind.force_at(program_time, 0, end_points.first[0]) * (1 + (rand()*1.0 / RAND_MAX));
    double zload = wind.force_at(program_time, 2, end_points.first[2]) * (1 + (rand()*1.0 / RAND_MAX));
    double spring_constant = (elastic_modulus * endthickness * pow(startthickness-endthickness, 3) ) / (4*pow(length,3));
    spring_constant = sin(.2*program_time) + cos(.2*program_time);
    if(spring_constant == 0)
    {
        spring_constant = .01;
    }
    if(VERBOSE2 || VERBOSE)
    {
        //cout << "Spring constant " << spring_constant << endl;
    }
    double dx = xload/spring_constant;
    double dz = zload/spring_constant;
    if(absd(dx) >= length)
    {
        dx = (dx / absd(dx)) * (length/2);
    }
    if(absd(dz) >= length)
    {
        dz = (dz / absd(dz)) * (length/2);
    }
    cout<<" dx : " << dx << " dz : " << dz << " length : " << length << endl;
    bent_angle[0] = asin(dx/length);
    bent_angle[2] = asin(dz/length);
    bent_angle.printvec(1);
    cout << endl;
    if(VERBOSE || VERBOSE2 && parent_branch != NULL)
    {
        //cout<< "Checking parent branch : ";
        //parent_branch->end_points.first.printvec(VERBOSE || VERBOSE2);
        //cout << endl;
        //parent_branch->end_points.second.printvec(VERBOSE || VERBOSE2);
        //cout << "checking bent angle ";
        //bent_angle.printvec(VERBOSE || VERBOSE2);
        //cout << endl;
    }
    else if(VERBOSE || VERBOSE2 && parent_branch == NULL)
    {
        //cout<<"Parent Branch Null"<<endl;
    }
    //elastic_modulus = 1;
    //total_bent_angle = bent_angle + (parent_branch==NULL ? vec3(0,0,0) : parent_branch->total_bent_angle);
    for(int i = 0; i < 3; i++)
    {
        if(abs((long) bent_angle[i]) > max_bent_angle[i]*M_PI/180)
        {
            bent_angle[i] = (bent_angle[i]/abs((long) bent_angle[i])) * max_bent_angle[i] * M_PI/180.0;
        }
    }
}

void Branch :: paint(double time, Wind wind)
{
    //pair<vec3, vec3> new_end_points;
    //new_end_points = make_pair(end_points.first, end_points.second);
    double xn = end_points.second[0]-end_points.first[0], yn = end_points.second[1]-end_points.first[1], zn = end_points.second[2]-end_points.first[2];
    double angle = acos(yn/sqrt(xn*xn+yn*yn+zn*zn)) * 180/M_PI;
    //wind_listener(wind, time);
    glPushMatrix();
        //glTranslated((parent_branch==NULL ? 0 : parent_branch->end_points.first[0]), (parent_branch==NULL ? 0 : parent_branch->end_points.first[1]), (parent_branch==NULL ? 0 : parent_branch->end_points.first[2]));
        //glRotated(parent_branch==NULL? 0 : parent_branch->total_bent_angle[0] * 180.0/M_PI, 1, 0, 0);
        //glRotated(parent_branch==NULL? 0 : parent_branch->total_bent_angle[2] * 180.0/M_PI, 0, 0, 1);
        //glTranslated((parent_branch==NULL ? end_points.first[0] : parent_branch->end_points.second[0]-parent_branch->end_points.first[0]), (parent_branch==NULL ? end_points.first[1] : parent_branch->end_points.second[1]-parent_branch->end_points.first[1]), (parent_branch==NULL ? end_points.first[2] : parent_branch->end_points.second[2]-parent_branch->end_points.first[2]));
        glTranslated(end_points.first[0], end_points.first[1], end_points.first[2]);
        //cout << "parent_branch->end_points.second ";
        if(parent_branch != NULL)
            //parent_branch->end_points.second.printvec(VERBOSE || VERBOSE2);
        //cout << endl;
        //cout << "bent_angle ";
        bent_angle.printvec(VERBOSE || VERBOSE2);
        cout << endl;
        //glRotated(bent_angle[0] * 180.0/M_PI, 1, 0, 0);
        //glRotated(bent_angle[2] * 180.0/M_PI, 0, 0, 1);
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
				//glRotated(90, 1, 0, 0);
				glBegin(GL_POLYGON);
					GLUquadricObj* disc = gluNewQuadric();
					gluDisk(disc, 0, endthickness/2, 8, 10);
				glEnd();
			}

        glPopMatrix();
    glPopMatrix();

}
