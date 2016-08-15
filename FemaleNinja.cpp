#include "FemaleNinja.h"
#include "Player.h"
#include "..\TinyXML\tinyxml.h"
#include "BitmapFont.h"
#include "FemNinEditor.h"
#include "GameplayState.h"
FemaleNinja::FemaleNinja( )
{
	Listener::RegisterForEvent("RELOAD_FEMNIN");
	LoadEntityStatsFromXML( );

	Knockback = { 1, 1 };

	LoadAnimations("FemaleNinja_Idle",
		"FemaleNinja_Walk",
		"FemaleNinja_Death",
		"FemaleNinja_Jump",
		"", "",
		"FemaleNinja_Attack");
	m_fTeleDist = 300;
	TS.m_pOwner = this;
}

void FemaleNinja::SpecialAnimationCase( )
{
	if ( TS.m_CurrentAnimation != m_Idle_Anim )
	{
		m_Idle_Anim = "FemaleNinja_Idle";
	}
	if ( TS.m_CurrentAnimation != m_Attack_Anim )
	{
		m_Attack_Anim = "FemaleNinja_Attack";
	}
	if ( TS.m_CurrentAnimation == m_Attack_Anim && m_pTarget && !m_bAttacking && !m_bDead )
	{
		TS.m_CurrentAnimation = m_Idle_Anim;
	}
}

FemaleNinja::~FemaleNinja( )
{
}

