#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <vector>
#include "fourmiliere.h"
#include "nourriture.h"
#include "squarecell.h"

using namespace std;

class Simulation {
public:
    Simulation();
	Simulation lecture (Simulation s);
	void dessiner();
	string get_fichier () const;
	Simulation (string fichier);
	bool get_erreur();
	bool get_monde_vide();
	vector<Fourmiliere*> get_vector();
	vector<Nourriture> get_food ();
	void save(ofstream &ofs);
	void destruction();
	void test_parite(int x, int y, int a, int b, vector<vector<int>>& options);
	vector<vector<int>> recherche_cible(Fourmi f, Fourmiliere hill, bool in_hill);
	void minimum_ennemy_fourmi(vector<vector<int>> &tab, size_t i);
	void collector_in_hill(Fourmi f, Fourmiliere hill, int& vx, int& vy, int& prox, bool& rien);
	void collector_move(Fourmiliere& hill, Fourmi& f, int i, int j);
	void test_depassement(Fourmi& f,int x, int y, int side, int& sens_1, int& sens_2, int& sens_3, int& sens_4);
	int choix_chemin(int vx, int vy, Fourmi f, int prox);
	void deplacement_collector (Fourmiliere& hill, int chemin, int vx, int vy, Fourmi& f, int i, int j, int prox);
	bool def_gen_move (Fourmiliere& hill, Fourmi& f, int i , int j, bool re);
	void deplacement_def_gen (int x, int y, Fourmi& f, int i, int j);
	void set_sens(Fourmi& f, int& sens_1, int& sens_2, int& sens_3, int& sens_4, bool c); 
	bool contact(Square f_1, Square f_2);
	bool contact_defensor (size_t i, Fourmi f);
	bool superposition_fourmiliere(size_t i, Square s);   
	bool superposition_nourriture_fourmiliere(Square c); 
	bool superposition_fourmiliere_erreur(int i, int j);
	void generer_nourriture(int &compteur);  
	void ajustement_fourmiliere(size_t i, int &cas);   
	void parcours_fourmi(size_t i);
	void move_predator(size_t i, size_t j);  
	void direction_predator(size_t i, size_t j, int x_objectif, int y_objectif, size_t k, size_t m, bool chasser);
	void cible_predator( size_t i, size_t j, size_t k, size_t m, int direction, bool &placed, bool chasser,int x, int y); 
	void kill_ennemy( size_t i, size_t j, size_t k, size_t m, bool &placed,bool chasser, int x, int y); 
	void predator_moved(size_t i, size_t j, int x, int y); 
	bool detection_ennemy_in_hill(size_t i, Square c);
	void chasing_ennemy(size_t i, size_t j,int x, int y, bool inside, Square c);
	int destruction_entite(int i);		
	void mise_a_jour();  
private:
	string nom_fichier;
	bool erreur;
	vector <Fourmiliere*> liste_fourmiliere;
	vector<Nourriture> liste_nourriture;
	bool monde_vide;
	
};

#endif
