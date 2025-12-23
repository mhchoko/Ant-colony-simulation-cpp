//Squarecell
//Bourgeat Fleur: 40%
//Huynh Melvyn: 60%

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include "squarecell.h"
#include "graphic.h"


using namespace std;

constexpr short unsigned g_dim(7);
constexpr short unsigned g_max(pow(2,g_dim));

static vector<vector<bool>> grid(g_max, vector<bool> (g_max));
static default_random_engine e;	

bool validation_carre (Square c, bool message) {
	if (c.x<0 or c.x>g_max-1) {
		if( message == true){
		cout << error_squarecell::print_index(c.x, g_max-1);}
		return true;
	}
	if (c.y<0 or c.y>g_max-1) {
		if( message == true){
		cout << error_squarecell::print_index(c.y, g_max-1);}
		return true;
	}
	if (c.centre == true) {
		if(c.side%2==0) {
			return true;
		}
		if (c.x+(c.side/2)>g_max-1 or c.x-(c.side/2)<0) {
			if( message == true){
			cout << error_squarecell::print_outside(c.x, c.side, g_max-1);}
			return true;
		}
		if (c.y+(c.side/2)>g_max-1 or c.y-(c.side/2)<0) {
			if( message == true){
			cout << error_squarecell::print_outside(c.y, c.side, g_max-1);}
			return true;
		}
	} else {
		if (c.side<=0) {return true;}
		if (c.x+c.side>g_max-1) {
			if( message == true){
			cout << error_squarecell::print_outside(c.x, c.side, g_max-1);}
			return true;
		}
		if (c.y+c.side>g_max-1) {
			if( message == true){
			cout << error_squarecell::print_outside(c.y, c.side, g_max-1);}
			return true;
		}
	}
	return false;
}

bool superposition_2_carres (Square c, Square s) {
	if (c.centre == false) {
		for (int i(c.x); i<=(c.x+c.side-1); ++i) {
			for (int j(c.y); j<=(c.y+c.side-1); ++j) {
				if (s.centre == false) {
					if ((s.x<=i and i<= (s.x+s.side-1)) and (s.y<=j and j<=(s.y+s.side-1))) {
						return true;
					}
				} else {
					if ((s.x-s.side/2<=i and i<=(s.x+(s.side/2))) and (s.y-s.side/2<=j and j<=(s.y+(s.side/2)))) {
						return true;
					}
				}
			}
		}
	} else {
		for (int i(c.x-c.side/2); i<=(c.x+c.side/2); ++i) {
			for (int j(c.y-c.side/2); j<=(c.y+c.side/2); ++j) {
				if (s.centre == true){
					if ((s.x-s.side/2<=i and i<=(s.x+s.side/2)) and (s.y-s.side/2<=j and j<=(s.y+s.side/2))) {
						return true;
					}
				} else {
					if ((s.x<=i and i<=s.x+s.side-1) and (s.y<=j and j<=s.y+s.side-1)) {
						return true;
					}
				}
			}
		}
	}
	return false;
}


void occupation_carre (Square c) {
	if (c.centre == false) {
		for (size_t i(grid.size()-c.y-c.side-1); i < grid.size()-c.y; ++i) {
			for (int j(c.x); j < c.x+c.side+1; ++j) {
				grid[i][j] = 1;
			}
		}
	} else {
		for (size_t i(grid.size()-c.y-1-(c.side/2)); i < grid.size()-c.y+c.side/2; ++i) {
			for (int j(c.x-(c.side/2)); j < c.x+(c.side/2)+1; ++j) {
				grid[i][j] = 1;
			}
		}
	}
}
	
void liberer_cellules(Square c) {
	if (c.centre == false) {
		for (size_t i(grid.size()-c.y-c.side-1); i < grid.size()-c.y; ++i) {
			for (int j(c.x); j < c.x+c.side+1; ++j) {
				grid[i][j] = 0;
			}
		}
	} else {
		for (size_t i(grid.size()-c.y-1-(c.side/2)); i < grid.size()-c.y+c.side/2; ++i) {
			for (int j(c.x-(c.side/2)); j < c.x+(c.side/2)+1; ++j) {
				grid[i][j] = 0;
			}
		}
	}
}
	
int superposition (Square c) {
	int l(0);
	if (c.centre == false) {
		for (size_t i(grid.size()-c.y-c.side-1); i < grid.size()-c.y; ++i) {
			for (int j(c.x); j < c.x+c.side+1; ++j) {
				if (grid[i][j] == 1) {
					l = l+1;
				}
			}
		}
	} else {
		for (size_t i(grid.size()-c.y-1-(c.side/2)); i < grid.size()-c.y+c.side/2; ++i) {
			for (int j(c.x-(c.side/2)); j < c.x+(c.side/2)+1; ++j) {
				if (grid[i][j] == 1) {
					l = l+1;
				}
			}
		}
	}
	return l;
}

unsigned int premiere_cellule_x(Square c){
	int a;
	bool fin(false);
	for (size_t i(grid.size()-c.y-1-(c.side/2)); i < grid.size()-c.y+c.side/2; ++i) {
		if (fin == true) break;
		for (int j(c.x-(c.side/2)); j < c.x+(c.side/2)+1; ++j) {
			if (grid[i][j] == 1) {
				a=j;
				fin =true;
				break;
			}
		}
	}
	return a;
}

