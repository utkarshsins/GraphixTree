#include "leaf.h"

const float Leaf::mat_ambient[]    = { 0.0f, 1.0f, 0.0f, 1.0f };
const float Leaf::mat_diffuse[]    = { 0.0f, 1.0f, 0.0f, 1.0f };
const float Leaf::mat_specular[]   = { 0.0f, 1.0f, 0.0f, 1.0f };
const float Leaf::high_shininess[] = { 100.0f };
bool Leaf::render_leaves = RENDER_LEAVES;

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
       glVertex3f(1,0,1.2);
       glVertex3f(0,0,2);
       glVertex3f(-1,0,1.2);
    glEnd();
    glPopMatrix();
}

Leaf::Leaf()
{
    length_fraction = random(0,1);
    thetabranch = random(30,90);
    thetabranch360 = random(0,360);
	thetax = 0;
	thetay = 0;
	thetaz = 0;
}

void Leaf::set(double length, bool value)
{
    length_fraction = random(0,length);
    thetabranch = random(30,90);
    thetabranch360 = random(0,360);
	onefbeta = BetaNoise::librarynoise(ONEFBETA_2POW, ONEFBETA_BETA);

	#ifdef DEBUG_LEAF_BETA
	if(value)
	{
		int num = onefbeta.size();
		for(int i=0; i<num; i++)
			std::cout << onefbeta[i] << std::endl;
			std::cout	<< "======================================================================================="
						<< std::endl;
	}
	#endif

}

#ifdef DEBUG_SINGLE_LEAF
void Leaf::paint(double val, bool show)
#else
void Leaf::paint(double val)
#endif
{
	val = val * (onefbeta.size()-1);
	double linearinterpol = onefbeta[floor(val)] * (1.0 - fmod(val, 1.0)) + onefbeta[ceil(val)] * fmod(val, 1.0);
	thetax = LEAF_ROTATION_LIMIT_X * linearinterpol;
	thetay = LEAF_ROTATION_LIMIT_Y * linearinterpol;
	thetaz = thetax * LEAF_ROTATION_X_Z_FACTOR + LEAF_ROTATION_LIMIT_Z * linearinterpol;

	#ifdef DEBUG_SINGLE_LEAF
	if(show)
		std::cout	<< "SCVAL : " << val
					<< "\t\t\t" << linearinterpol
					<< "\t\t\t" << thetax
					<< "\t\t\t" << thetay
					<< "\t\t\t" << thetaz
					<< std::endl;
	#endif

    glPushMatrix();
        glTranslated(0,0,length_fraction);
        glRotated(thetabranch360,0,0,1);
        glRotated(thetabranch,1,0,0);

		glRotated(thetax,1,0,0);
		glRotated(thetay,0,1,0);
		glRotated(thetaz,0,0,1);

        glPushMatrix();
            glTranslatef(0,0,LEAF_PETIOLE);
            drawLeaf();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0,LEAF_PETIOLE*2.0/3.0);
            glRotatef(-LEAF_ANGLE, 0, 1, 0);
            drawLeaf();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(0,0,LEAF_PETIOLE/3.0);
            glRotatef(LEAF_ANGLE, 0, 1, 0);
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