DamageAttributes FemaleNinja::CalculateDamage(void)
{
	DamageAttributes Damage;
	Damage.m_nHitBy = this->GetType();

	if (m_nMinDamage != m_nMaxDamage)
		Damage.m_fDamage = float(rand() % (m_nMaxDamage - m_nMinDamage) + m_nMinDamage);
	else
		Damage.m_fDamage = float(m_nMinDamage);

	Damage.m_bFacingRight = m_bFacingRight;
	Damage.m_vKnockBack = Knockback;
	if (m_bStealth)
	{
		m_fStealthDuration = 0.0f;

		m_bStealth = false;
		m_fStealthCD = 6.0f;

		Damage.m_bIgnoreDefense = true;
		Damage.m_bHitByCritical = true;
		Damage.m_bStealthed = true;
	}
	else
	if (rand() % 100 < m_nCriticalChance)
	{
		Damage.m_fDamage *= 2.0f;
		Damage.m_bHitByCritical = true;
	}
	else
		Damage.m_bHitByCritical = false;

	return Damage;
}
void FemaleNinja::LoadEntityStatsFromXML( )
{
	std::string Filepath;
	if (FemNinEditor::GetInstance()->GetWasFemNinEditted() == true)
	{
		Filepath = Game::GetInstance()->GetPath();
		Filepath += "EdittedEnemyFem.xml";
	}
	else
		Filepath = "resource/xml/Characters/EnemyFem.xml";

	TiXmlDocument xDoc;
	if (!xDoc.LoadFile(Filepath.c_str()))
		return;
	TiXmlElement *xFemFile = xDoc.RootElement( );
	if ( xFemFile )
	{
		// Varables for this enemy come from Living Object 
		//		(maxHealth, CurrHealth, MinDmg, MaxDmg, Defense, CritChance, attackspeed)

		double TempAtkSpd, TempAggRan, TempHealth, TempMaxHealth, TempMSX, TempMSY;
		TiXmlElement* xStats = xFemFile->FirstChildElement("Stats");
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
	}

}
void FemaleNinja::Render(void)
{
	SGD::Point m_ptEntityOffset = m_ptPosition;
	SGD::Point CamPos = GameplayState::GetInstance( )->GetCameraPos( );
	m_ptEntityOffset.x -= CamPos.x;
	m_ptEntityOffset.y -= CamPos.y;
	SGD::Color RenderColor = { };

	if ( m_bStealth )
		RenderColor = { 127, 255, 255, 255 };
	const BitmapFont* pFont = Game::GetInstance( )->GetFont( );

	SGD::OStringStream Health;
	float m_fHealthPercent = (m_fHealth / m_fMaxHealth) * 100.0f;

	unsigned char red = unsigned char(255 * (m_fHealthPercent > 50 ? 1 - 2 * (m_fHealthPercent - 50) / 100.0f : 1.0f));
	unsigned char green = unsigned char(255 * (m_fHealthPercent > 50 ? 1.0f : 2 * m_fHealthPercent / 100.0f));

	SGD::Color HealthColor = { red, green, 0 };
	Health << std::fixed << std::setprecision(0) << m_fHealth << "/" << m_fMaxHealth;

	if (!m_bStealth)
		pFont->Draw(Health.str().c_str(), { m_ptEntityOffset.x - 32, m_ptEntityOffset.y - 64 }, 0.5f, HealthColor);

#if COLLISIONBOXES
	Entity::Render( );
#endif
	AnimationManager::GetInstance( )->Render(m_ptPosition, m_szSize,
		TS, !m_bFacingRight, RenderColor);

	LivingObject::Render( );
}
void FemaleNinja::Update(float elapsedTime, float updateSpeed)
{
	//if (OffScreen())
	//	return;
	if (m_fTeleCD > 0)
		m_fTeleCD -= elapsedTime * updateSpeed;
	LivingObject::Update(elapsedTime, updateSpeed);
	if ( m_fStealthCD > 0.0f )
		m_fStealthCD -= elapsedTime * updateSpeed;
	
	if ( m_fStealthDuration > 0.0f )
		m_fStealthDuration -= elapsedTime * updateSpeed;
	if ( m_fStealthDuration < 0.0f )
	{
		m_fStealthCD = 6.0f;
		m_bStealth = false;
		m_fStealthDuration = 0.0f;
	}

	if ( m_pTarget && !m_bDead && !m_bInvuln && !( dynamic_cast<Player*>( m_pTarget )->IsStealthed( ) ) )
	{
		float distance = fabs(m_ptPosition.x - m_pTarget->GetPosition( ).x);
		if ( m_fDiceRoller <= 0.0f )
		{
			dice = rand( ) % 10 + 1;
			m_fDiceRoller = 1.0f;
		}
		if ( m_pTarget->GetPosition( ).x - m_ptPosition.x < 0 )
			m_bFacingRight = false;
		else
			m_bFacingRight = true;
		if ( distance < 80 )
		{


			//SWING!!
			//if (!m_bAttacking && !m_bCasting && WWCountDown <= 0.0f)
			//{
			//if ( <= 0.0f && dice >= 8)
			//{
			//teleport code
			//}
			if ( m_fStealthCD < 0.0f )
			{
				//activate stealth
				m_bStealth = true;
				m_fStealthDuration = 4.0f;
				m_fMSBonus = 1.25f;
				m_fStealthCD = 0.0f;
			}
			m_bWalking = false;
			m_vtVelocity.x *= 1.0f - ( elapsedTime * updateSpeed * 10.0f );
			m_fMovementHeldDuration = 0.0f;
		}

		else
		{
			if (m_pTarget->GetPosition().y < m_ptPosition.y - 40 && m_bGrounded)
			{
				m_vtVelocity.y = -MoveSpeedY;
			
			}
			else if (m_pTarget->GetPosition().y < m_ptPosition.y - 40 && m_bWallSliding)
			{
				m_vtVelocity.y = -MoveSpeedY;
			}
			else if (m_pTarget->GetPosition().y < m_ptPosition.y - 40 && !m_bGrounded && m_vtVelocity.y > 0)
			{
				if (m_pTarget->GetPosition().y < m_ptPosition.y - 40 && m_fTeleCD <= 0.0f)
				{
					m_ptPosition.y -= m_fTeleDist * .8f;
					if (fabs(m_pTarget->GetPosition().x - m_ptPosition.x) > 200)
					{
						if (m_pTarget->GetPosition().x > m_ptPosition.x + 40)
							m_ptPosition.x += m_fTeleDist * .8f;
						else
							m_ptPosition.x -= m_fTeleDist * .8f;
					}
					m_fTeleCD = 3.0f;
				}
			}
			if (m_pTarget->GetPosition().x > m_ptPosition.x + 40 && m_bGrounded && m_fTeleCD <= 0)
			{
				float distAnts = fabs(m_pTarget->GetPosition().x - m_ptPosition.x);
				if (distAnts < m_fTeleDist)
					m_ptPosition.x += distAnts - 40;
				else
					m_ptPosition.x += m_fTeleDist;
				m_fTeleCD = 3.00f;

			}
			else if (m_pTarget->GetPosition().x < m_ptPosition.x && m_bGrounded && m_fTeleCD <= 0)
			{
				float distAnts = fabs(m_pTarget->GetPosition().x - m_ptPosition.x);
				if (distAnts < m_fTeleDist)
					m_ptPosition.x -= distAnts;
				else
					m_ptPosition.x -= m_fTeleDist;
				m_fTeleCD = 3.00f;
			}
			/*	if (m_fTeleCD <= 0.0f)
				{
				if (m_pTarget != nullptr)
				{
				if (!reinterpret_cast<Player*>(m_pTarget)->IsStealthed())
				{

				SGD::Point temp;
				float distance = m_pTarget->GetPosition().x - m_ptPosition.x;
				m_fTeleCD = 8.0f;
				}
				}
				}*/
			if ( m_fStealthCD < 0.0f )
			{
				//activate stealth
				m_bStealth = true;
				m_fStealthDuration = 4.0f;
				m_fMSBonus = 1.25f;
				m_fStealthCD = 0.0f;
			}
			m_fMovementHeldDuration += elapsedTime * updateSpeed * MoveSpeedX * m_fMSBonus;
			if ( m_vtVelocity.x < -MoveSpeedX * m_fMSBonus || m_vtVelocity.x > MoveSpeedX* m_fMSBonus )
			{
				m_vtVelocity.x = 0;
				m_bWalking = false;
			}
			//Walk towards player

			if ( m_bFacingRight )
			{
				m_bWalking = true;
				m_bAttacking = false;

				m_vtVelocity.x += ( +( MoveSpeedX * m_fMSBonus * 0.05f ) * m_fMovementHeldDuration * elapsedTime * updateSpeed );
				if ( m_vtVelocity.x > +MoveSpeedX * m_fMSBonus )
					m_vtVelocity.x = +MoveSpeedX * m_fMSBonus;
			}
			else
			{
				m_vtVelocity.x += ( -( MoveSpeedX * m_fMSBonus * 0.05f ) * m_fMovementHeldDuration * elapsedTime * updateSpeed );
				if ( m_vtVelocity.x < -MoveSpeedX * m_fMSBonus )
					m_vtVelocity.x = -MoveSpeedX * m_fMSBonus;

			}
		}
	}
	else
	{


		if ( m_pTarget != nullptr && dynamic_cast<Player*>( m_pTarget )->IsStealthed( ) )
		{
			m_pTarget = nullptr;
			m_vtVelocity.x = 0.0f;
			m_bWalking = false;
			m_bAttacking = false;
			TS.m_CurrentAnimation = m_Idle_Anim;
			m_fMovementHeldDuration = 0.0f;
			m_fConfused = 1.8f;
		}

		if ( m_bDead )
		{
			m_vtVelocity.x *= 1.0f - ( elapsedTime * updateSpeed * 10.0f );
			m_fMovementHeldDuration = 0.0f;
			m_bWalking = false;
		}

		else if ( !m_pTarget && m_fConfused <= 0.0f )
		{
			m_bAttacking = false;
			//m_bWalking = true;
			Roam(elapsedTime, updateSpeed);

		}

	}
}
void FemaleNinja::LoadStats(std::string StatsPath)
{


}
void FemaleNinja::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "RELOAD_FEMNIN")
		LoadEntityStatsFromXML();
	Enemy::HandleEvent(pEvent);
}