#ifndef __DISK
#define __DISK
#include <map>
using std::map;
typedef unsigned int blockindex;

//naamgeving gaat uit van de gebruiker: Write is een schrijfoperatie naar
//de schijf, enzovoorts
template<class T>
class Disk : private map<blockindex, T> {
public:
	//Write: voegt een T toe aan de schijf. Geeft de index terug
	blockindex Write(const T& b) {
		++indexcounter;
		//this-> is nodig (bij sommige compilers) omdat de parametrisatie
		//niet afgeleid kan worden uit de argumenten.
		this->operator[](indexcounter) = b;
		return indexcounter;
	}
	void Delete(blockindex bi) {
		this->erase(bi);
	}
	void Rewrite(const T& b, const blockindex bi) {
		this->operator[](bi) = b;
	}
	void Read(T& b, const blockindex bi) {
		b = this->operator[](bi);
	}
	//
	//  einde interface
	//
	Disk() : map<blockindex, T >(), indexcounter(666) {};
private:
	blockindex indexcounter;

};


#endif