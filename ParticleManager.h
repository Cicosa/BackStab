/***************************************************************
|	File:		ParticleManager.h

|	Author:		Nicholas Jeffries
|	Course:		SGP I 1407
|	Purpose:	ParticleManager class handles the emitters, particles, and flyweights.
***************************************************************/
#pragma once
#include <string>
#include <vector>
#include "Emitter.h"
#include "EmitterFlyweight.h"
#include "ParticleFlyweight.h"
using namespace std;
class ParticleManager
{
	//static ParticleManager* s_Instance;

	ParticleFlyweight m_FlyWt;
public:
	ParticleManager();
	~ParticleManager();
	EmitterFlyweight m_emitFlyWeight;
	vector<Emitter*> m_vcEmitters;
	Emitter* SparkBurst;
	Emitter* SparkBurst2;
	Emitter* Explode;
	Emitter* Explode2;

	Emitter* Explode3;
	Emitter* Explode4;

	Emitter* Spew;
	Emitter* Spew7;
	Emitter* Spew2;
	Emitter* Spew3;
	Emitter* Spew4;
	Emitter* Spew5;
	Emitter* Spew6;

	Emitter* LoadEmitter(string _xml);
	void UnloadEmitter(Emitter* _emit);
	void Update(float _dt, float _updateSpeed);
	void Render();
	void SpecialRender();

	static ParticleManager* GetInstance();
};

