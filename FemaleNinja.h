#pragma once
#include "Enemy.h"
class FemaleNinja :
	public Enemy
{
	float m_fStealthDuration;
	float m_fStealthCD = 2.0f;
	float m_fTeleDist;
	float m_fTeleCD;
	float m_fTeleCastTime;
	float m_fDiceRoller = 0.0f;
	int dice = 0;
	float m_fMSBonus = 1.0f;
	bool m_bStealth = false;
public:
	FemaleNinja();
	virtual ~FemaleNinja();
	virtual DamageAttributes	CalculateDamage(void)override;
	virtual void Render(void)override;
	virtual void Update(float elapsedTime, float updateSpeed)override;
	virtual void LoadStats(std::string StatsPath);
	virtual void LoadEntityStatsFromXML() override;
	virtual int	 GetType(void)const	override	{ return ENT_NINJA; }
	void SpecialAnimationCase( ) override;

	virtual void	HandleEvent(const SGD::Event* pEvent)	override;

};


