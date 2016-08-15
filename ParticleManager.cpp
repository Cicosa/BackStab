/***************************************************************
|	File:		ParticleManager.cpp

|	Author:		Nicholas Jeffries
|	Course:		SGP I 1407
|	Purpose:	ParticleManager class handles the emitters, particles, and flyweights.
***************************************************************/

#include "ParticleManager.h"
#include "../TinyXML/tinyxml.h"
#include "../SGD Wrappers\SGD_GraphicsManager.h"
#include "Entity.h"

ParticleManager::ParticleManager()
{
	
}

ParticleManager::~ParticleManager()
{
	for (unsigned int i = 0; i < m_vcEmitters.size(); i++)
	{
		m_vcEmitters[i]->KillParticles();
		delete m_vcEmitters[i];
	}

}

ParticleManager* ParticleManager::GetInstance(void)
{
	static ParticleManager s_Instance;	// stored in global memory once
	return &s_Instance;
}

void ParticleManager::UnloadEmitter(Emitter* _emit)
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(_emit->m_hTexture);
	_emit->active = false;
	/*for (unsigned int  i = 0; i < m_vcEmitters.size(); i++)
	{
		if (m_vcEmitters[i] == _emit)
		{
			_emit->KillParticles();
			m_vcEmitters.erase(m_vcEmitters.begin() + i);
			break;
		}
	}*/
	//m_vcEmitters.clear();
}
Emitter* ParticleManager::LoadEmitter(string _xml)
{
	TiXmlDocument xDoc;
	if (!xDoc.LoadFile(_xml.c_str()))
		return false;
	TiXmlElement *xPartManager = xDoc.RootElement();
	if (xPartManager)
	{
		Emitter* tempEmit = new Emitter();
		double tempDoub;
		TiXmlElement* xEmitFly = xPartManager->FirstChildElement("EmitterFlyweight");
		xEmitFly->Attribute("spawnrate", &tempDoub);
		m_emitFlyWeight.m_fSpawnRate = (float)tempDoub;
		xEmitFly->Attribute("NumOfParticles", &m_emitFlyWeight.m_nNumParticles);
		xEmitFly->Attribute("shape", &m_emitFlyWeight.m_nShape);
		TiXmlElement* xEmit = xEmitFly->FirstChildElement("Emitter");
		int tempInt;
		tempEmit->particleCap = m_emitFlyWeight.m_nNumParticles;
		xEmit->Attribute("IsLooping", &tempInt);
		if (tempInt == 0)
			tempEmit->m_bIsLooping = false;
		else
			tempEmit->m_bIsLooping = true;
		xEmit->Attribute("x", &tempDoub);

		tempEmit->m_ptPos.x = (float)tempDoub;
		xEmit->Attribute("y", &tempDoub);
		tempEmit->m_ptPos.y = (float)tempDoub;
		tempEmit->imgPath = xEmit->Attribute("texture");
		if (strcmp(tempEmit->imgPath.c_str(), "None"))
		{
			string tempstring = "resource/graphics/";
			tempstring += tempEmit->imgPath;
			tempEmit->imgPath = tempstring;
		}
		TiXmlElement* xFly = xEmit->FirstChildElement("ParticleFlyWeight");
		xFly->Attribute("MinLife", &tempDoub);
		m_FlyWt.m_fMinLife = (float)tempDoub;
		xFly->Attribute("MaxLife", &tempDoub);
		m_FlyWt.m_fMaxLife = (float)tempDoub;

		xFly->Attribute("StartWidth", &tempDoub);
		m_FlyWt.m_szStartsize.width = (float)tempDoub;
		xFly->Attribute("StartHeight", &tempDoub);
		m_FlyWt.m_szStartsize.height = (float)tempDoub;

		xFly->Attribute("EndWidth", &tempDoub);
		m_FlyWt.m_szEndsize.width = (float)tempDoub;
		xFly->Attribute("EndHeight", &tempDoub);
		m_FlyWt.m_szEndsize.height = (float)tempDoub;

		xFly->Attribute("startVX", &tempDoub);
		m_FlyWt.m_vStartVel.x = (float)tempDoub;
		xFly->Attribute("startVY", &tempDoub);
		m_FlyWt.m_vStartVel.y = (float)tempDoub;

		xFly->Attribute("endVX", &tempDoub);
		m_FlyWt.m_vEndVel.x = (float)tempDoub;
		xFly->Attribute("endVY", &tempDoub);
		m_FlyWt.m_vEndVel.y = (float)tempDoub;

		xFly->Attribute("sRed", &tempInt);
		m_FlyWt.m_clrStartColor.red = (char)tempInt;
		xFly->Attribute("sGreen", &tempInt);
		m_FlyWt.m_clrStartColor.green = (char)tempInt;
		xFly->Attribute("sBlue", &tempInt);
		m_FlyWt.m_clrStartColor.blue = (char)tempInt;

		xFly->Attribute("eRed", &tempInt);
		m_FlyWt.m_clrEndColor.red = (char)tempInt;
		xFly->Attribute("eGreen", &tempInt);
		m_FlyWt.m_clrEndColor.blue = (char)tempInt;
		xFly->Attribute("eBlue", &tempInt);
		m_FlyWt.m_clrEndColor.green = (char)tempInt;

		xFly->Attribute("sScaleX", &tempDoub);
		m_FlyWt.m_fStartScale.x = (float)tempDoub;
		xFly->Attribute("sScaleY", &tempDoub);
		m_FlyWt.m_fStartScale.y = (float)tempDoub;

		xFly->Attribute("eScaleX", &tempDoub);
		m_FlyWt.m_fEndScale.x = (float)tempDoub;
		xFly->Attribute("eScaleY", &tempDoub);
		m_FlyWt.m_fEndScale.y = (float)tempDoub;

		xFly->Attribute("RotMin", &tempDoub);
		m_FlyWt.m_fMinRotation = (float)tempDoub;

		xFly->Attribute("RotMax", &tempDoub);
		m_FlyWt.m_fMaxRotation = (float)tempDoub;

		//xFly->Attribute("RotVStart", &tempDoub);
		//m_FlyWt.m_vStartRotationVel = (float)tempDoub;

		//xFly->Attribute("RotVEnd", &tempDoub);
		//m_FlyWt.m_vFinalRotationVel = (float)tempDoub;

		xFly->Attribute("AlphStart", &tempInt);
		m_FlyWt.m_clrStartColor.alpha = (char)tempInt;
		xFly->Attribute("AlphEnd", &tempInt);
		m_FlyWt.m_clrEndColor.alpha = (char)tempInt;
		xFly->Attribute("Gravity", &tempInt);
		if (tempInt == 1)
			tempEmit->m_bGravity = true;
		else
			tempEmit->m_bGravity = false;
		tempEmit->m_FlyWt = m_FlyWt;
		tempEmit->spawnRate = m_emitFlyWeight.m_fSpawnRate;
		tempEmit->m_FlyWt.m_szStartsize.height = m_FlyWt.m_szStartsize.height * m_FlyWt.m_fStartScale.y;
		tempEmit->m_FlyWt.m_szStartsize.width = m_FlyWt.m_szStartsize.width * m_FlyWt.m_fStartScale.x;
		if (m_vcEmitters.size() == 0)
			tempEmit->emitterIndex = 0;
		else
			tempEmit->emitterIndex = m_vcEmitters.size() - 1;
		m_vcEmitters.push_back(tempEmit);
		return tempEmit;
	}
	return nullptr;
}
void ParticleManager::Update(float _dt, float _updateSpeed)
{
	for (unsigned int i = 0; i < m_vcEmitters.size(); i++)
	{	
		if (m_vcEmitters[i]->active)
		{
			m_vcEmitters[i]->Update(_dt, _updateSpeed);
			if (m_vcEmitters[i]->target == nullptr)
				continue;
			else
				if (m_vcEmitters[i]->target->IsDead())
					m_vcEmitters[i]->target = nullptr;
		}
	}
}
void ParticleManager::Render()
{
	for (unsigned int i = 0; i < m_vcEmitters.size(); i++)
	{
		if (m_vcEmitters[i]->active && m_vcEmitters[i]->special == false)
			m_vcEmitters[i]->Render();
	}
}
void ParticleManager::SpecialRender()
{
	for (unsigned int i = 0; i < m_vcEmitters.size(); i++)
	{
		if (m_vcEmitters[i]->active && m_vcEmitters[i]->special)
			m_vcEmitters[i]->Render();
	}
}