unsigned int premiere_cellule_y(Square c){
	bool fin (false);
	int a;
	for (size_t i(grid.size()-c.y-1-(c.side/2)); i < grid.size()-c.y+c.side/2; ++i) {
		if (fin == true) break;
		for (int j(c.x-(c.side/2)); j < c.x+(c.side/2)+1; ++j) {
			if (grid[i][j] == 1) {
				a=g_max-1-i;
				fin = true;
				break;
			}
		}
	}
	return a;
}

void grille_nulle(){
	for(size_t i(0); i < grid.size(); ++i){
		for(size_t j(0); j < grid.size(); ++j){
			grid[i][j] = 0;
		}
	}
}

bool contact(Square f_1, Square f_2) {
	Square a = {f_1.x-1, f_1.y, f_1.side, true};
	Square b = {f_1.x+1, f_1.y, f_1.side, true};
	Square c = {f_1.x, f_1.y+1, f_1.side, true};
	Square d = {f_1.x, f_1.y-1, f_1.side, true};
	if (superposition_2_carres(a,f_2) or superposition_2_carres(b,f_2) or
		superposition_2_carres(c,f_2)
		or superposition_2_carres(d,f_2)) {
		return true;
	}
	return false;
}

bool random_booleen(double probabilite){
	bernoulli_distribution b(probabilite);
	if (b(e)){
		return true;
	}
	return false;
}

int random_entier(int min, int max){
	uniform_int_distribution<int> u(min, max);  
	return u(e);
}

int cell_x(Square c1, int side){
	bool fin(false);
	int b;
	for(int i(c1.y+side/2+1); i < c1.y + c1.side-side/2-1; ++i){  
		if(fin ==true){break;}
		for (int j(c1.x+side/2 + 1); j < c1.x+c1.side - side/2 - 1; ++j){
			Square c = {j,i,side,true};
			if(superposition(c)==0){
				fin = true;
				b = c.x;
				break;
			}
		}
	}
	return b;
}


int cell_y(Square c1, int side){
	bool fin(false);
	int b;
	for(int i(c1.y+side/2+1); i < c1.y + c1.side-side/2-1; ++i){  
		if(fin ==true){break;}
		for (int j(c1.x+side/2 + 1); j < c1.x+c1.side - side/2 - 1; ++j){
			Square c = {j,i,side,true};
			if(superposition(c)==0){
				fin = true;
				b = c.y;
				break;
			}
		}
	}
	return b;
}


bool free(Square c1, int side){
	for(int i(c1.y+side/2+1); i < c1.y + c1.side-side/2-1; ++i){  
		for (int j(c1.x+side/2 + 1); j < c1.x+c1.side - side/2 - 1; ++j){
			Square c = {j,i,side,true};
			if(superposition(c)==0){
				return true;
			}
		}
	}
	return false;
}


			
			
bool cellule_libre(int x, int y){  										
	int grid_y( grid.size() - y - 1);
	if(grid[grid_y][x] == 0){
		return true;
	}
	return false;
}

double distance_euclidienne(int x, int y, int x1, int y1){		       
	double difference_x;
	double difference_y;
	difference_x = static_cast<double> (x1)-x;
	difference_y = static_cast<double> (y1)-y;
	return sqrt((difference_x)*(difference_x) + (difference_y)*(difference_y));
}		



void dessin_vide(int index,double x, double y, int length){

	choix_couleur(index);
	dessin_carre_vide(x+0.5, y+0.5,length);
}

void dessin_uniforme( int index, int x, int y, int length){
	
	choix_couleur(index);
	dessin_carre_plein(x,y,length);
}

void dessin_losange(int index, double x, double y, double length){
	
	choix_couleur(index);
	dessin_carre_losange(x, y, length);
	
}
	
void dessin_diagonale(int index,int x, int y,int length){
	
	choix_couleur(index); 
	dessin_carre_plein(x,y,length/3);
	dessin_carre_plein(x+2*(length/3),y,length/3);
	dessin_carre_plein(x+length/3,y+length/3,length/3);
	dessin_carre_plein(x,y+2*(length/3),length/3);
	dessin_carre_plein(x+2*(length/3),y+2*(length/3),length/3);
	choix_couleur(index + 6); 
	dessin_carre_plein(x+length/3,y,length/3);
	dessin_carre_plein(x,y+length/3,length/3);
	dessin_carre_plein(x+2*(length/3),y+length/3,length/3);
	dessin_carre_plein(x+length/3,y+2*(length/3),length/3);
}
	
void dessin_grille(int index, int x, int y, int length){
	
	choix_couleur(index); 
	dessin_carre_plein(x+length/3,y,length/3);
	dessin_carre_plein(x,y+length/3,length/3);
	dessin_carre_plein(x+2*(length/3),y+length/3,length/3);
	dessin_carre_plein(x+length/3,y+2*(length/3),length/3);
	dessin_carre_plein(x+length/3,y+length/3,length/3);
	choix_couleur(index+ 6); 
	dessin_carre_plein(x,y,length/3);
	dessin_carre_plein(x+2*(length/3),y,length/3);
	dessin_carre_plein(x,y+2*(length/3),length/3);
	dessin_carre_plein(x+2*(length/3),y+2*(length/3),length/3);
	
}

