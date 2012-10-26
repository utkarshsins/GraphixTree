#include "branch.h"

Branch::Branch()
{
    //ctor
}

Branch :: Branch(vec3 start, vec3 end)
{
    end_points = make_pair(start, end);
    thickness = .2;
    length = dist(start, end);
}

Branch :: Branch(vec3 start, vec3 end, double t)
{
    end_points = make_pair(start, end);
    thickness = t;
    length = dist(start, end);
}

void Branch :: set(vec3 start, vec3 end, double t)
{
    end_points = make_pair(start, end);
    thickness = t;
    length = dist(start, end);
}

void Branch :: paint()
{
    double xn = end_points.second[0]-end_points.first[0], yn = end_points.second[1]-end_points.first[1], zn = end_points.second[2]-end_points.first[2];
    double angle = acos(yn/sqrt(xn*xn+yn*yn+zn*zn)) * 180/M_PI;
    glPushMatrix();
        glTranslated(end_points.first[0], end_points.first[1], end_points.first[2]);
        glRotated(angle, (-length*zn), 0, length*xn);
        glRotated(-90, 1, 0, 0);
        glPushMatrix();
        //glutSolidCube(1.0);
            //glRotated(-90, 1, 0, 0);
            glBegin(GL_POLYGON);
                GLUquadricObj* cylinder = gluNewQuadric();
                gluCylinder(cylinder, thickness/2, thickness/2, length, 8 , 10);
            glEnd();

            glTranslated(0, 0, length);
            //glRotated(90, 1, 0, 0);
            glBegin(GL_POLYGON);
                GLUquadricObj* disc = gluNewQuadric();
                gluDisk(disc, 0, thickness/2, 8, 10);
            glEnd();

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
