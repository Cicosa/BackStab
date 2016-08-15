/***************************************************************
|	File:		Particle.h

|	Author:		Nicholas Jeffries
|	Course:		SGP I 1407
|	Purpose:	Particle class handles the particles and all of it's values
***************************************************************/
#pragma once
#include "..\SGD Wrappers\SGD_Geometry.h"
#include "..\SGD Wrappers\SGD_Color.h"
class Particle 
{

public:
	Particle();
	virtual ~Particle();
	SGD::Point m_ptPos;
	float m_fCurLife;
	float m_fMaxLife;
	SGD::Size m_szSize;
	//vel,color,alpha,scale,rotation,rotVel
	SGD::Vector m_vcVel;
	SGD::Color m_clrColor;
	char m_cAlpha;

	SGD::Vector m_vcScale;
	float m_fRotation;
	float m_fRotVelocity;
	SGD::Size FindCurSize();
};

