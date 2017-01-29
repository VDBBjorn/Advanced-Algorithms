#pragma once
#include "Subset.h"
#include "Graaf.h"
#include "Evolutor.h"

template <RichtType RT>
class GrootsteIndependentSet: Evolutor<Subset> {
	Subset& GeefGrootste(const Graaf<RT>&);
protected:
	double f(const Subset& p) override;
	void muteer(const Subset& in, Subset& uit) override;
	void kruis(const Subset& ouder1, const Subset& ouder2, Subset& kind) override;
	void init(Subset&) override;
private:
	Graaf<RT> g;
};

template <RichtType RT>
Subset& GrootsteIndependentSet<RT>::GeefGrootste(const Graaf<RT>& graaf)
{
	g = graaf;
	auto oplossing = geefOplossing();
	return oplossing;
}

template <RichtType RT>
double GrootsteIndependentSet<RT>::f(const Subset& p)
{
	//implementation
	return 0;
}

template <RichtType RT>
void GrootsteIndependentSet<RT>::muteer(const Subset& in, Subset& uit)
{

}

template <RichtType RT>
void GrootsteIndependentSet<RT>::kruis(const Subset& ouder1, const Subset& ouder2, Subset& kind)
{
	kind.kruis(ouder1, ouder2);
}

template <RichtType RT>
void GrootsteIndependentSet<RT>::init(Subset& s)
{
	s.init(s.size());
}
