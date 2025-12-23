#ifndef FOURMI_H
#define FOURMI_H

#include <string>
#include "constantes.h"

using namespace std;


class Fourmi {
public:
	Fourmi();
	Fourmi (Fourmi const& autre);
	virtual ~Fourmi();
	int get_x_ant () const;
	int get_y_ant () const;
	bool get_centre_ant () const;
	bool get_erreur_fourmi() const;
	Fourmi fourmi_g_lecture (int& x, int& y, Fourmi& ant);
	Fourmi fourmi_lecture (ifstream& ifs, Fourmi& ant);
	void dessin_fourmi(int index, int x, int y, int length, Fourmi ant);
	bool get_end_of_life() const; 
	virtual int get_ant_side() const { return ant_side;}
	virtual int get_ant_type() const {return ant_type;}
	void set_etat(Etat_collector etat) {etat_collector = etat;}
	virtual Etat_collector get_etat() const {return etat_collector;}
	virtual int get_way() const {return way;}
	virtual int get_vx() const {return vx;}
	virtual int get_vy() const {return vy;}
	virtual int get_where() const {return where;}
	virtual int get_sym() const {return sym;}
	double distance_to_other_fourmi(Fourmi reference);
	void set_sym(int s);
	void set_x_y(int x, int y);
	void set_side(int side);
	void set_ant_age (int a);
	void set_way (int a, int w, int x, int y) {way=a; where=w; vx=x; vy=y;}
	void set_where(int where_);
	int get_ant_age() const;
	string get_ant_food() const;
	void set_x_y_side_age_food_ant (int x, int y, short unsigned side, int age, string food);
	void set_type (int t);
	void set_end_of_life(bool vie);
protected:
	int x_ant;
	int y_ant;
	int ant_side;
	int ant_type;
	bool erreur_fourmi;
	bool centre_ant;
	string ant_food;
	int ant_age;
	Etat_collector etat_collector;
	int where ,way, vx, vy, sym;
	bool end_of_life; 
};

class Generator : public Fourmi {
public:
	Generator();
	int get_ant_side() const;
protected:
	int side_g;
};

class Collector :public Fourmi {
public:
	Collector();
	int get_ant_side () const;
	int get_ant_type() const;
	void set_etat(Etat_collector etat) {etat_collector=etat;}
	int get_sym() const {return sym;}
	virtual Etat_collector get_etat () const;
	int get_way() const;
	int get_vx() const;
	int get_vy() const;
	int get_where() const;
private:
	int side_c;
	int type_c;
	Etat_collector etat_collector;
	int where ,way, vx, vy, sym;
};	
	
class Defensor : public Fourmi {
public:
	Defensor();
	int get_ant_side () const;
	int get_ant_type() const;
protected:
	int side_d;
	int type_d;
};


class Predator : public Fourmi {
public:
	Predator();
	int get_ant_side() const;
protected:
	int side_p;
};

#endif
