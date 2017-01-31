#ifndef __SIMULANNEAL
#define __SIMULANNEAL
#include <cstdlib>
#include <memory>

//abstracte klasse voor simulated annealing
//Hier worden GEEN probleemafhankelijke functies gedefinieerd
//deze zijn allemaal virtual.
//Notaties: zie Blum and Roli
//Parameter S: De oplossing (zowel de voorlopige als de uiteindelijke)
//is een S. S staat voor Search space
#include <vector>
using std::vector;
#include <iostream>
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
using std::cout;
#include<sstream>
using std::istringstream;
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

/*
//initiële begin een goede heuristiek oplossing, probleem lokaalminimu tmperatuur niet te laag nemen
// groepen die vrijgekomen zijn FFD en BFD=> beter omgrvingen definiëren
//heuristieken van oplossingn naar oplossing springen => sa alle tijd die je hebt besteden voor beter oplossingen tevinden
//als je het naief implementeerd sorteert O(n) logn
//BFD: n groepen, één voor één in boten droppen => ergens een lus O(n) eerste groep laatste groep sorteren O(n)logn
//het vinden van BFD de best horende boot in O(logn) en die gegevens structuur updaten in O(logn) 
//je wilt de kleinst mogelijke vrije ruimte die bij u past niet de aller kleinste die nog vrij is waar je bijvoorbeeld niet bij kan
//multiset mogelijk om te gebruiken O(logn)
//zoeken,steken in multiset en u waarden er insteken hem dan gaat zoeken en de volgende nemen =>STDL multiset
//zelf zoekfunctie voor binaire bomen, lege plaats -> opvolger zoeken anders gewoon de plek geven 

DBF: groepen sorteren: ineen gegevensstructuur type A: groep is sleutel 
type B gegevensstructuur van alle Boten die niet_leeg zijn waar de groep in kan: binaire boom O(1) eruit halen maar dit niet
boot ik steek hier groep in dan moet je tegen alle groepen kleiner dan 45 zeggen van de boot is weh =>oplossing
type B waar alle boten in kan maar de vorige groep => eerder staat in de sortering kan er niet in.
type A : één verder kunnen gaan + updaten als ik een boot heb waar ik een groep insteek, de boot vinden waar de groep inzat geen probleem volgende
welke groep is de grootste groep wat er nog inkan => hier zoek je naar de grootste groep die er nog inkan
boot waar er nog 20 inkan, tegen deze groep met max 20 leden geef je de boot van je kan er nog in in deze boot
Type A efficiënte iterator en iffeciënt opzoeken, vector zelf binair zoeken
binaire boom ook zoeken je zoekt de groep die net een beetje kleiner is/nie groter dan de plek
Type B B::insert
		B::remove
		
als ik een groep heb en ik steek die in een boot, wat gebeurt er met de andere boten die ook deze groep kan hebben
die moeten gebruikt worden in de volgende boten
join: O(logn) 2 verzamelingen samenvoegen
B::find -> alleen kleinste element
join: pairing heap!!: maakt het moeilijkst enigste structuur die toelaat op O(logn) 		

FFD pairing heap boten gesorteerd op index, grootst genoeg meest links in het rijtje
alq je sorteert hoe groter hoe slechter dan is dit bestfit
code pairingheap -> zorgen dat je ook BFT kan doen parameter hoe je de boten sorteert, sorteert functor of lamda functie
gedeeltelijkeoplossing code gebruiken die een vector teruggeeft per groep een boot nummer, gedeeltelijke oplossing heuristieken optoepassen in deel 2


*/

template <class S>
class SimulatedAnnealing{
public:
    S oplossing(){
    	int amount = 0;
        T=T0();
        s=initialSolution(); 
        while (!terminateCond()){
        	amount++;
        	//std::cout<<"starting next iteration\n";
            S sprime=pickAtRandom(s);
            double deltaf=f(sprime)-f(s);
            //als f(sprime)<f(s) dan f(sprime)-f(s) <0 || probabiliteit vaeranderen, probailiteit niet negatief?
            if (deltaf < 0 || rand()<p(T,deltaf)*RAND_MAX){
                s=std::move(sprime);
            }            
            updateT();
        }
        std::cout<<"I worked for: "<<amount<<" iterations\n";
        return s;
    }

protected:
	~SimulatedAnnealing()
	{
	}

	double T;//temperatuur;
    S s;//voorlopige en eindoplossing.
    virtual double T0()=0;//initiëele temperatuur
    virtual bool terminateCond()=0;//true als stoppen met zoeken
    virtual double f(const S&)=0;//objectieffunctie. Moet > 0 blijven
    virtual S initialSolution()=0;
    virtual S pickAtRandom(const S&)=0;
    virtual double p(double T, double deltaf)=0;//probabiliteit verandering
    virtual void updateT()=0;
};

