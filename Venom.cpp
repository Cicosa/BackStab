#include "Venom.h"
#include "..\TinyXML\tinyxml.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "VenomEditor.h"

Venom::Venom( )
{
	Listener::RegisterForEvent("RELOAD_VENOM");
	LoadEntityStatsFromXML( );
	Knockback = { 1, 1 };

	LoadAnimations("Venom_Idle_1", "Venom_Walk", "Venom_Death", "Venom_Jump", "", "", "Venom_Attack_1");
	SpecialAnimationCase( );

	TS.m_pOwner = this;
}
void Venom::LoadEntityStatsFromXML( )
{
	std::string Filepath;
	if ( VenomEditor::GetInstance( )->GetWasVenomEditted( ) == true )
	{
		Filepath = Game::GetInstance()->GetPath();
		Filepath += "EdittedEnemyVenom.xml";
	}
	else
		Filepath = "resource/xml/Characters/EnemyVenom.xml";

	TiXmlDocument xDoc;
	if ( !xDoc.LoadFile(Filepath.c_str( )) )
		return;
	TiXmlElement *xVenomFile = xDoc.RootElement( );
	if ( xVenomFile )
	{
		// Varables for this enemy come from Living Object 
		//		(maxHealth, CurrHealth, MinDmg, MaxDmg, Defense, CritChance, attackspeed)

		double TempAtkSpd, TempAggRan, TempPoisTime, TempHealth, TempMaxHealth, TempMSX, TempMSY;
		TiXmlElement* xStats = xVenomFile->FirstChildElement("Stats");
		xStats->Attribute("Defense", &m_nDefense);
		xStats->Attribute("MaxDamage", &m_nMaxDamage);
		xStats->Attribute("MinDamage", &m_nMinDamage);
		xStats->Attribute("CriticalChance", &m_nCriticalChance);

		xStats->Attribute("MaxHealth", &TempMaxHealth);
		m_fMaxHealth = (float)TempMaxHealth;

		xStats->Attribute("CurrHealth", &TempHealth);
		m_fHealth = (float)TempHealth;

		xStats->Attribute("AttackSpeed", &TempAtkSpd);
		m_fAttackSpeed = (float)TempAtkSpd;

		xStats->Attribute("AggroRange", &TempAggRan);
		m_fAggroRange = (float)TempAggRan;

		xStats->Attribute("MoveSpeedX", &TempMSX);
		MoveSpeedX = (float)TempMSX;

		xStats->Attribute("MoveSpeedY", &TempMSY);
		MoveSpeedY = (float)TempMSY;

		xStats->Attribute("MaxPoisonDamage", &maxDmg);
		//m_Poison.m_fMinDMG = maxpoison;

		xStats->Attribute("MinPoisonDamage", &minDmg);


		xStats->Attribute("PoisonTime", &TempPoisTime);
		poisonTime = (float)TempPoisTime;
	}
}
DamageAttributes Venom::CalculateDamage(void)
{
	DamageAttributes Damage = Enemy::CalculateDamage();
	if ( m_pTarget )
	{
		Poison tempPoison;
		tempPoison.m_fMaxDMG = maxDmg;
		tempPoison.m_fMinDMG = minDmg;
		tempPoison.m_fPoisonTimer = poisonTime;

		//Damage.m_fDamage = float(rand( ) % ( m_nMaxDamage - m_nMinDamage ) + m_nMinDamage);
		//Damage.m_bFacingRight = m_bFacingRight;
		//Damage.m_vKnockBack = Knockback;
		Damage.m_Poison = tempPoison;

		myPoison = tempPoison;

		SGD::Event *newEvent = new SGD::Event("Apply_Poison", &myPoison, this);
		newEvent->QueueEvent(m_pTarget);
	}
	return Damage;
}
Venom::~Venom( )
{
}
void Venom::HandleEvent(const SGD::Event* pEvent)
{
	if ( pEvent->GetEventID( ) == "RELOAD_VENOM" )
		LoadEntityStatsFromXML( );

	Enemy::HandleEvent(pEvent);
}

void Venom::SpecialAnimationCase( )
{
	if ( TS.m_CurrentAnimation != m_Attack_Anim )
	{
		if ( rand( ) % 4 + 1 > 2 )
			m_Attack_Anim = "Venom_Attack_1";
		else
			m_Attack_Anim = "Venom_Attack_2";
	}
	if ( TS.m_CurrentAnimation != m_Idle_Anim )
	{
		if ( rand( ) % 4 + 1 > 2 )
			m_Idle_Anim = "Venom_Idle_1";
		else
			m_Idle_Anim = "Venom_Idle_2";
	}
	if ( TS.m_CurrentAnimation == m_Attack_Anim && m_pTarget && !m_bAttacking && !m_bDead )
	{
		TS.m_CurrentAnimation = m_Idle_Anim;
	}

	return;
}
