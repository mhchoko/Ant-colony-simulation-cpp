//fourmilliere
//Bourgeat Fleur: 50%
//Huynh Melvyn: 50%

#include <iostream>
#include <algorithm>
#include "fourmiliere.h"
#include "nourriture.h"
#include "message.h"
#include "squarecell.h"

using namespace std;

Fourmiliere::Fourmiliere ()
: hill_centre(false),
  erreur_fourmiliere(false),
  end_of_klan(false)
{}

Fourmiliere::Fourmiliere(Fourmiliere const& autre) 
: x(autre.x), y(autre.y), xg(autre.xg), yg(autre.yg),
  side(autre.side), total_food(autre.total_food),
  nbC(autre.nbC), nbD(autre.nbD), nbP(autre.nbP),
  hill_centre(autre.hill_centre), erreur_fourmiliere(autre.erreur_fourmiliere),
  liste_fourmi(autre.liste_fourmi), etat(autre.etat), end_of_klan(autre.end_of_klan)
{}

int Fourmiliere::get_x ()const {
		return x;
	}

int Fourmiliere::get_y ()const {
	return y;
}

short unsigned Fourmiliere::get_side ()const {
	return side;
}

bool Fourmiliere::get_hill_centre ()const {
	return hill_centre;
}

double Fourmiliere::get_total_food()const {
	return total_food;
}

vector<Fourmi*> Fourmiliere::get_liste_fourmi() {
	return liste_fourmi;
}

int Fourmiliere::get_nbC() const {
	return nbC;
}

int Fourmiliere::get_nbD()const {
	return nbD;
}

int Fourmiliere::get_nbP()const {
	return nbP;
}

bool Fourmiliere::get_erreur_fourmiliere()const {
	return erreur_fourmiliere;
}

void Fourmiliere::set_x_y_side (int x_hill, int y_hill, short unsigned side_hill) {
	x = x_hill;
	y = y_hill;
	side = side_hill;
}

void Fourmiliere::set_param_hill (double tot_food, int c, int d, int p) {
	total_food = tot_food;
	nbC = c;
	nbD = d;
	nbP = p;
}

void Fourmiliere::add_food () {
	total_food = total_food + val_food;
}

Fourmiliere Fourmiliere::fourmiliere_lecture (ifstream& ifs, Fourmiliere& hill, size_t i) {
	string line;
	int x,y, side, xg, yg, total_food, nbC, nbD, nbP;
	while (getline(ifs >>ws, line)) {
		if(line[0]=='#') continue;
		istringstream data(line);
		data >> x >> y >> side >> xg >> yg >> total_food >> nbC >> nbD >> nbP;	
		break;
	}
	hill.set_param_hill(total_food, nbC, nbD, nbP);
	hill.set_x_y_side (x,y,side);
	Square fourmilliere_square = {hill.get_x(), hill.get_y(), hill.get_side(), hill.get_hill_centre()};
	if (validation_carre(fourmilliere_square, true)) {
		erreur_fourmiliere = true;
	}											
	Fourmi ant;										
	Generator ant_g;
	ant = ant_g.fourmi_g_lecture (xg, yg, ant_g);
	if (ant.get_erreur_fourmi()) {
		erreur_fourmiliere = true;
	}	
	fourmi_interne(hill, ant_g, i);		
	liste_fourmi.push_back(new Fourmi(ant));
	for (int i(0); i<nbC; ++i) {
		Collector ant_c;											
		ant = ant_c.fourmi_lecture (ifs, ant_c);
		ant.set_way(0,ant_c.get_where(),0,0);
		ant.set_type (ant_c.get_ant_type());
		ant.set_sym (ant_c.get_sym());
		if (ant.get_erreur_fourmi()) {
			erreur_fourmiliere = true;
		}					
		liste_fourmi.push_back(new Fourmi(ant));
	}
	for (int j(0); j<nbD; ++j) {
		Defensor ant_d;
		ant = ant_d.fourmi_lecture (ifs, ant_d);
		ant.set_type (ant_d.get_ant_type());
		if (ant.get_erreur_fourmi()) {
			erreur_fourmiliere = true;
		}
		fourmi_interne (hill,ant_d, i);
		liste_fourmi.push_back(new Fourmi(ant));
	}
	for (int i(0); i<nbP; ++i) {
		Predator ant_p;
		ant = ant_p.fourmi_lecture (ifs, ant_p);
		if (ant.get_erreur_fourmi()) {
			erreur_fourmiliere = true;
		}
		liste_fourmi.push_back(new Fourmi(ant));
	}
	return hill;
}


