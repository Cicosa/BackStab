/***************************************************************
|	File:		ParticleFlyweight.h

|	Author:		Nicholas Jeffries
|	Course:		SGP I 1407
|	Purpose:	ParticleFlyweight class handles all of the particles values.
***************************************************************/
#pragma once
#include "..\SGD Wrappers\SGD_Declarations.h"
#include "..\SGD Wrappers\SGD_Geometry.h"
#include "..\SGD Wrappers\SGD_Color.h"
class ParticleFlyweight
{
public:
	float m_fMinLife;
	float m_fMaxLife;
	SGD::Size m_szStartsize;
	SGD::Size m_szEndsize;
	SGD::Vector m_vStartVel;
	SGD::Vector m_vEndVel;
	SGD::Color m_clrStartColor;
	SGD::Color m_clrEndColor;

	SGD::Vector m_fStartScale;
	SGD::Vector m_fEndScale;

	float m_fMinRotation;
	float m_fMaxRotation;

	float m_vStartRotationVel;
	float m_vFinalRotationVel;
	char m_cEndAlpha;
	char m_cStartAlpha;
	ParticleFlyweight();
	virtual ~ParticleFlyweight();
};

