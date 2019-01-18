#include "pch.h"
#include "GOTO.h"

GOTO::GOTO()
{
}

void GOTO::SetFlag(ConditionalFlags flag)
{
	m_flags.insert(flag);
}

bool GOTO::Check(ConditionalFlags flag)
{
	return flag == True || m_flags.count(flag);
}

void GOTO::Clear()
{
	m_flags.clear();
}
