/*
* File:   zoekboom11.h
* Author: Jelle De Bock
*
* Created on October 5, 2016, 6:32 PM
*/

#ifndef __ZOEKBOOM_H
#define __ZOEKBOOM_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include <stack>
#include <cmath>
using namespace std;

/**********************************************************************

Class: Zoekboom

beschrijving: Binaire zoekboom waarin geen duplicaatsleutels zijn toegestaan.

***************************************************************************/

template <class Sleutel, class Data>
class Zoekknoop;

template <class Sleutel, class Data>
class Zoekboom : public unique_ptr<Zoekknoop<Sleutel, Data>> {
	friend ostream& operator<<(ostream& out, const Zoekboom<Sleutel, Data>& boom) {
		std::queue<Zoekknoop<Sleutel, Data>*> te_behandelen;
		te_behandelen.push(boom.get());
		Zoekknoop<Sleutel, Data> * knoop = te_behandelen.front();
		while (knoop) {
			te_behandelen.pop();
			out << *knoop;

			if (knoop->getLinks()) {
				te_behandelen.push(knoop->getLinks());
			}
			if (knoop->getRechts()) {
				te_behandelen.push(knoop->getRechts());
			}

			knoop = te_behandelen.front();
		}
		return out;
	}
public:
	void voegtoe(const Sleutel& sleutel, const Data& data);
	Zoekboom() { this->release(); };
	Zoekboom(Zoekknoop<Sleutel, Data>* knoop) {
		cout << "Adding new knoop to search tree..." << endl;
		this->reset(std::move(knoop));
	};
	int getDiepte() {
		return this->get()->diepte();
	};
	int getGemiddeldeDiepte() {
		return (int)(this->get()->somDiepte() / this->get()->aantalKnopen());
	};
	int getSomDiepte() {
		return this->get()->somDiepte();
	};
	int getAantalKnopen() {
		return this->get()->aantalKnopen();
	};
	void roteer(bool links) {
		Zoekboom<Sleutel, Data> boom;

		//als je naar links moet roteren
		if (links) {
			/*In a binary search tree, a left rotation is the movement of a node, X, down to the left. This rotation assumes that X has a right child (or subtree).
			X's right child, R, becomes X's parent node and R's left child becomes X's new right child.
			This rotation is done to balance the tree; specifically when the right subtree of node X has a significantly (depends on the type of tree) greather height than its left subtree.*/

			boom = move((*this)->rechts); //zet rechterkind in placeholder
			(*this)->rechts = move(boom->links); //linkerkind wordt het rechterkind van onze huidige knoop, maar ouder moet nog worden juistgezet

			if ((*this)->rechts != nullptr) {
				(*this)->rechts->ouder = (*this).get(); // als je een rechterdeelboom hebt wordt zijn ouder onze huidige knoop
			}

			boom->ouder = (*this)->ouder; //De ouder van de placeholder wordt de ouder van onze huidige knoop
			(*this)->ouder = (*this)->links->ouder; //ouder van onze huidige knoop wordt de ouder van de linkerdeelboom (niet zeker of dit niet overbodig is)

			boom->links = move(*this); //zet de huidige knoop in de linkerdeelboom van de placeholder
			boom->links->ouder = boom.get(); //de ouder van de linkerdeelboom is de placeholder

		}
		else {
			//Zie links voor uitleg, maar hier gaan we omgekeerd tewerk
			boom = move((*this)->links);
			(*this)->links = move(boom->rechts);
			if ((*this)->links != nullptr)
				(*this)->links->ouder = (*this).get();

			boom->ouder = (*this)->ouder;

			boom->rechts = move(*this);
			boom->rechts->ouder = boom.get();
		}

		*this = move(boom);
	};
	Zoekboom<Sleutel, Data>& operator=(Zoekboom<Sleutel, Data>&& other) noexcept
	{
		if (this != &other) {
			this->reset(other.release());
		}
		return *this;
	}

