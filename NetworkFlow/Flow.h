#pragma once
#include <ostream>
#include <vector>
#include "Graaf.h"

using namespace std;

template <class T>
//Een Pad is een reeks van knoopnummers.
class Pad: public vector<int>
{
public:
	T geefCapaciteit() const
	{
		return capaciteit;
	}

	void zetCapaciteit(const T& _capaciteit)
	{
		capaciteit = _capaciteit;
	}

	friend ostream& operator<<(ostream& os, const Pad<T>& p)
	{
		os << "Capaciteit= " << p.capaciteit << " :: ";
		if (p.size() > 0)
		{
			os << p[0];
		}
		for (int i = 1; i < p.size(); i++)
		{
			os << "->" << p[i];
		}
		os << "\n";
		return os;
	}

protected:
	T capaciteit;
};


template <class T>
class Vergrotendpadzoeker
{
public:
	Vergrotendpadzoeker(const GraafMetTakdata<GERICHT, T>& _gr, int _van, int _naar, Pad<T>& _pad);
private:
	void verwerk(int knoopnr, int diepte);
protected:
	const GraafMetTakdata<GERICHT, T>& gr;
	Pad<T>& pad;
	vector<int> voorganger;
	vector<bool> bezocht;
	int van, naar;
};

template <class T>
Vergrotendpadzoeker<T>::Vergrotendpadzoeker(const GraafMetTakdata<GERICHT, T>& _gr, int _van, int _naar, Pad<T>& _pad):
	gr(_gr), pad(_pad), voorganger(gr.aantalKnopen()), bezocht(gr.aantalKnopen(), false),
	van(_van), naar(_naar)
{
	pad.clear();
	verwerk(van, 0);

	if (pad.size() > 0)
	{
		T capaciteit = *gr.geefTakdata(pad[0], pad[1]);
		for (int i = 2; i < pad.size(); i++)
		{
			T nucapaciteit = *gr.geefTakdata(pad[i - 1], pad[i]);
			if (nucapaciteit < capaciteit)
				capaciteit = nucapaciteit;
		}
		pad.zetCapaciteit(capaciteit);
	}
}

template <class T>
void Vergrotendpadzoeker<T>::verwerk(int knoopnr, int diepte)
{
	bezocht[knoopnr] = true;
	const typename GraafMetTakdata<GERICHT, T>::Knoop& kn = gr[knoopnr];
	int nudiepte = diepte + 1;
	for (auto it = kn.begin(); it != kn.end(); it++)
	{
		int kind = it->first;
		if (*gr.geefTakdata(knoopnr, kind) > 0)
		{
			if (it->first == naar && nudiepte + 1 >= pad.size())
			{
				voorganger[naar] = knoopnr;
				pad.resize(nudiepte + 1);
				int nunr = naar;
				int i = nudiepte;
				while (nunr != van)
				{
					pad[i--] = nunr;
					nunr = voorganger[nunr];
				}
				pad[0] = nunr;
			}
			else if (it->first != naar && !bezocht[kind])
			{
				voorganger[kind] = knoopnr;
				verwerk(kind, nudiepte);
			}
		}
	}
}

template <class T>//T = takdata
class Stroomnetwerk :public GraafMetTakdata<GERICHT, T>
{
public:
	//Copyconstructor. Let op: Graaf<GERICHT>(gr) moet toegevoegd,
	//anders roept de copyconstructor van GraafMetTakdata de defaultconstructor
	//van Graaf op en krijgen we een lege graaf.
	Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr) :
		Graaf<GERICHT>(gr), GraafMetTakdata<GERICHT, T>(gr)
	{
	};


	Stroomnetwerk(const GraafMetTakdata<GERICHT, T>& gr, int _van, int _naar) :
		Graaf<GERICHT>(gr.aantalKnopen()), van(_van), naar(_naar)
	{
		Stroomnetwerk<T> restnetwerk(gr);
		Pad<T> vergrotendpad;
		Vergrotendpadzoeker<T> vg(restnetwerk, van, naar, vergrotendpad);
		while (vergrotendpad.size() != 0)
		{
			this->cap += vergrotendpad.geefCapaciteit();
			restnetwerk -= vergrotendpad;
			*this += vergrotendpad;
			Vergrotendpadzoeker<T> vg(restnetwerk, van, naar, vergrotendpad);
		}
	}

	Stroomnetwerk<T> operator-=(Pad<T> pad)
	{
		//verwijderVerbinding()
		int van = pad[0];
		for (int i = 1; i < pad.size(); i++)
		{
			int naar = pad[i];
			T* data = this->geefTakdata(van, naar);
			*data -= pad.geefCapaciteit();
			//			this->verwijderVerbindingUitDatastructuur(van,naar);
			data = this->geefTakdata(naar, van);
			if (data != 0)
			{
				*data += pad.geefCapaciteit();
			}
			else
			{
				this->voegVerbindingToe(naar, van, pad.geefCapaciteit());
			}
			van = naar;
		}
		return *this;
	}

	Stroomnetwerk<T> operator+=(Pad<T> pad)
	{
		//verwijderVerbinding()
		int van = pad[0];
		for (int i = 1; i < pad.size(); i++)
		{
			int naar = pad[i];
			//takdatavector[vn]-=pad.geefCapaciteit();
			T* data = this->geefTakdata(van, naar);
			if (data != 0)
			{
				*data += pad.geefCapaciteit();
			}
			else
			{
				data = this->geefTakdata(naar, van);
				if (data != 0)
				{
					T tmp = *data - pad.geefCapaciteit();
					if (tmp < 0)
					{
						this->verwijderVerbinding(naar, van);
						this->voegVerbindingToe(van, naar, -tmp);
					}
					else
					{
						*data -= pad.geefCapaciteit();
						if (*data == 0)
						{
							this->verwijderVerbinding(naar, van);
						}
					}
				}
				else
				{
					this->voegVerbindingToe(van, naar, pad.geefCapaciteit());
				}
			}
			van = naar;
		}
		return *this;
	}

	T geefCapaciteit()
	{
		return cap;
	}

protected:
	int van, naar;
	T cap = 0;
};
