#include "gates.h"

// gates constructed with nand gates

bool GateNand(bool a, bool b)
{
	return !(a && b);
}

bool GateNot(bool a)
{
	return GateNand(a, a);
}

bool GateAnd(bool a, bool b)
{
	return GateNot(GateNand(a, b));
}

bool GateOr(bool a, bool b)
{
	return 
		GateNot
		(
			GateAnd
			(
				GateNot(a), 
				GateNot(b)
			)
		);
}

bool GateXor(bool a, bool b)
{
	bool x = GateNand(a, b);
	bool y = GateNand(x, a);
	bool z = GateNand(x, b);
	return GateNand(y, z);
}

// blackbox gates

bool Nand(bool a, bool b)
{
	return !(a && b);
}

bool Not(bool a)
{
	return !a;
}

bool And(bool a, bool b)
{
	return a && b;
}

bool Or(bool a, bool b)
{
	return a || b;
}

bool Xor(bool a, bool b)
{
	return a ^ b;
}
