#ifndef SQUARECELLE_H
#define SQUARECELL_H


#include "error_squarecell.h"


struct Square {
	int x,y;
	int side;		
	bool centre;
};


bool validation_carre (Square c, bool message);
bool superposition_2_carres (Square c, Square s);
void occupation_carre (Square c);
void liberer_cellules(Square c);
int superposition (Square c) ;
unsigned int premiere_cellule_x (Square c); 
unsigned int premiere_cellule_y (Square c);
void grille_nulle();
bool free(Square c1, int side);
int cell_x(Square c1, int side);
int cell_y(Square c1, int side);
bool contact(Square f_1, Square f_2);
bool cellule_libre(int x, int y); 
double distance_euclidienne(int x, int y, int x1, int y1);
bool random_booleen(double probabilite);
int random_entier(int min, int max);


void dessin_vide(int index, double x, double y, int length);
void dessin_losange(int index, double x, double y, double length);
void dessin_uniforme(int index, int x,int y,int length);
void dessin_diagonale(int index, int x, int y, int length);
void dessin_grille(int index, int x, int y, int length);

#endif

