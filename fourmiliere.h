#ifndef FOURMILIERE_H
#define FOURMILIERE_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "fourmi.h"
#include "constantes.h"

using namespace std;

class Fourmiliere {
public:
	Fourmiliere ();
	~Fourmiliere () {};
	Fourmiliere (Fourmiliere const& autre);
	Fourmiliere fourmiliere_lecture (ifstream& ifs, Fourmiliere& hill, size_t i);
	void fourmi_interne (Fourmiliere& hill, Fourmi& ant, size_t& i);
	void dessin_fourmiliere(int index, int x, int y, int length);
	void naissance_fourmi(); 
	void creation_fourmi(double proba_collector, double proba_defensor);
	int get_x ()const;
	int get_y ()const;
	short unsigned get_side () const;
	Etat_fourmiliere get_etat_fourmiliere() const;  
	bool get_end_of_klan() const;  
	double get_total_food() const;
	int get_nbC() const;
	int get_nbD() const;
	int get_nbP() const;
	bool get_hill_centre ()const;
	bool get_erreur_fourmiliere ()const;
	void set_x_y_side (int x_hill, int y_hill, short unsigned side_hill);
	void set_side(int side_hill);  
	void set_etat_fourmiliere(Etat_fourmiliere etat_);  
	void set_end_of_klan(bool vie); 
	void add_food();
	void set_param_hill (double tot_food, int c, int d, int p);
	void set_total_food(double tot_food);
	void add_fourmi(int x, int y, int side, int type);  
	vector<Fourmi*> get_liste_fourmi();
	void fin_vie_fourmi( size_t j);  
	void grille_detruite();
private:
	int x,y,xg,yg;
	short unsigned side;
	 double total_food;
	int nbC, nbD, nbP;
	bool hill_centre;
	bool erreur_fourmiliere;
	vector<Fourmi*> liste_fourmi;
	Etat_fourmiliere etat;      
	bool end_of_klan;   
};

#endif
