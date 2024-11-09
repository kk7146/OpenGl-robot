#pragma once
#include "Mesh.h"

class Robot {
public:
	Mesh Head, Body, RightArm, RightHand, LeftArm, LeftHand;
	Mesh RightLeg, RightFoot, LeftLeg, LeftFoot;
	float scale;
	Robot()
	{
		scale = 100;
		printf("Loading Robot Models....\n");
		Head.readAse("s_head.ASE");
		printf("loading Head....... OK.\n");
		Body.readAse("s_body.ASE");
		printf("loading Body........OK. \n");
		RightArm. readAse("s_rightarm.ASE");
		printf("loading RightArm....OK. \n");
		RightHand.readAse("s_righthand.ASE");
		printf("loading Righthand...OK. \n");
		LeftArm.readAse("s_leftarm.ASE");
		printf("loading LeftArm.....OK. \n");
		LeftHand.readAse("s_lefthand.ASE");
		printf("loading LeftHand....OK. \n");
		RightLeg.readAse("s_rightleg.ASE");
		printf("loading RightLeg....OK.\n");
		RightFoot.readAse("s_rightfoot.ASE");
		printf("loading RightFoot...OK.\n");
		LeftLeg.readAse("s_leftleg.ASE");
		printf("loading Lefteg.....OK. \n");
		LeftFoot.readAse("s_leftfoot.ASE");
		printf("loading LeftFoot....OK. \n");
	}
	void draw() {
		glPushMatrix();
		glTranslated(0.0, 0.0, 0.0);
		Body.draw(0.5, 0.8, 0.8, scale, true);
			glPushMatrix();
				glTranslated(0.0, 0.16, 0.43);
				Head.draw(0.8, 0.7, 0.7, scale);
			glPopMatrix();
			glPushMatrix();
				glTranslated(0.27, 0.16, 0.37);
				LeftArm.draw(0.8, 0.0, 0.7, scale);
				glPushMatrix();
					glTranslated(0.05, 0.0, -0.37);
					RightHand.draw(0.8, 0.7, 0.0, scale);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				glTranslated(-0.27, 0.16, 0.37);
				RightArm.draw(0.0, 0.7, 0.7, scale);
				glPushMatrix();
					glTranslated(-0.05, 0.0, -0.37);
					RightHand.draw(0.0, 0.0, 0.7, scale);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				glTranslated(0.11, 0.16, -0.05);
				LeftLeg.draw(0.8, 0.0, 0.0, scale);
				glPushMatrix();
					glTranslated(0.0, 0.0, -0.43);
					LeftFoot.draw(0.0, 0.7, 0.0, scale);
				glPopMatrix();
			glPopMatrix();
			glPushMatrix();
				glTranslated(-0.11, 0.16, -0.05);
				RightLeg.draw(0.5, 0.5, 0.5, scale);
				glPushMatrix();
					glTranslated(0.0, 0.0, -0.43);
					RightFoot.draw(0.0, 0.0, 0.0, scale);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	}
};