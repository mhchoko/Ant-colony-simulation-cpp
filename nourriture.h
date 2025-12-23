#ifndef NOURRITURE_H
#define NOURRITURE_H

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Nourriture {
public:
	Nourriture nourriture_lecture (ifstream& ifs, Nourriture& n);
	Nourriture();
	void dessin_nourriture(int x, int y);
	short unsigned get_side_n () const;
	bool get_centre_n () const;
	bool get_erreur_nourriture () const;
	int get_x () const;
	int get_y () const;
	void set_x_y_n (int x, int y);
private:
	int x_n;
	int y_n;
	short unsigned side;
	bool centre;
	bool erreur_nourriture;
};

#endif
