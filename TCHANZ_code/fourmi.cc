//Fourmi
//Bourgeat Fleur:100%
//Huynh Melvyn: 0%

#include <iostream>
#include <fstream>
#include <sstream>
#include "fourmi.h"
#include "squarecell.h"
#include "message.h"
#include "nourriture.h"



using namespace std;

Fourmi::Fourmi()
:  erreur_fourmi(false),
   centre_ant (true),
   ant_age(0),
   end_of_life(false) 
{}

Fourmi ::~Fourmi(){}

Fourmi::Fourmi (Fourmi const& autre)
:  x_ant(autre.x_ant), y_ant(autre.y_ant),
   ant_side(autre.ant_side), ant_type(autre.ant_type),
   erreur_fourmi(autre.erreur_fourmi), centre_ant(autre.centre_ant),
   ant_food(autre.ant_food), ant_age(autre.ant_age), etat_collector(autre.etat_collector),
   where(autre.where), way(autre.way), vx(autre.vx), vy(autre.vy),
   sym(autre.sym), end_of_life(autre.end_of_life)
{}
 
int Fourmi::get_x_ant() const {
		return x_ant;
}

int Fourmi::get_y_ant () const {
	return y_ant;
}

bool Fourmi::get_centre_ant () const {
	return centre_ant;
}

bool Fourmi::get_erreur_fourmi() const {
	return erreur_fourmi;
}

int Fourmi::get_ant_age() const {
	return ant_age;
}

string Fourmi::get_ant_food() const {
	return ant_food;
}

bool Fourmi::get_end_of_life() const {
	return end_of_life;
}

void Fourmi::set_x_y_side_age_food_ant (int x, int y, short unsigned side, int age, string food) {
	x_ant = x;
	y_ant = y;
	ant_side = side;
	ant_age = age;
	ant_food = food;
}

void Fourmi:: set_x_y(int x,int y){
	x_ant = x;
	y_ant = y;
}	

void Fourmi:: set_side(int side){
	ant_side = side;
}

void Fourmi::set_ant_age(int a) {
	ant_age=a;
}

void Fourmi:: set_where(int where_){
	where = where_;
}

void Fourmi::set_type (int t) {
	ant_type = t;
}

void Fourmi::set_sym(int s) {
	sym=s;
}

void Fourmi:: set_end_of_life(bool vie){    
	end_of_life = vie;
 }

Generator::Generator()
: Fourmi (), side_g(sizeG)
{}


int Generator::get_ant_side() const {
	return side_g;
}

Collector::Collector()
: Fourmi(), side_c(sizeC),
			type_c(1), where(-1), sym(0)
{}


int Collector::get_ant_side () const {
		return side_c;
}

int Collector::get_ant_type() const {
	return type_c;
}


Etat_collector Collector::get_etat() const {
	return etat_collector;
}

int Collector::get_way () const {
	return way;
}

int Collector::get_vx () const {
	return vx;
}

int Collector::get_vy() const {
	return vy;
}

int Collector::get_where () const {
	return where;
}



Defensor::Defensor()
: Fourmi (), side_d(sizeD),
			 type_d(2)
{}

int Defensor::get_ant_side () const {
	return side_d;
}

int Defensor::get_ant_type() const {
	return type_d;
}

Predator::Predator()
: Fourmi(), side_p(sizeP)
{}


int Predator::get_ant_side () const {
	return side_p;
}

Fourmi Fourmi::fourmi_g_lecture (int& x, int& y, Fourmi& ant) {
	Square f_square = {x,y, ant.get_ant_side(), ant.get_centre_ant()};
	if (!(validation_carre(f_square, true))) {
		if (superposition(f_square)!=0) {
			cout << message::generator_overlap(x, y, premiere_cellule_x(f_square),premiere_cellule_y(f_square));
			erreur_fourmi = true;
		} else {
			occupation_carre (f_square);
		}
	} else {
		erreur_fourmi = true;
	}
	ant.set_x_y_side_age_food_ant (x, y, ant.get_ant_side(), 2, "false");
	return ant;
}

Fourmi Fourmi::fourmi_lecture (ifstream& ifs, Fourmi& ant) {
	string line;
	while(getline(ifs>>ws, line)) {
		if(line[0]=='#') continue;
		istringstream data(line);
		data >> x_ant >> y_ant >> ant_age;	
		if (ant.get_ant_type() == 1) {
			data >> ant_food;
			if (ant_food[0] == 't') ant.set_etat(LOADED); 
			if (ant_food[0] == 'f') ant.set_etat(EMPTY); 
		}
		break;
	}
	Square f_square = {x_ant,y_ant, ant.get_ant_side(), ant.get_centre_ant()};
	if (!(validation_carre(f_square, true))) {
		if (superposition(f_square)!=0) {
			if (ant.get_ant_side()==sizeC) {
				if (ant.get_ant_type()==1) {
					cout << message::collector_overlap(x_ant, y_ant, premiere_cellule_x(f_square), premiere_cellule_y(f_square));
					erreur_fourmi = true;
				}
				if (ant.get_ant_type()==2) {
					cout << message::defensor_overlap(x_ant, y_ant, premiere_cellule_x(f_square), premiere_cellule_y(f_square));
					erreur_fourmi = true;
				}
			}
			if (ant.get_ant_side()==sizeP) {
				cout << message::predator_overlap(x_ant, y_ant);
				erreur_fourmi = true;
			}
		} else {
			occupation_carre (f_square);
		}
	} else {
		erreur_fourmi = true;
	}
	ant.set_x_y_side_age_food_ant(x_ant, y_ant, ant.get_ant_side(), ant_age, ant_food);
	return ant;
}

void Fourmi::dessin_fourmi(int index, int x, int y, int length, Fourmi ant) {
	if(length == sizeC){ 
		if(ant.get_ant_type()== 1){ 
			dessin_diagonale(index, x- sizeC/2, y-sizeC/2, sizeC);
		}
		if(ant.get_ant_type()== 2){
			dessin_grille(index, x- sizeD/2, y- sizeD/2, sizeD);
		}
	}
	if(length== sizeP){
		dessin_uniforme(index, x- sizeP/2, y- sizeP/2, sizeP);
	}
	if(length == sizeG){
		dessin_uniforme(index, x- sizeG/2, y-sizeG/2, sizeG);
	}
}

double Fourmi:: distance_to_other_fourmi(Fourmi reference){
	return distance_euclidienne(get_x_ant(), get_y_ant(), reference.get_x_ant(), reference.get_y_ant());
}
	
	
	


