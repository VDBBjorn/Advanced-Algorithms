#include "Subset.h"
#include "Graaf.h"

template <RichtType RT>
class GrootsteIndependentSet {
	Subset& geefGrootste(const Graaf<RT>&);
};

template <RichtType RT>
Subset& GrootsteIndependentSet<RT>::geefGrootste(const Graaf<RT>&)
{

}

int main(int argc, const char * argv[])
{
	GrootsteIndependentSet<GERICHT> indset;
	return 0;
}
