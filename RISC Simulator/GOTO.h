#pragma once
#include <set>
enum ConditionalFlags {
	True,
	False,
	Underflow,
	Overflow,
	Greater,
	Equal,
	Less
};
class GOTO {
public:
	GOTO();
	void SetFlag(ConditionalFlags flag);
	bool Check(ConditionalFlags flag);
	void Clear();
private:
	std::set<ConditionalFlags> m_flags;
};