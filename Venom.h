#pragma once
#include "Enemy.h"
class Venom :
	public Enemy
{

	int maxDmg, minDmg;
	float poisonTime;
	Poison myPoison;
public:
	virtual int	GetType(void)	const	override	{ return ENT_VENOM; }

	Venom();
	virtual ~Venom();
	virtual DamageAttributes	CalculateDamage(void)	override;
	virtual void HandleEvent(const SGD::Event* pEvent);

	void SpecialAnimationCase( );

protected:
	void LoadEntityStatsFromXML() override;
};