class Group{
	public:
		Group(int nummer, int aantal, int groupid) : groupID(groupid), groupNameID(nummer),amount(aantal){
		}
		int getGroupNameID(){
			return groupNameID;
		}
		int getAmount(){
			return amount;
		}
		int getGroupID(){
			return groupID;
		}
	private:
		int groupID;
		int groupNameID;
		int amount;
};
int BOATSIZE = 1000;
int ITERATIONAMOUNT=100;
class SinterklaasBoot : public SimulatedAnnealing<vector<int> >{
protected:
	~SinterklaasBoot()
	{
	}

public:
		SinterklaasBoot(){
			srand(time(nullptr));
			ifstream input ("t.txt");
			string line;
			int groupNumber = 0;
			int id=0;
			int total = 0;
			while(getline(input,line)){
				istringstream iss(line);
				string part1;
				iss>> part1;
				int part2;
				iss>> part2;
				total += part2;
				groupNames.push_back(part1);
				for(int i = part2 ; i >0 ; i-= BOATSIZE){
					if(i >= BOATSIZE){
						Group g(groupNumber, BOATSIZE,id);
						groups.push_back(g);
						//Opsplitsen in groepen niet groter dan de boatsize
					}else{
						Group g(groupNumber,i,id);
						groups.push_back(g);
					}
					id++;
				}
				groupNumber++;
			}
			std::cout<<"total amount of people: " <<total <<"\n";
		}
	private:
		vector<string> groupNames;
		vector<Group> groups;
		vector<int> boatSizes;
		int aantal_Niet_Leeg; //aantal niet lege boten
		vector<int > initialSolution() override
		{ 
			vector<int > result(groups.size());
			int boatNumber = 0;
			int boatSize = 0;
			for(int i = 0 ; i < groups.size() ; i++){
				if(boatSize + groups[i].getAmount() > BOATSIZE){
					boatSizes.push_back(boatSize);//hou bootsize bij
					boatSize = 0;//nieuwe boot vullen
					boatNumber++; //nieuwe boot vullen
				}
				result[groups[i].getGroupID()] = boatNumber;
				boatSize += groups[i].getAmount();
			}
			aantal_Niet_Leeg = boatSizes.size();
			return result;
		}
		int boat_amount = 0;
		int iterations_not_changed = 0;
		bool terminateCond() override
		{			
			if(boat_amount == aantal_Niet_Leeg){ //aantal boten is gelijk aan de aantal niet legen
				iterations_not_changed++;
				if(iterations_not_changed == ITERATIONAMOUNT){ //wanner maximaalaatal iteraties overlopen
					return true;
				}
				//pas hier nooit boat_amount aan???
			}else{
				boat_amount = aantal_Niet_Leeg;
				iterations_not_changed = 0;
			}
			return false;
		}
		
		vector<int > pickAtRandom(const vector<int> &  old) override
		{
			int first = rand()%groups.size();
			int second = rand()%groups.size();
			while(first == second){ // 2 verschillende groepen nemen
				second = rand()%groups.size();
			}
			//de boot is niet vol && de boot is niet vol met de groep erop
			while(BOATSIZE - boatSizes[old[groups[first].getGroupID()]] > 0 && boatSizes[old[groups[first].getGroupID()]] + groups[second].getAmount() > BOATSIZE){
				first = rand()%groups.size();
				second = rand()%groups.size();
			}
			boatSizes[old[groups[first].getGroupID()]] += groups[second].getAmount(); //plaats over naar andere boor
			boatSizes[old[groups[second].getGroupID()]] -= groups[second].getAmount();
			if(boatSizes[old[groups[second].getGroupID()]] ==0 ){ //wanneer deze boot leeg is verminder het aantal
				aantal_Niet_Leeg--; //boat is lig
				std::cout<<"Reduced amount to: "<<aantal_Niet_Leeg<<"\n";
			}
			vector<int> result;
			for(int i = 0 ; i < old.size();i++){
				result.push_back(old[i]);
			}
			result[groups[second].getGroupID()] = old[groups[first].getGroupID()];
			return result;
		}
		
		double T0() override
		{
			return 1000000.0;
		}//initi"ele temperatuur
   
	    double f(const vector<int>& vect) override
	    {
	    	double counter = 0;
	    	for(int i =  0 ; i < vect.size();i++){
	    		if(vect[i] != 0){
	    			counter++;
				}
			}
			return counter;
		}
	    double p(double T, double deltaf) override
	    {
			return exp(-deltaf/T); 
		}//probabiliteit verandering
	    void updateT() override
	    {
			SimulatedAnnealing::T *=0.99;
		}
		
};

#endif
