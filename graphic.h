#ifndef GRAPHIC_H
#define GRAPHIC_H


#include <gtkmm.h> 
/*				
struct Couleur{
	string rouge;					//au lieu d'un entier on peut faire une structure avec un champ associé à chaque couleur
	string vert;
	string bleu;
	string jaune;
	string magenta;
	string cyan;
} */

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);
/*Cairo::RefPtr<Cairo::Context> choix_couleur(int x); */
void choix_couleur(int index);
void dessin_ligne(int x, int y, int x1, int y1);
void dessin_ligne(double x, double y, double x1, double y1); //surcharge de fonction
void dessin_ligne_vertical(double x, double y, int length, bool sens);
void dessin_ligne_horizontal(double x, double y, int length, bool sens);
void dessin_carre_vide(double x, double y, int length);
void dessin_carre_plein(int x, int y, int length);
void dessin_carre_losange(double x, double y, double length);
void dessin_monde_blanc();
void dessin_monde(int length);

// partie squarecell 
/*
void dessin_vide(void(*couleur)(int), int index, double x, double y, int length);
void dessin_losange(void(*couleur)(int), int index, double x, double y, double length);
void dessin_uniforme(void(*couleur)(int), int index, int x,int y,int length);
void dessin_diagonale(void(*couleur)(int), int index, int x, int y, int length);
void dessin_grille(void(*couleur)(int), int index, int x, int y, int length);
*/
//void dessin_carre(void(*style)(int,int,int), void(*couleur)(int),int x, int y, int length, int index);


#endif 
