//Nourriture 
//Bourgeat Fleur:0%
//Huynh Melvyn: 100%

#include <iostream>
#include "nourriture.h"
#include "squarecell.h"
#include "message.h"


Nourriture::Nourriture()
: 	side(1),
	centre(true),
	erreur_nourriture(false)
{}

int Nourriture::get_x() const {
	return x_n;
}

int Nourriture::get_y() const {
	return y_n;
}

short unsigned Nourriture::get_side_n ()const {
	return side;
}

bool Nourriture::get_centre_n ()const {
	return centre;
}

bool Nourriture::get_erreur_nourriture() const {
	return erreur_nourriture;
}

void Nourriture::set_x_y_n(int x, int y) {
	x_n=x;
	y_n=y;
}

Nourriture Nourriture::nourriture_lecture (ifstream& ifs, Nourriture& n) {
	string line;
	while(getline(ifs>>ws, line)) {
		if(line[0]=='#') continue;
		istringstream data(line);
		data >> x_n >> y_n;	
		break;
	}				
	Square n_square = {x_n,y_n,n.get_side_n(),n.get_centre_n()};
	if (!validation_carre (n_square, true)) {						
		if (superposition (n_square) != 0) {
			cout << message::food_overlap(x_n,y_n);
			erreur_nourriture = true;
		}
		occupation_carre (n_square);
	}
	return n;
}	

void Nourriture::dessin_nourriture(int x, int y){
	dessin_losange(12, x, y, 1);
}

								

