//Simulation
//Bourgeat Fleur: 60%
//Huynh Melvyn: 40%

#include <iostream>
#include <fstream>
#include <sstream> 
#include <cmath>
#include "simulation.h" 
#include "constantes.h" 
#include "message.h"     


Simulation::Simulation()
: liste_nourriture({}),
  monde_vide (true)
{}

Simulation::Simulation (string fichier)
: nom_fichier(fichier),
  erreur(false),
  liste_fourmiliere({}),
  monde_vide (false)
{}

string Simulation::get_fichier ()const {
	return nom_fichier;
}

Simulation Simulation::lecture(Simulation s) {   	
	string nom_fichier (s.get_fichier());										
    ifstream ifs;
	ifs.open(nom_fichier, ios::in); 
    if(!ifs.fail()) {
		string line;
		size_t nbN(0);					
		while(getline(ifs>>ws, line)) {
			if(line[0]=='#') continue;
			istringstream data(line);
			data >> nbN ;
			break;
		}
		for(size_t i(0) ; i< nbN ; ++i) {
			Nourriture n;
			n = n.nourriture_lecture (ifs, n);
			if (n.get_erreur_nourriture() == true) {
				erreur = true;
			}	
			liste_nourriture.push_back(n);	
		}
		int nbF;
		while(getline(ifs>>ws, line)) {
			if(line[0]=='#') continue;
			istringstream data(line); 
			data >> nbF ;
			break;
		}
		if (nbF > maxF) {
			cout << "Le nombre de fourmilières est trop important";
		}
		for(int i(0) ; i< nbF ; ++i) {
			Fourmiliere hill;
			hill = hill.fourmiliere_lecture(ifs, hill, i);
			if (hill.get_erreur_fourmiliere()) {
				erreur = true;
			} 	
			liste_fourmiliere.push_back(new Fourmiliere(hill));										
			if (i>=1) {
				for (int j(0); j<i; ++j) {
					if(superposition_fourmiliere_erreur(i,j)){   
						erreur = true;
					}
				}
			}
		}
	}
	return s;
}

bool Simulation::superposition_fourmiliere_erreur(int i, int j){									
	Fourmiliere hill1, hill2;
	hill1 = (*liste_fourmiliere[i]);
	hill2 = (*liste_fourmiliere[j]);
	Square square1 = {hill1.get_x(), hill1.get_y(), hill1.get_side(), hill1.get_hill_centre()};
	Square square2 = {hill2.get_x(), hill2.get_y(), hill2.get_side(), hill2.get_hill_centre()};
	if (superposition_2_carres(square1, square2)) { 
		cout << message::homes_overlap(i,j);
		return true;
	}
	return false;
}