void Fourmiliere::fourmi_interne (Fourmiliere& hill, Fourmi& ant, size_t& i) {
	short unsigned side_ant (ant.get_ant_side());
	int x_ant(ant.get_x_ant());
	int y_ant(ant.get_y_ant());
	int x(hill.get_x());
	int y(hill.get_y());
	short unsigned side (hill.get_side());
	if ((x_ant-(side_ant/2)<x+1) or (x_ant+(side_ant/2)>x+side-2)) {
		if (ant.get_ant_side() == sizeG) {
			cout << message::generator_not_within_home(x_ant,y_ant,i);
			erreur_fourmiliere = true;
		}
		if (ant.get_ant_side() == sizeD) {
			cout << message::defensor_not_within_home(x_ant,y_ant,i);
			erreur_fourmiliere = true;
		}
	}
	if ((y_ant-(side_ant/2)<y+1) or (y_ant+(side_ant/2)>y+side-2)) {
		if (ant.get_ant_side() == sizeG) {
			cout << message::generator_not_within_home(x_ant,y_ant,i);
			erreur_fourmiliere = true;
		}
		if (ant.get_ant_side() == sizeD) {
			cout << message::defensor_not_within_home(x_ant,y_ant,i);
			erreur_fourmiliere = true;
		}
	}
}

void Fourmiliere::grille_detruite(){
	grille_nulle();
}

void Fourmiliere:: creation_fourmi(double proba_collector, double proba_defensor){   
	int nbT(1+ nbC + nbD + nbP);
	if(static_cast<double>(nbC)/nbT <  proba_collector){
		Collector collector;
		Square c = { get_x(), get_y(), get_side(),get_hill_centre()};   
		if(free(c, collector.get_ant_side())){
			int x(cell_x(c, collector.get_ant_side()));
			int y(cell_y(c, collector.get_ant_side()));
			Square ant = {x, y, sizeC, true};   
			occupation_carre(ant);
			add_fourmi(x, y ,collector.get_ant_side(),1);          
			nbC = nbC +1;	
		}
	} else if(static_cast<double>(nbD)/nbT < proba_defensor){
		Defensor defensor;
		Square c = { get_x(), get_y(), get_side(),get_hill_centre()};
		if(free(c, defensor.get_ant_side())){
			int x(cell_x(c, defensor.get_ant_side()));
			int y(cell_y(c, defensor.get_ant_side()));
			Square ant = {x, y, defensor.get_ant_side(), defensor.get_centre_ant()};
			occupation_carre(ant);
			add_fourmi(x, y ,defensor.get_ant_side(),2);
			nbD = nbD +1;	
		}	
	} else{
		Predator predator;
		Square c = { get_x(), get_y(), get_side(),get_hill_centre()};
		if(free(c, predator.get_ant_side())){
			int x(cell_x(c, predator.get_ant_side()));
			int y(cell_y(c, predator.get_ant_side()));
			predator.set_x_y(x,y);
			Square ant = {x, y, predator.get_ant_side(), predator.get_centre_ant()};
			occupation_carre(ant);
			liste_fourmi.push_back(new Predator(predator));
			nbP = nbP +1;	
		}
	}
}

void Fourmiliere:: add_fourmi(int x, int y, int side, int type){ 
	if( type == 1){
		Fourmi f;
		f.set_x_y(x,y);
		f.set_side(side);
		f.set_type(type);
		f.set_where(-1);
		f.set_sym(0);
		liste_fourmi.push_back(new Fourmi(f));
	}
	else if(type == 2){
		Fourmi f;
		f.set_x_y(x,y);
		f.set_side(side);
		f.set_type(type);
		liste_fourmi.push_back(new Fourmi(f));
	}
}

void Fourmiliere:: naissance_fourmi(){									
	double probabilite(min(1.0, total_food*birth_rate));									
	if(random_booleen(probabilite)){
		if(etat == FREE){
			creation_fourmi(prop_free_collector, prop_free_defensor);
		}
		else{
			creation_fourmi(prop_constrained_collector, prop_constrained_defensor);
		}
	}
}
				
void Fourmiliere:: fin_vie_fourmi( size_t j){
	swap(liste_fourmi[j], liste_fourmi.back());
	liste_fourmi.pop_back();
}

void Fourmiliere::set_side(int side_hill){
	side = side_hill;
}

void Fourmiliere::dessin_fourmiliere(int index, int x, int y, int length){
	dessin_vide(index,x, y, length);
}

Etat_fourmiliere Fourmiliere:: get_etat_fourmiliere() const{
	return etat;
}

void Fourmiliere:: set_etat_fourmiliere(Etat_fourmiliere etat_){
	etat = etat_;
}

bool Fourmiliere::get_end_of_klan() const{
	return end_of_klan;
}

void Fourmiliere:: set_end_of_klan(bool vie){
	end_of_klan = vie;
}

void Fourmiliere:: set_total_food(double tot_food){    
	total_food = tot_food;
}



	
	

