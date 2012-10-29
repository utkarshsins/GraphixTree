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

}

Branch :: Branch(vec3 start, vec3 end, int depth)
{
    end_points = make_pair(start, end);
    startthickness = .2;
    endthickness = .2;
    length = dist(start, end);
    
    addleaves(depth*MAX_LEAVES);
}

Branch :: Branch(vec3 start, vec3 end, double startt, double endt, int depth)
{
    end_points = make_pair(start, end);
    startthickness = startt;
    endthickness = endt;
    length = dist(start, end);
    addleaves(depth*MAX_LEAVES);
}

void Branch :: set(vec3 start, vec3 end, double startt, double endt, int depth)
{
	if(VERBOSE)
	{
		std::cout << "[VECOP] Start : " ;
		start.printvec();
		std::cout << std::endl;
		std::cout << "[VECOP] End : ";
		end.printvec();
		std::cout << std::endl;
	}

    end_points = make_pair(start, end);
    startthickness = startt;
    endthickness = endt;
    length = dist(start, end);
    addleaves(depth*MAX_LEAVES);
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
        //glutSolidCube(1.0);
            //glRotated(-90, 1, 0, 0);

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
        //glTranslated(end_points.second[0], end_points.second[1], end_points.second[2]);
    glPopMatrix();
    /*
    glPushMatrix();
        glTranslated(0, 2*length, 0);
        glRotated(angle, (-length*zn), 0, length*xn);
        glTranslated(end_points.first[0], end_points.first[1] + 2*length, end_points.first[2]);
        //glRotated(90, 1, 0, 0);
        glBegin(GL_POLYGON);
            GLUquadricObj* disc = gluNewQuadric();
            gluDisk(disc, 0, thickness/2, 8, 10);
        glEnd();
    glPopMatrix();
    */
}
