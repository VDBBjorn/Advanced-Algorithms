#pragma once
#include<vector>
#include <algorithm>

template <class P>
class Evolutor
{
public:
	virtual ~Evolutor() = default;
	//opgelet: geefOplossing geeft een referentie naar tijdelijk object.
	//dit kan maar tijdelijk gebruikt worden!
	const P& geefOplossing()
	{
		std::vector<P> populatie(N);
		for (int i = 0; i < N; i++)
		{
			init(populatie[i]);
		}
		for (int i = 0; i < AANTALGENERATIES; i++)
		{
			for (int j = N / 3; j < (2 * N) / 3; j++)
			{
				muteer(populatie[rand() % N / 3], populatie[j]);
			}
			for (int j = (2 * N) / 3; j < N; j++)
			{
				kruis(populatie[rand() % (2 * N) / 3],
				      populatie[rand() % (2 * N) / 3], populatie[j]);
			}
			zetBesteVooraan(populatie, N / 3);
		}
		zetBesteVooraan(populatie, 1);
		return populatie[0];
	}

protected:
	//probleemafhankelijke functies:
	//evaluatiefunctie: hoe kleiner hoe beter!
	virtual double f(const P& p) = 0;
	virtual void muteer(const P& in, P& uit) = 0;
	virtual void kruis(const P& ouder1, const P& ouder2, P& kind) = 0;
	virtual void init(P&) = 0;
	//probleemonafhankelijk:
	//zetbesteVooraan zet de beste (op basis van f) n elementen
	//van de populatie vooraan in de vector
	virtual void zetBesteVooraan(std::vector<P>&, int n);
	//populatiegrootte. N is deelbaar door 3
	static const int N = 30;
	//aantal generaties
	static const int AANTALGENERATIES = 10;
};

template <class P>
void Swap(std::vector<P>& v, int i, int j)
{
	P temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

template <class P>
void Evolutor<P>::zetBesteVooraan(std::vector<P>& v, int n)
{
	for (int i = 0; i < n && i < v.size(); i++)
	{
		int max = i;
		for (int j = i + 1; j < v.size(); j++)
		{
			if (f(v[j]) > f(max))
			{
				max = j;
			}
		}
		Swap(v, i, max);
	}
}