	double potentiaal() {
		stack<Zoekboom<Sleutel, Data>*> toVisit;
		toVisit.push(this);
		double pot = 0;
		Zoekboom<Sleutel, Data> *cur;
		while (!toVisit.empty()) {
			cur = toVisit.top(); toVisit.pop();
			if ((*cur)->links.get()) toVisit.push(&((*cur)->links));
			if ((*cur)->rechts.get()) toVisit.push(&((*cur)->rechts));
			pot += log2(cur->getSomDiepte());
		}
		return pot;
	};
protected:
	//zoekfunctie zoekt sl en geeft de boom in waaronder de sleutel zit (eventueel een lege boom als de sleutel
	//ontbreekt) en de pointer naar de ouder (als die bestaat, anders nulpointer).
	//noot: alhoewel de functie niets verandert aan de boom is ze geen const functie.
	void zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats);
};
template <class Sleutel, class Data>
class Zoekknoop {
	friend class Zoekboom<Sleutel, Data>;
	friend ostream& operator<<(ostream& out, const Zoekknoop<Sleutel, Data>& knoop) {
		out << "KnoopId: " << &knoop << endl;
		out << "Sleutel: " << knoop.sleutel << endl;
		out << "Data: " << knoop.data << endl;
		out << "Links: " << knoop.links.get()/*<<"(sleutel:"<<(knoop.links && knoop.links.get()?""+knoop.links.get()->sleutel:"geen")<<")"*/ << endl;
		out << "Rechts: " << knoop.rechts.get()/*<<"(sleutel:"<<(knoop.rechts && knoop.rechts.get()?""+knoop.rechts.get()->sleutel:"geen")<<")"*/ << endl;
		out << "-----------------------------------" << endl;
		return out;
	};
protected:
	Sleutel sleutel;
	Data data;
	Zoekknoop<Sleutel, Data>* ouder;
	Zoekboom<Sleutel, Data> links, rechts;
	Zoekknoop() :ouder(0) {}
	Zoekknoop(Sleutel sl, Data data) :sleutel(sl), data(data) {};
public:
	Zoekknoop<Sleutel, Data>* getLinks() { return links.get(); };
	Zoekknoop<Sleutel, Data>* getRechts() { return rechts.get(); };
	int getLinkerDiepte() {
		//linker diepte
		int linker_diepte = 0;
		if (this->links.get()) {
			linker_diepte = 1 + this->links.get()->diepte();
		}
		return linker_diepte;
	}
	int getRechterDiepte() {
		int rechter_diepte = 0;
		if (this->rechts.get()) {
			rechter_diepte = 1 + this->rechts.get()->diepte();
		}
		return rechter_diepte;
	}

	int aantalLinks() {
		int aantal_links = 0;
		if (this->links.get()) {
			aantal_links += this->links.get()->aantalKnopen();
		}
		return aantal_links;

	}
	int aantalRechts() {
		int aantal_rechts = 0;
		if (this->rechts.get()) {
			aantal_rechts += this->rechts.get()->aantalKnopen();
		}
		return aantal_rechts;

	}
	int diepte() {
		//linker diepte
		int linker_diepte = getLinkerDiepte(), rechter_diepte = getRechterDiepte();
		return (linker_diepte>rechter_diepte ? linker_diepte : rechter_diepte);
	}
	int getLinkerSomDiepte() {
		int aantal_links = 0;
		if (this->links.get()) {
			aantal_links = 1 + this->links.get()->somDiepte();
		}
		return aantal_links;
	};

	int getRechterSomDiepte() {
		int aantal_rechts = 0;
		if (this->rechts.get()) {
			aantal_rechts = 1 + this->rechts.get()->somDiepte();
		}
		return aantal_rechts;

	};
	int somDiepte() {
		int som_diepte_links = (this->links.get() ? this->links.get()->somDiepte() : 0);
		int som_diepte_rechts = (this->rechts.get() ? this->rechts.get()->somDiepte() : 0);
		return diepte() + som_diepte_links + som_diepte_rechts;
	}

	int aantalKnopen() {
		return 1 + aantalLinks() + aantalRechts();
	}

	Zoekknoop<Sleutel, Data> getOuder() {
		return this->ouder;
	}

	Zoekboom<Sleutel, Data>* Links() {
		return &(links);
	}

	Zoekboom<Sleutel, Data>* Rechts() {
		return &(rechts);
	}

	const Sleutel& getSleutel() {
		return this->sleutel;
	}

	Data* getData() {
		return &(this->data);
	}
};

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::voegtoe(const Sleutel& sleutel, const Data& data) {
	Zoekboom<Sleutel, Data>* plaats;
	Zoekknoop<Sleutel, Data>* ouder;
	zoek(sleutel, ouder, plaats);
	if (!plaats->get()) {
		Zoekboom<Sleutel, Data> nieuw(new Zoekknoop<Sleutel, Data>(sleutel, data));
		nieuw->ouder = ouder;
		*plaats = move(nieuw);
	};

};

template <class Sleutel, class Data>
void Zoekboom<Sleutel, Data>::zoek(const Sleutel& sleutel, Zoekknoop<Sleutel, Data>*& ouder, Zoekboom<Sleutel, Data>*& plaats) {
	plaats = this;
	while (plaats->get() && (*plaats)->sleutel != sleutel) {
		ouder = plaats->get();
		if ((*plaats)->sleutel < sleutel)
			plaats = &(*plaats)->rechts;
		else
			plaats = &(*plaats)->links;
	};
};
#endif
