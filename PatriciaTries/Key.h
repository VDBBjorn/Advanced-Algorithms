#ifndef __BITPATROON
#define __BITPATROON
#include <iostream>
using std::ostream;
typedef unsigned int uint;
const int patroonlengte = sizeof(uint) * 8;
class Key {
public:
	uint bits;
	Key(uint u = 0) :bits(u) {};
	//EN-operator
	const Key operator&(const Key& b) const {
		return Key(bits & b.bits);
	}
	//OF-operator
	const Key operator|(const Key& b) const {
		return Key(bits | b.bits);
	}
	//EN-=-operator
	const Key& operator&=(const Key& b) {
		bits &= b.bits;
		return *this;
	}
	//OF-=-operator
	const Key& operator|=(const Key& b) {
		bits |= b.bits;
		return *this;
	}

	bool operator==(const Key& s2) {
		return this->bits == s2.bits;
	}
	//logische operatoren
	bool en(const Key& b) const {
		return (bits & b.bits) != 0;
	}
	bool of(const Key& b) const {
		return (bits | b.bits) != 0;
	}
	//let op: shiftoperaties verplaatsen niets als verplaatsing >= patroonlengte.
	const Key shiftlinks(uint verplaatsing) const {
		return Key(bits << verplaatsing);
	}
	const Key shiftrechts(uint verplaatsing) const {
		return Key(bits >> verplaatsing);
	}
	//niet beveiligd: u moet kleiner dan patroonlengte zijn.
	static Key eenbit(uint u) {
		//vanaf meest significant, aantal keer naar rechts opschijven vanaf meest significante
		return Key(uint(1) << (patroonlengte - 1 - u));
	}
	friend ostream& operator<<(ostream& os, const Key& b) {
		for (int i = 0; i<patroonlengte; i++) {
			os << b.en(eenbit(i));
		}
		return os;
	}

	bool bit(int i) const {

		return bits& uint(1) << (i - 1);
	}

	int verschilbit(const Key& s2) {
		int teller = patroonlengte;

		while (bit(teller) == s2.bit(teller) && teller >= 0) {
			//	cout<<teller<<" libnks "<<bit(teller) <<"rechts "<<s2.bit(teller)<<endl;
			teller--;
		}

		return teller;

	}
};
#endif