void Simulation::dessiner() {
	Nourriture n;
	Fourmiliere hill;
	int index(0);
	for(size_t i(0); i < (liste_nourriture).size(); ++i){
		n.dessin_nourriture(liste_nourriture[i].get_x(), liste_nourriture[i].get_y());
	}
	for(size_t i(0); i < liste_fourmiliere.size(); ++i) {
		hill.dessin_fourmiliere(index,liste_fourmiliere[i]->get_x(), 
								liste_fourmiliere[i]->get_y(), liste_fourmiliere[i]->get_side()); 
			for(size_t j(0); j < (liste_fourmiliere[i]->get_liste_fourmi()).size(); ++j){
				((liste_fourmiliere[i]->get_liste_fourmi())[j])->dessin_fourmi(index,
																			  (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_x_ant(),
																			  (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_y_ant(),
																			  (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_side(),
																			  *(liste_fourmiliere[i]->get_liste_fourmi())[j]);
		}
		if( index == 5){
			index = 0;
		}else{
			index = index + 1;
		}
	}
}

void Simulation:: save(ofstream &ofs){
	for(size_t i(0); i < (liste_nourriture).size(); ++i){
		ofs << liste_nourriture[i].get_x() << " " << liste_nourriture[i].get_y() << endl;
	}
	ofs << endl;
	ofs << liste_fourmiliere.size() << " # nb anthill"<< endl; //ou liste_fourmiliere.size()
	ofs << endl;
	for(size_t i(0); i < liste_fourmiliere.size(); ++i){
		size_t nbC(liste_fourmiliere[i]->get_nbC());
		size_t nbD(liste_fourmiliere[i]->get_nbD());
		size_t nbP(liste_fourmiliere[i]->get_nbP());
		ofs << liste_fourmiliere[i]->get_x() << " " << liste_fourmiliere[i]->get_y() << " " 
			<< liste_fourmiliere[i]->get_side() << " "
			<< (liste_fourmiliere[i]->get_liste_fourmi())[0]->get_x_ant() << " " 
			<< (liste_fourmiliere[i]->get_liste_fourmi())[0]->get_y_ant() 
			<< " " << liste_fourmiliere[i]->get_total_food() << " " << liste_fourmiliere[i]->get_nbC() 
			<< " " << liste_fourmiliere[i]->get_nbD() << " " << liste_fourmiliere[i]->get_nbP() << endl;
		ofs << endl;
		if(nbC !=0){
			ofs << "# collectors: " << endl;
			for(size_t j(1); j <= (nbC); ++j){ 
				ofs << (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_x_ant() << " " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_y_ant() <<" " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_age() << " " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_food() << endl;
			}
			ofs << endl;
			ofs << "# defensors: " << endl;
			for(size_t j(nbC+1); j <= (nbC +nbD); ++j){
				ofs << (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_x_ant() << " " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_y_ant() <<" " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_age() << endl;
			}
			ofs << endl;
			ofs << "# predators: " << endl;
			for(size_t j(nbC +nbD +1); j <=(nbC+nbD+nbP); ++j){
				ofs << (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_x_ant() << " " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_y_ant() <<" " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_age() << endl;
			}
			ofs << endl;
		}
		else{
			ofs << "# defensors: " << endl;
			for(size_t j(1); j <= (nbC+nbD); ++j){
				ofs << (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_x_ant() << " " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_y_ant() <<" " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_age() << endl;
			}
			ofs << endl;
			ofs << "# predators: " << endl;
			for(size_t j(nbD +1); j<=(nbC+nbD+nbP); ++j){
				ofs << (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_x_ant() << " " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_y_ant() <<" " 
					<< (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_age() << endl;
			}
		}
	}
}

void Simulation::destruction() {
	Fourmiliere hill;
	hill.grille_detruite();   
	liste_nourriture.clear();  
	for(size_t i(0); i < liste_fourmiliere.size(); ++i){
		for (size_t j(0); j<liste_fourmiliere[i]->get_liste_fourmi().size(); ++j) {
			delete liste_fourmiliere[i]->get_liste_fourmi()[j];
			liste_fourmiliere[i]->get_liste_fourmi()[j] = nullptr;
		}
		liste_fourmiliere[i]->get_liste_fourmi().clear();
	}
	for(size_t i(0); i < liste_fourmiliere.size(); ++i){
		delete liste_fourmiliere[i]; 
		liste_fourmiliere[i] = nullptr; 
	}
	liste_fourmiliere.clear();   
} 


bool Simulation::contact(Square f_1, Square f_2) {
	Square a = {f_1.x-1, f_1.y, f_1.side, true};
	Square b = {f_1.x+1, f_1.y, f_1.side, true};
	Square c = {f_1.x, f_1.y+1, f_1.side, true};
	Square d = {f_1.x, f_1.y-1, f_1.side, true};
	Square e = {f_1.x+1, f_1.y+1, f_1.side, true};
	Square f = {f_1.x+1, f_1.y-1, f_1.side, true};
	Square g = {f_1.x-1, f_1.y+1, f_1.side, true};
	Square h = {f_1.x-1, f_1.y-1, f_1.side, true};
	if (superposition_2_carres(a,f_2) or superposition_2_carres(b,f_2) or superposition_2_carres(c,f_2)
		or superposition_2_carres(d,f_2) or superposition_2_carres(e,f_2) or superposition_2_carres(f,f_2)
		or superposition_2_carres(g,f_2) or superposition_2_carres(h,f_2)) {
		return true;
	}
	return false;
}

bool Simulation::contact_defensor (size_t i, Fourmi f) {
	for (size_t j(0); j<liste_fourmiliere.size(); ++j) {
		if (j!=i) {
			 for (size_t g(0); g<liste_fourmiliere[j]->get_liste_fourmi().size(); ++g) {
				 if (liste_fourmiliere[j]->get_liste_fourmi()[g]->get_ant_type() == 2) {
					 Fourmi d (*liste_fourmiliere[j]->get_liste_fourmi()[g]);
					 Square f_1 = {f.get_x_ant(),f.get_y_ant(), f.get_ant_side(), true};
					 Square f_2 = {d.get_x_ant(),d.get_y_ant(), d.get_ant_side(), true};
					 if (contact(f_1, f_2)) {
						 return true;
					 }
				 }
			 }
		 }
	 }
	 return false;
}

void Simulation::test_parite(int x, int y, int a, int b, vector<vector<int>>& options) {
	if (((x%2==a) and (y%2==b)) or ((x%2!=a) and (y%2!=b))) {
		vector<int> r;
		r.push_back(x);
		r.push_back(y);
		options.push_back(r);
	}
}

vector<vector<int>> Simulation::recherche_cible (Fourmi f, Fourmiliere hill, bool in_hill) {
	vector <vector<int>> food_options;
	vector < vector<int>> hill_options;
	int a(f.get_x_ant()%2), b(f.get_y_ant()%2);
	if (f.get_etat()==EMPTY and in_hill==false) {
		for (size_t i(0); i<liste_nourriture.size(); ++i) {
			test_parite(liste_nourriture[i].get_x(), liste_nourriture[i].get_y(),a,b,food_options);
		}
		return food_options;
	} else  {
		int x(hill.get_x()), y(hill.get_y());
		for (int i(x); i<=x+hill.get_side()-1; ++i) {
			if (f.get_etat()==LOADED and y-f.get_y_ant()<0) test_parite(i,y+1,a,b,hill_options);
			else if (f.get_etat()==LOADED and y-f.get_y_ant()>0) test_parite(i,y-1,a,b,hill_options);
			else test_parite(i,y-1,a,b,hill_options);	
		}
		for (int i(y); i<=y+hill.get_side()-1; ++i) {
			if (f.get_etat()==LOADED and x-f.get_x_ant()<0) test_parite(x+1,i,a,b,hill_options);
			else if (f.get_etat()==LOADED and y-f.get_y_ant()>0) test_parite(x-1,i,a,b,hill_options);
			else test_parite(x-1,i,a,b,hill_options);	
		};
		y = y+hill.get_side()-1;
		for (int i(x); i<=x+hill.get_side()-1; ++i) {
			if (f.get_etat()==LOADED and y-f.get_y_ant()<0) test_parite(i,y+1,a,b,hill_options);
			else if (f.get_etat()==LOADED and y-f.get_y_ant()>0) test_parite(i,y-1,a,b,hill_options);
			else test_parite(i,y+1,a,b,hill_options);	
		}
		x = x+hill.get_side()-1;
		for (int i(y-hill.get_side()+1); i<=y;++i) {
			if (f.get_etat()==LOADED and x-f.get_x_ant()<0) test_parite(x+1,i,a,b,hill_options);
			else if (f.get_etat()==LOADED and y-f.get_y_ant()>0) test_parite(x-1,i,a,b,hill_options);
			else test_parite(x+1,i,a,b,hill_options);	
		}
		return hill_options;
	}
}

void Simulation::collector_in_hill (Fourmi f, Fourmiliere hill, int& vx, int& vy, int& prox_1, bool& rien) {
	Square a = {hill.get_x(), hill.get_y(), hill.get_side(), hill.get_hill_centre()};
	Square b = {f.get_x_ant(), f.get_y_ant(), f.get_ant_side(), f.get_centre_ant()};
	if (superposition_2_carres(b,a)) {				
		vector<vector<int>> options(recherche_cible(f, hill, true)); 
		vx = options[0][0]-f.get_x_ant();
		vy = options[0][1]-f.get_y_ant();	
		for (size_t i(1); i<options.size(); ++i) {	
			int vx_1 (options[i][0]-f.get_x_ant());
			int vy_1 (options[i][1]-f.get_y_ant());
			int prox_2 (max(abs(vx_1), abs(vy_1)));
			if (prox_2 < prox_1) {
				prox_1 = prox_2;
				vx = vx_1;
				vy = vy_1;
			} else if (prox_2==prox_1) {
				int a (min(127-f.get_x_ant(),127-f.get_y_ant()));
				int b (min(f.get_x_ant(), f.get_y_ant()));
				int c (min(a,b));
				if (((c==127-f.get_x_ant() or c==f.get_x_ant()) and (c+vx_1>c))
					or ((c==127-f.get_y_ant() or c==f.get_y_ant()) and (c+vy_1>c))) {
						prox_1 = prox_2;
						vx = vx_1;
						vy = vy_1;
				}
			}
		}
	} else rien = true;
}
			

void Simulation::collector_move(Fourmiliere& hill,Fourmi& f, int i, int j) {			
	bool rien (false);
	if (f.get_where() == -1) {
		vector<vector<int>> options(recherche_cible(f, hill, false));
		int vx, vy, prox_1 (max(abs(vx), abs(vy)));
		if (options.size()!=0) {
			vx = options[0][0]-f.get_x_ant();
			vy = options[0][1]-f.get_y_ant();
			if (options.size()>1) {
				for (size_t i(1); i<options.size(); ++i) {
					int vx_1 (options[i][0]-f.get_x_ant());
					int vy_1 (options[i][1]-f.get_y_ant());
					int prox_2 (max(abs(vx_1), abs(vy_1)));
					if (prox_2 < prox_1) {
						prox_1 = prox_2;
						vx = vx_1;
						vy = vy_1;
					} else if (prox_2==prox_1) {
						int a (min(127-f.get_x_ant(),127-f.get_y_ant()));
						int b (min(f.get_x_ant(), f.get_y_ant()));
						int c (min(a,b));
						if (((c==127-f.get_x_ant() or c==f.get_x_ant()) and (c+vx_1>c))
							or ((c==127-f.get_y_ant() or c==f.get_y_ant()) and (c+vy_1>c))) {
							prox_1 = prox_2;
							vx = vx_1;
							vy = vy_1;
						}
					}
				}
			}
		} else {						
			vx = 0;
			vy =0;
			collector_in_hill(f,hill,vx,vy,prox_1, rien);	
		}
		int a;
		if (abs(vx) != abs(vy)) {
			a = choix_chemin(vx, vy, f, prox_1);
		} else {
			a = 1; 
		}
		if (vx==0 and vy==0) rien=true;
		f.set_way(a,0, vx, vy);
	}
	int prox (max(abs(f.get_vx()), abs(f.get_vy())));
	if (!rien) deplacement_collector(hill,f.get_way(), f.get_vx(), f.get_vy(), f, i, j, prox);
	if (rien) f.set_way(0,-1,0,0);
}

void Simulation::test_depassement(Fourmi& f,int x, int y, int side, int& sens_1, int& sens_2, int& sens_3, int& sens_4) {
	if (sens_1!=0) {
		if (x+sens_1+side/2 < 0 or x+sens_1+side/2 > 128-1) {
			sens_1 = -1*sens_1;
			f.set_sym(1);
		}
		if (y+sens_1+side/2 < 0 or y+sens_1+side/2 > 128-1) {
			f.set_sym(3);
			sens_3 = -1*sens_3;
		}
	}
	if (sens_2!=0) {
		if (x+sens_2+side/2 < 0 or x+sens_2+side/2 > 128-1) {
			f.set_sym(2);
			sens_2 = -1*sens_2;
		}
		if (y+sens_2+side/2 < 0 or y+sens_2+side/2 > 128-1) {
			f.set_sym(4);
			sens_4 = -1*sens_4;
		}
	}
}	

int Simulation::choix_chemin (int vx, int vy, Fourmi f, int prox) {
	int x (f.get_x_ant()), y (f.get_y_ant()), ref(0);
	int sup_1(0), sup_2(0);
	int sens_1(1), sens_2(1), sens_3(1), sens_4(1);
	set_sens(f, sens_1, sens_2, sens_3, sens_4, false);
	bool premiere_cellule_1 (false);
	Square a,b,n;
	n = {f.get_x_ant(), f.get_y_ant(), f.get_ant_side(), true};
	for (int i(0); i>=prox; ++i) {
		liberer_cellules(n);
		if (i<abs(vx+vy)/2) {
			test_depassement(f,x, y, f.get_ant_side(), sens_1, ref, sens_3, ref);
			a = {x+sens_1, y+sens_3, f.get_ant_side(), f.get_centre_ant()};
		}
		if (i>=abs(vx+vy)/2) {
			if (i==abs(vx+vy)) set_sens(f, sens_1, sens_2, sens_3, sens_4, true);
			test_depassement(f,x, y, f.get_ant_side(), ref, sens_2, ref, sens_4);
			 a = {x+sens_2, y-sens_4, f.get_ant_side(), f.get_centre_ant()}; 
		 }
		if (superposition(a) != 0) {
			if (i == 0) {premiere_cellule_1 = true;}
			++sup_1;
		}
		if (i<abs(vx-vy)/2) {
			test_depassement(f,x, y, f.get_ant_side(), ref, sens_2, ref, sens_4);
			b = {x+sens_2, y-sens_4, f.get_ant_side(), f.get_centre_ant()};
		}
		if (i>=abs(vx-vy)/2) {
			if (i==abs(vx-vy)) set_sens(f, sens_1, sens_2, sens_3, sens_4, true);
			test_depassement(f,x, y, f.get_ant_side(), sens_1, ref, sens_3, ref);
			b = {x+sens_1, y+sens_3, f.get_ant_side(), f.get_centre_ant()};
		}
		if (superposition(b) != 0) {++sup_2;}
	}
	occupation_carre(n);
	if (sup_1 < sup_2) return 1;
	if (sup_1 > sup_2) return 2;
	if (premiere_cellule_1 == true) return 2;
	return 1;
}

void Simulation::set_sens(Fourmi& f, int& sens_1, int& sens_2, int& sens_3, int& sens_4, bool c) {
	if (f.get_sym()==1) sens_1 =-1;
	if (f.get_sym()==2) {sens_2 = -1;}
	if (f.get_sym()==3) sens_3 =-1;
	if (f.get_sym()==4) sens_4 =-1;
	if (f.get_vx()+f.get_vy()<0) {sens_1 = -1; sens_3=-1;}
	if (f.get_vx()-f.get_vy()<0) {sens_2 = -1; sens_4=-1;}
	if (c) {
		if (f.get_sym()==1) {f.set_sym(2); sens_2=-1*sens_2;}
		if (f.get_sym()==2) {f.set_sym(1); sens_1=-1*sens_1;}
		if (f.get_sym()==3) {f.set_sym(4); sens_4=-1*sens_4;}
		if (f.get_sym()==4) {f.set_sym(3); sens_3=-1*sens_3;}
	}
}

void Simulation::deplacement_collector (Fourmiliere& hill, int chemin, int vx, int vy, Fourmi& f, int i, int j, int prox) {
	int sens_1(1), sens_2(1), ref(0), sens_3(1), sens_4(1);
	set_sens(f,sens_1, sens_2, sens_3, sens_4, false);
	Square h = {hill.get_x(), hill.get_y(), hill.get_side(), false};
	Square a = {f.get_x_ant(), f.get_y_ant(), f.get_ant_side(), f.get_centre_ant()};
	liberer_cellules(a);
	Square b;
	if (f.get_way() == 1) {test_depassement(f,a.x,a.y,a.side,sens_1,ref, sens_3, ref); 
		b = {f.get_x_ant()+sens_1, f.get_y_ant()+sens_3, f.get_ant_side(), f.get_ant_type()};}
	if (f.get_way() == 2) {test_depassement(f,a.x,a.y,a.side,ref,sens_2, ref, sens_4);
		b = {f.get_x_ant()+sens_2, f.get_y_ant()-sens_4, f.get_ant_side(), f.get_ant_type()};}
	if ((vx+vy==0 or (f.get_where()>=abs(f.get_vx()+f.get_vy())/2 and vx-vy!=0)) and f.get_way() == 1) {
		if (f.get_where()==abs(f.get_vx()+f.get_vy())/2) set_sens(f,sens_1, sens_2, sens_3, sens_4, true);
		test_depassement(f,a.x,a.y,a.side,ref,sens_2, ref, sens_4);
		b = {f.get_x_ant()+sens_2, f.get_y_ant()-sens_4, f.get_ant_side(), f.get_ant_type()};}
	if ((vx-vy==0 or (f.get_where() >= abs(f.get_vx()-f.get_vy())/2 and vx+vy!=0)) and f.get_way() == 2) {
		if (f.get_where()==abs(f.get_vx()-f.get_vy())/2) set_sens(f,sens_1, sens_2, sens_3, sens_4, true);
		test_depassement(f,a.x,a.y,a.side,sens_1,ref, sens_3, ref);
		 b = {f.get_x_ant()+sens_1, f.get_y_ant()+sens_3, f.get_ant_side(), f.get_ant_type()};
	 }
	if (contact(b,h) and f.get_etat()==LOADED) { 
			liste_fourmiliere[i]->get_liste_fourmi()[j]->set_etat(EMPTY);
			liste_fourmiliere[i]->add_food();
			f.set_way(0,-2,0,0);
			f.set_sym(0);
	}
	if (superposition(b)==0 and f.get_where()==prox) {f.set_way(0,-2,0,0); f.set_sym(0);}
	if (superposition(b)==0 ) {
		f.set_x_y_side_age_food_ant(b.x, b.y, f.get_ant_side(),f.get_ant_age(), f.get_ant_food());
		f.set_way(f.get_way(), f.get_where()+1, f.get_vx(), f.get_vy());
		occupation_carre(b);
	} else {
		if (f.get_etat()==EMPTY and f.get_where()==prox-2) {
			liste_fourmiliere[i]->get_liste_fourmi()[j]->set_etat(LOADED);
			int x(premiere_cellule_x(b)), y(premiere_cellule_y(b));
			for (size_t t(0); t<liste_nourriture.size(); ++t) {
				if (liste_nourriture[t].get_x()==x and liste_nourriture[t].get_y()==y) {
					Square n = {liste_nourriture[t].get_x(), liste_nourriture[t].get_y(), liste_nourriture[t].get_side_n(), liste_nourriture[t].get_centre_n()};
					liberer_cellules(n);
					swap(liste_nourriture[t],liste_nourriture[liste_nourriture.size()-1]);
					liste_nourriture.pop_back();
				}
			}
			f.set_x_y_side_age_food_ant(b.x, b.y, f.get_ant_side(),f.get_ant_age(), f.get_ant_food());
			occupation_carre(b);
			f.set_way(0,-1,0,0);
			f.set_sym(0);
		} else {
			occupation_carre(a);	
		}
	}	
}


bool Simulation::def_gen_move(Fourmiliere& hill, Fourmi& f, int i, int j, bool re) {
	bool sup;
	Square h = {hill.get_x(), hill.get_y(), hill.get_side(), false};
	Square n = {f.get_x_ant(),f.get_y_ant(), f.get_ant_side(), true};
	if (!superposition_2_carres(n,h)) {return false;}
	int vx, vy, vx_, vy_;
	int sens_x(0), sens_y(0);
	int x(hill.get_x()), y(hill.get_y());
	for (int h(x); h<=x+hill.get_side()-1; ++h) {
		Square a = {h,y,1,true};
		if (superposition_2_carres(n,a)) {vy_=0; sup=true;} 
		else if(f.get_x_ant()==h) vy_ = y+1-f.get_y_ant()+f.get_ant_side()/2;
	}
	for (int k(y); k<=y+hill.get_side()-1; ++k) {
		Square a = {x,k,1,true};
		if (superposition_2_carres(n,a)) {vx_=0; sup=true;} 
		else if(f.get_y_ant()==k) vx_ = x+1-f.get_x_ant()+f.get_ant_side()/2;
	}
	y = y+hill.get_side()-1;
	for (int l(x); l<=x+hill.get_side()-1; ++l) {
		Square a = {l,y,1,true};
		if (superposition_2_carres(n,a)) {vy=0; sup=true;} 
		else if(f.get_x_ant()==l) vy = y-1-f.get_y_ant()-f.get_ant_side()/2;
	}
	x = x+hill.get_side()-1;
	for (int m(y-hill.get_side()+1); m<=y+hill.get_side()-1;++m) {
		Square a = {x,m,1,true};
		if (superposition_2_carres(n,a)) {vx=0; sup=true;} 
		else if(f.get_y_ant()==m) vx = x-1-f.get_x_ant()-f.get_ant_side()/2;
	}
	if (re==true) {
		if (sup==true) return true;
		else return false;
	} else {
		if (sup==true) {
				if (vx==0) sens_x=-1;
				if (vy==0) sens_y=-1;
				if (vy_==0) {vy=vy_;sens_y=1;}
				if (vx_==0) {vx=vx_;sens_x=1;}
				if (f.get_ant_side()==sizeD) {
					if (vx == vy) { deplacement_def_gen(sens_x,0,f,i,j);}
				} else { deplacement_def_gen(sens_x, sens_y, f, i, j);} 
			} else if (f.get_ant_side()==sizeD) {
				sens_x = 1; sens_y = 1;
				if (abs(vx_)<abs(vx) and vx_!=0) {
					sens_x = -1;
					vx = vx_;
				}else if (abs(vy_)<abs(vy) and vy_!=0) {
					sens_y = -1;
					vy = vy_;
				}
				if (vx!=0 and vy!=0 and vx_!=0 and vy_!=0) {
					if (abs(vx)<abs(vy)) deplacement_def_gen(sens_x, 0, f, i, j) ;
					if (abs(vy)<=abs(vx)) deplacement_def_gen(0, sens_y, f, i, j) ;
				}
			}
		Square s = {f.get_x_ant(),f.get_y_ant(), f.get_ant_side(), true};
		bool test (def_gen_move(hill, f, i, j, true));
		if (test or !superposition_2_carres(s,h)) {
			return false;
		}
		return true; 
	}	
}

void Simulation::deplacement_def_gen(int x, int y, Fourmi& f, int i, int j) {
	Square b;
	Square a = {f.get_x_ant(), f.get_y_ant(), f.get_ant_side(), f.get_centre_ant()};
	liberer_cellules(a);
	b = {f.get_x_ant()+x, f.get_y_ant()+y, f.get_ant_side(), f.get_centre_ant()};
	if(superposition(b)!=0) {
		occupation_carre(a);
	} else {
		f.set_x_y_side_age_food_ant(a.x+x, a.y+y, a.side, f.get_ant_age(), f.get_ant_food());
		occupation_carre(b);
	}
}

bool Simulation:: superposition_nourriture_fourmiliere(Square c){
	for(size_t i(0); i < liste_fourmiliere.size(); ++i){
		Square hill = {liste_fourmiliere[i]->get_x(), liste_fourmiliere[i]->get_y(), liste_fourmiliere[i]->get_side(), liste_fourmiliere[i]->get_hill_centre()};
		if (superposition_2_carres(hill, c) == true){
			return true;
		}
	}
	return false;
}
		
													
													
void Simulation:: generer_nourriture(int &compteur){										
	Nourriture n;  
	int x(random_entier(0,126));
	int y(random_entier(0,126));
	Square n_square = {x,y,n.get_side_n(),n.get_centre_n()};
	if ((superposition (n_square) != 0) or (superposition_nourriture_fourmiliere(n_square))){	
		if(compteur != max_food_trial){
			compteur = compteur + 1;  		
			generer_nourriture(compteur);						
		}
	} else{
		occupation_carre (n_square);
		n.set_x_y_n(x, y); 									
		liste_nourriture.push_back(n);
	}		
}

bool Simulation::superposition_fourmiliere(size_t i, Square s){ 
	for (size_t j(0); j< liste_fourmiliere.size(); ++j) {
		if(j != i){
			Square s1 = {liste_fourmiliere[j]->get_x(), liste_fourmiliere[j]->get_y(), liste_fourmiliere[j]->get_side(), liste_fourmiliere[j]->get_hill_centre()};
			if(superposition_2_carres(s,s1)){  
			return true;											
			}
		}
	}
	return false;
}
	
void Simulation:: mise_a_jour(){
	for(size_t i(0); i < liste_fourmiliere.size(); ++i){
		for(size_t j(0); j < (liste_fourmiliere[i]->get_liste_fourmi()).size(); ++j){
		}
	}
	if(random_booleen(food_rate)){
		int compteur(1);
		generer_nourriture(compteur);
	}
	for(size_t i(0); i < liste_fourmiliere.size(); ++i){
		int cas(0);
		ajustement_fourmiliere(i, cas);
		liste_fourmiliere[i]->naissance_fourmi();								
		parcours_fourmi(i);										
	}
	for(size_t i(0); i < liste_fourmiliere.size(); ++i){  
		int j(destruction_entite(i));
		if(j!=-1 and liste_fourmiliere.size()>=2){
			for(size_t k(j); k < liste_fourmiliere.size()-1; ++k){
				swap(liste_fourmiliere[k], liste_fourmiliere[k+1]);
			}
		}
	}
}
     
void Simulation:: parcours_fourmi(size_t i){
	if((liste_fourmiliere[i]->get_end_of_klan()) == false){
		for (size_t j(0); j<liste_fourmiliere[i]->get_liste_fourmi().size(); ++j) {
			if (liste_fourmiliere[i]->get_liste_fourmi()[j]->get_ant_side()==sizeD) {
				if (liste_fourmiliere[i]->get_liste_fourmi()[j]->get_ant_type()==1) {
					if(contact_defensor(i, *liste_fourmiliere[i]->get_liste_fourmi()[j])) {
						liste_fourmiliere[i]->get_liste_fourmi()[j]->set_end_of_life(true);
					}
					collector_move (*liste_fourmiliere[i], *liste_fourmiliere[i]->get_liste_fourmi()[j], i, j);
					if(contact_defensor(i, *liste_fourmiliere[i]->get_liste_fourmi()[j])){
						liste_fourmiliere[i]->get_liste_fourmi()[j]->set_end_of_life(true);
					}
				} else if (liste_fourmiliere[i]->get_liste_fourmi()[j]->get_ant_type()==2) {
					int age((liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_age() +1);
					(liste_fourmiliere[i]->get_liste_fourmi())[j]->set_ant_age(age);
					if (!def_gen_move (*liste_fourmiliere[i], *liste_fourmiliere[i]->get_liste_fourmi()[j], i, j, false)) {
						liste_fourmiliere[i]->get_liste_fourmi()[j]->set_end_of_life(true);
					}
				}
			} else if (liste_fourmiliere[i]->get_liste_fourmi()[j]->get_ant_side()==sizeG) {
				int nbT(1 + liste_fourmiliere[i]->get_nbC() + liste_fourmiliere[i]-> get_nbD() + liste_fourmiliere[i]->get_nbP());                                   
				double total_food(liste_fourmiliere[i]-> get_total_food() - nbT*food_rate);																					
				if(total_food <=0){
					liste_fourmiliere[i]->set_end_of_klan(true);
				} else{ 
					liste_fourmiliere[i]->set_total_food(total_food);
				}
				if (!def_gen_move (*liste_fourmiliere[i], *liste_fourmiliere[i]->get_liste_fourmi()[j], i, j, false)){
					liste_fourmiliere[i]->get_liste_fourmi()[j]->set_end_of_life(true);
					liste_fourmiliere[i]->set_end_of_klan(true);
				}
			} else if (liste_fourmiliere[i]->get_liste_fourmi()[j]->get_ant_side() == sizeP){  							
				int age((liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_age() +1);		
				(liste_fourmiliere[i]->get_liste_fourmi())[j]->set_ant_age(age);				
				move_predator(i,j);
			}			
		}
	}
}


					
void Simulation::move_predator(size_t i, size_t j){
	int x_ant(liste_fourmiliere[i]->get_liste_fourmi()[j]->get_x_ant());	  
	int y_ant(liste_fourmiliere[i]->get_liste_fourmi()[j]->get_y_ant());	  
	int x_hill(liste_fourmiliere[i]-> get_x());
	int y_hill(liste_fourmiliere[i]->get_y());   
	int hill_side(liste_fourmiliere[i]-> get_side());             
	Square hill = { x_hill, y_hill, hill_side, liste_fourmiliere[i]-> get_hill_centre()};
	Square predator = { x_ant, y_ant, sizeP, true};   
	
	if(liste_fourmiliere[i]->get_etat_fourmiliere() == FREE){                
		if(!superposition_2_carres(hill, predator)){    
			direction_predator(i,j, x_hill + hill_side/2, y_hill +hill_side/2, 0,0, false);					
		} else{   
			if (liste_fourmiliere.size()>=1) {   
				chasing_ennemy(i, j, x_ant, y_ant, true, hill); 
			}
		}
	} else{   
		if(detection_ennemy_in_hill(i,hill) == true){      
			chasing_ennemy(i,j, x_ant, y_ant, true, hill);  
		} else{													
			chasing_ennemy(i,j,x_ant,y_ant,false,hill);  
		}
	}
}			


void Simulation::chasing_ennemy(size_t i, size_t j, int x, int y , bool inside, Square c){  
	vector<vector<int>> ennemy_fourmi;
	for (size_t j(0); j<liste_fourmiliere.size(); ++j) {   
		if(j!=i){
			for(size_t k(0); k < (liste_fourmiliere[j]->get_liste_fourmi()).size(); ++k){  
				int x_ennemy((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_x_ant());
				int y_ennemy((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_y_ant());
				Square ant_ennemy = {x_ennemy, y_ennemy, (liste_fourmiliere[j]->get_liste_fourmi())[k]->get_ant_side(), true};
				if(superposition_2_carres(ant_ennemy, c) == inside){      
					if((((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_ant_side() == sizeC) and ((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_ant_type() == 1))
					or((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_ant_side() == sizeP)){  
						vector<int> donnes;
						donnes.push_back(j);
						donnes.push_back(k);
						donnes.push_back(distance_euclidienne(x,y, x_ennemy, y_ennemy));
						ennemy_fourmi.push_back(donnes);
					}
				}
			}
		}
	}
	if(ennemy_fourmi.size() !=0){  
		if( ennemy_fourmi.size() == 1){  
			int indice_hill(ennemy_fourmi[0][0]);
			int indice_ant(ennemy_fourmi[0][1]);
			direction_predator(i, j, (liste_fourmiliere[indice_hill]-> get_liste_fourmi())[indice_ant]-> get_x_ant(),
							  (liste_fourmiliere[indice_hill]-> get_liste_fourmi())[indice_ant]-> get_y_ant(),indice_hill, indice_ant, true);
		} else{	
			for(size_t j(0); j< ennemy_fourmi.size() - 1; ++j){								
				minimum_ennemy_fourmi(ennemy_fourmi, j);
				
		} 
		int indice_hill(ennemy_fourmi[0][0]);
		int indice_ant(ennemy_fourmi[0][1]);										
		direction_predator(i, j, (liste_fourmiliere[indice_hill]-> get_liste_fourmi())[indice_ant]-> get_x_ant(),
						(liste_fourmiliere[indice_hill]-> get_liste_fourmi())[indice_ant]-> get_y_ant(),indice_hill, indice_ant, true);
		}
	}
}

void Simulation::minimum_ennemy_fourmi(vector<vector<int>> &tab, size_t i){									
	if(min(tab[i][2], tab[i+1][2]) == tab[i][2]){
		tab[0][0] = tab[i][0]; 
		tab[0][1] = tab[i][1]; 
		tab[0][2] = tab[i][2]; 
	} else{
		tab[0][0] = tab[i+1][0];
		tab[0][1] = tab[i+1][1];
		tab[0][2] = tab[i+1][2];
	}	
}
	

bool Simulation:: detection_ennemy_in_hill(size_t i, Square c){
	for (size_t j(0); j<liste_fourmiliere.size(); ++j) {   
		if(j!=i){
			for(size_t k(0); k < (liste_fourmiliere[j]->get_liste_fourmi()).size(); ++k){ 
				int x_ennemy((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_x_ant());
				int y_ennemy((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_y_ant());
				Square ant_ennemy = {x_ennemy, y_ennemy, (liste_fourmiliere[j]->get_liste_fourmi())[k]->get_ant_side(), true};
				if(superposition_2_carres(ant_ennemy, c)){      
					if((((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_ant_side() == sizeC) and ((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_ant_type() == 1))
				    or((liste_fourmiliere[j]->get_liste_fourmi())[k]->get_ant_side() == sizeP)){  
						return true;
					}
				}
			}
		}
	}
	return false;
}
						
	
void Simulation:: predator_moved(size_t i, size_t j, int x, int y){											
	Square old_pos = {(liste_fourmiliere[i]->get_liste_fourmi())[j]-> get_x_ant(), (liste_fourmiliere[i]->get_liste_fourmi())[j]->get_y_ant(), sizeP, true};
	Square new_pos = { x,y, sizeP, true};
	liberer_cellules(old_pos);							
	(liste_fourmiliere[i]->get_liste_fourmi())[j]->set_x_y(x,y);   
	occupation_carre(new_pos);
}                              

void Simulation:: kill_ennemy(size_t i, size_t j, size_t k, size_t m, bool &placed, bool chasser,int x, int y){
	if(chasser == true){
		Square predator = {x, y, sizeP, true};
		Square ennemy = {(liste_fourmiliere[k]->get_liste_fourmi())[m]-> get_x_ant(), (liste_fourmiliere[k]->get_liste_fourmi())[m]-> get_y_ant(), 
						(liste_fourmiliere[k]->get_liste_fourmi())[m]-> get_ant_side(), true};
		if((ennemy.side == sizeP) and ((superposition_2_carres(predator,ennemy)) or (contact(predator, ennemy)))){
			predator_moved(i,j,x, y);
			liberer_cellules(ennemy);
			liberer_cellules(predator);
			(liste_fourmiliere[i]->get_liste_fourmi())[j]->set_end_of_life(true);
			(liste_fourmiliere[k]->get_liste_fourmi())[m]->set_end_of_life(true);
			placed = true;
		}
		else if((ennemy.side == sizeC) and ((superposition_2_carres(predator,ennemy)or (contact(predator, ennemy))))){		
			predator_moved(i,j,x, y);
			(liste_fourmiliere[k]->get_liste_fourmi())[m]->set_end_of_life(true);
			liberer_cellules(ennemy);
			placed = true;
		}
		else if(cellule_libre(x, y)){	
			predator_moved(i,j,x, y);				
			placed = true;} 
	}
	else{
		if(cellule_libre(x, y)){		
			predator_moved(i,j,x, y);				
			placed = true;} 
	}
}
		

void Simulation:: cible_predator( size_t i, size_t j, size_t k, size_t m, int direction, bool &placed, bool chasser, int x, int y){  //x,y is for the predator
	switch(direction){   
		case 1:
			kill_ennemy(i,j, k, m, placed, chasser, x-2, y+1);
			break;
		case 2:
			kill_ennemy(i,j, k, m, placed, chasser,x-1, y+2);
			break;
		case 3:					 
			kill_ennemy(i,j, k, m, placed, chasser,x+1, y+2);
			break;
		case 4:	
			kill_ennemy(i,j, k, m, placed, chasser,x+2, y+1);
			break;
		case 5:				
			kill_ennemy(i,j, k, m, placed, chasser,x+2, y-1);
			break;
		case 6:
			kill_ennemy(i,j, k, m, placed, chasser, x+1, y-2);
			break;
		case 7:				 
			kill_ennemy(i,j, k, m, placed, chasser,x-1, y-2);
			break;
		case 8:
			kill_ennemy(i,j, k, m, placed, chasser,x-2, y-1);
			break;	
	}
} 


void Simulation:: direction_predator(size_t i, size_t j,int x_objectif, int y_objectif, size_t k, size_t m, bool chasser){
	int x_ant((liste_fourmiliere[i]->get_liste_fourmi())[j]->get_x_ant());		  
	int y_ant((liste_fourmiliere[i]->get_liste_fourmi())[j]->get_y_ant());		   
	int difference_x(x_ant - x_objectif);
	int difference_y(y_ant -y_objectif);
	bool placed(false);
	if((difference_x == 0) and (difference_y ==0)){
	}
	else{
		if((difference_x >= 0) and (difference_y <=0)){          
			cible_predator(i,j , k, m, 1, placed, chasser, x_ant,y_ant);
			if( placed == false){
				cible_predator(i,j,k,m,2,placed, chasser, x_ant,y_ant);}
		} else if((difference_x <= 0) and (difference_y <=0)){
			cible_predator(i,j , k, m, 3, placed, chasser, x_ant,y_ant);
			if( placed == false) {
				cible_predator(i,j , k, m, 4, placed, chasser, x_ant,y_ant);}
		} else if((difference_x <= 0) and (difference_y >=0)){  
			cible_predator(i,j , k, m, 5, placed, chasser, x_ant,y_ant);
			if( placed == false){
				cible_predator(i,j , k, m, 6, placed, chasser, x_ant,y_ant);}
		}else if((difference_x >= 0) and (difference_y >=0)){  
			cible_predator(i,j , k, m, 7, placed, chasser, x_ant,y_ant);
			if( placed == false){
				cible_predator(i,j , k, m, 8, placed, chasser, x_ant,y_ant);}
		}
	}
}	


void Simulation:: ajustement_fourmiliere(size_t i, int &cas){
	
	int nbC(liste_fourmiliere[i]-> get_nbC());
	int nbD(liste_fourmiliere[i]-> get_nbD());
	int nbP(liste_fourmiliere[i]-> get_nbP());
	int sizeF(sqrt(4*(sizeG * sizeG + sizeC * sizeC *nbC + sizeD * sizeD * nbD + sizeP * sizeP * nbP)));
	Square s_inf_g = {liste_fourmiliere[i]->get_x(), liste_fourmiliere[i]->get_y(), sizeF+2, liste_fourmiliere[i]->get_hill_centre()};
	Square s_sup_g = {liste_fourmiliere[i]-> get_x(), liste_fourmiliere[i]->get_y() + liste_fourmiliere[i]->get_side()- sizeF - 2 , sizeF+2, liste_fourmiliere[i]->get_hill_centre()};
	Square s_sup_d = {liste_fourmiliere[i]->get_x() + liste_fourmiliere[i]->get_side()- sizeF-2, liste_fourmiliere[i]->get_y() + liste_fourmiliere[i]->get_side()- sizeF, sizeF+2, liste_fourmiliere[i]->get_hill_centre()};
	Square s_inf_d = {liste_fourmiliere[i]->get_x() + liste_fourmiliere[i]->get_side() - sizeF-2, liste_fourmiliere[i]->get_y(), sizeF+2, liste_fourmiliere[i]->get_hill_centre()};
		if(!validation_carre(s_inf_g, false)and (!superposition_fourmiliere(i, s_inf_g))){  
			cas = 1; 
			liste_fourmiliere[i]-> set_side(sizeF + 2);
			liste_fourmiliere[i] ->set_etat_fourmiliere(FREE);
		}
		else if(!validation_carre(s_sup_g, false)and (!superposition_fourmiliere(i, s_sup_g))){ 
			cas = 2;
			liste_fourmiliere[i]-> set_x_y_side(liste_fourmiliere[i]->get_x(), liste_fourmiliere[i]->get_y() + liste_fourmiliere[i]->get_side()- sizeF-2, sizeF +2);	
		}				
		else if(!validation_carre(s_sup_d, false)and (!superposition_fourmiliere(i, s_sup_d))){ 
			cas = 3;
			liste_fourmiliere[i]-> set_x_y_side(liste_fourmiliere[i]->get_x() + liste_fourmiliere[i]->get_side()- sizeF - 2, liste_fourmiliere[i]->get_y() + liste_fourmiliere[i]->get_side()- sizeF - 2, sizeF + 2);
			liste_fourmiliere[i] ->set_etat_fourmiliere(FREE);	
		}		
		else if(!validation_carre(s_inf_d, false)and(!superposition_fourmiliere(i, s_inf_d))){ 
			cas = 4; 
			liste_fourmiliere[i]-> set_x_y_side(liste_fourmiliere[i]->get_x() + liste_fourmiliere[i]->get_side() - sizeF - 2, liste_fourmiliere[i]->get_y(), sizeF+2);
			liste_fourmiliere[i] ->set_etat_fourmiliere(FREE);
		}
		else{
			cas = 0;
			liste_fourmiliere[i]->set_etat_fourmiliere(CONSTRAINED);
		}
}


int Simulation:: destruction_entite(int i){
	int j(-1);
	for(size_t j(0); j < (liste_fourmiliere[i]->get_liste_fourmi()).size(); ++j){
		if(((liste_fourmiliere[i]->get_liste_fourmi())[j]->get_ant_age() == bug_life)
			or ((liste_fourmiliere[i]->get_liste_fourmi())[j]->get_end_of_life() == true)){
			liste_fourmiliere[i]->fin_vie_fourmi(j);
		}
	}
	if(liste_fourmiliere[i]->get_end_of_klan() == true){		
		j = i;													
		swap(liste_fourmiliere[i], liste_fourmiliere.back());    
		liste_fourmiliere.pop_back();							  		
	} 
	return j;
}	
	


vector<Fourmiliere*> Simulation::get_vector() 
{
	return liste_fourmiliere;
}
 
bool Simulation::get_erreur() 
{
	return erreur;
}

bool Simulation::get_monde_vide()
{
	return monde_vide;
}

vector<Nourriture> Simulation::get_food()
{
	return liste_nourriture;
}


