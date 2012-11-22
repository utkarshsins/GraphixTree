#include "leaf.h"
#include "Debug.h"

const float Leaf::mat_ambient[]    = { 0.0f, 1.0f, 0.0f, 1.0f };
const float Leaf::mat_diffuse[]    = { 0.0f, 1.0f, 0.0f, 1.0f };
const float Leaf::mat_specular[]   = { 0.0f, 1.0f, 0.0f, 1.0f };
const float Leaf::high_shininess[] = { 100.0f };

double random(double min, double max)
{
    return min + ((double) rand() / RAND_MAX) * (max-min);
}

void drawLeaf()
{
    glPushMatrix();
        glScalef(LEAF_SCALE, LEAF_SCALE, LEAF_SCALE);
    glBegin(GL_QUADS);
       glVertex3f(0,0,0);
       glVertex3f(0,1,1.2);
       glVertex3f(0,0,2);
       glVertex3f(0,-1,1.2);
    glEnd();
    glPopMatrix();
}

Leaf::Leaf()
{
    length_fraction = random(0,1);
    thetabranch = random(30,90);
    thetabranch360 = random(0,360);
    theta360 = random(0,360);
}

void Leaf::set(double length)
{
    length_fraction = random(0,length);
    thetabranch = random(30,90);
    thetabranch360 = random(0,360);
    theta360 = random(0,360);
		#ifdef VERBOSE
		std::cout << "[LEAFL] " << length_fraction << std::endl;
		#endif
}

void Leaf::paint()
{
    glPushMatrix();
        glTranslated(0,0,length_fraction);
        glRotated(thetabranch360,0,0,1);
        glRotated(thetabranch,1,0,0);
        glRotated(theta360,0,0,1);

        glPushMatrix();
            glTranslatef(0,0,LEAF_PETIOLE);
            drawLeaf();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0,LEAF_PETIOLE*2.0/3.0);
            glRotatef(-LEAF_ANGLE, 1, 0, 0);
            drawLeaf();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0,LEAF_PETIOLE/3.0);
            glRotatef(LEAF_ANGLE, 1, 0, 0);
            drawLeaf();
        glPopMatrix();

    glPopMatrix();
}

void Leaf::setMaterial()
{
     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
}

