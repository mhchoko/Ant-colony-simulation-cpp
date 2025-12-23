//graphic
//Fleur Bourgeat: 0%
//Huynh Melvyn: 100%

#include <iostream>
#include "graphic.h"
//#include <cairomm/context.h>
using namespace std;

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);  

//fonction initialisant le pointeur
void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr){   //pointeur RefPtr nommé cr venant de la classe Cairo qui pointe vers le type Context (sous classe de Cairo)
	ptcr = &cr;   //on affecte à ptcr l'adresse du pointeur cr (on rappelle qu'un pointeur est une adresse)
}

void choix_couleur(int index){						
	if(index == 0){				//rouge									
		(*ptcr)-> set_source_rgb(1.0,0.0,0.0);
	}
	if(index == 1){				//vert
		(*ptcr)-> set_source_rgb(0.0,0.8,0.0);}
	if(index == 2){				//bleu
		(*ptcr)-> set_source_rgb(0.0,0.0,0.8);}
	if(index == 3){				//jaune
		(*ptcr)-> set_source_rgb(0.7,0.7,0.0);}
	if(index == 4){				//magenta
		(*ptcr)-> set_source_rgb(0.8,0.0,0.8);}
	if(index == 5){				//cyan
		(*ptcr)-> set_source_rgb(0.0,0.6,0.6);}
		
	if(index == 6){        		//rouge clair
		(*ptcr)-> set_source_rgb(1.0,0.3,0.3);}
	if(index == 7){				//vert clair
		(*ptcr)-> set_source_rgb(0.,1.0,0.);}
	if(index == 8){				//bleu clair
		(*ptcr)-> set_source_rgb(0.3,0.3,1.0);}
	if(index == 9){				//jaune clair
		(*ptcr)-> set_source_rgb(1.0,1.0,0.3);}
	if(index == 10){			//magenta clair
		(*ptcr)-> set_source_rgb(1.0,0.3,1.0);}
	if(index == 11){			//cyan clair
		(*ptcr)-> set_source_rgb(0.3,1.0,1.0);}
    if(index == 12){			//blanc
		(*ptcr)-> set_source_rgb(1.0,1.0,1.0);}
}

//dessin de lignes
void dessin_ligne(int x, int y, int x1, int y1){				
	(*ptcr)->move_to(static_cast<double>(x),static_cast<double>(y));
    (*ptcr)->line_to(static_cast<double>(x1), static_cast<double>(y1));
    (*ptcr)->stroke();
}

void dessin_ligne(double x, double y, double x1, double y1){    //fonction utilisée pour le dessin de nourriture
	(*ptcr)-> move_to(x,y);
	(*ptcr)->line_to(x1,y1);
}
	
void dessin_ligne_vertical(double x, double y, int length, bool sens){					
	if(sens == true){			//de bas en haut
		(*ptcr)-> move_to(x,y);
		(*ptcr)-> line_to(x, y + length);
	}
	else{						//de haut en bas
		(*ptcr)-> move_to(x,y);
		(*ptcr)-> line_to(x, y - length);
	}
}

void dessin_ligne_horizontal(double x, double y, int length, bool sens){					//cas où le trait est vertical ou horizontal
	if(sens == true){				//de gauche à droite
		(*ptcr)-> move_to(x,y);
		(*ptcr)-> line_to(x + length, y);
	}
	else{							// de droite à gauche
		(*ptcr)-> move_to(x,y);
		(*ptcr)-> line_to(x - length, y);
	}
}

void dessin_carre_vide(double x, double y, int length){   
	
	(*ptcr)-> set_line_width(0.4);
	dessin_ligne_vertical(x,y,length-0.2, true);  // de bas en haut
	dessin_ligne_horizontal(x-0.2,y+ length-0.8,length-0.2, true); // de gauche à droite
	dessin_ligne_vertical(x+length-1.2, y+length-0.8, length-0.2,false); // de haut en bas
	dessin_ligne_horizontal(x+length-1.2, y, length-0.4, false); // de droite à gauche
	(*ptcr)-> stroke();
}

void dessin_carre_plein(int x, int y, int length){
	
	(*ptcr)-> move_to(x,y);
	(*ptcr)-> line_to(x,y+length);
	(*ptcr)-> line_to(x+length, y+length);
	(*ptcr)-> line_to(x+length,y);
	(*ptcr)-> line_to(x,y);
	(*ptcr)-> fill();
	(*ptcr)-> stroke();
}
	
void dessin_carre_losange(double x, double y, double length){   //pas réussi avec static_cast donc double en paramètre
	
	 (*ptcr)-> move_to(x,y+length/2);
	 (*ptcr)-> line_to(x+length/2,y+length);
	 (*ptcr)-> line_to(x+length,y+length/2);
	 (*ptcr)-> line_to(x+length/2,y);
	 (*ptcr)-> line_to(x,y+length/2);
	 (*ptcr)-> fill();
	 (*ptcr)-> stroke();
 }

//partie squarecell ------------------------------------------------------------
/*
void dessin_vide(void(*couleur)(int), int index,double x, double y, int length){
	
	(*couleur)(index); 
	dessin_carre_vide(x+0.5, y+0.5,length);
}

void dessin_uniforme(void(*couleur)(int), int index, int x, int y, int length){
	
	(*couleur)(index);    
	dessin_carre_plein(x,y,length);
}

void dessin_losange(void(*couleur)(int), int index, double x, double y, double length){
	
	(*couleur)(index);   
	dessin_carre_losange(x, y, length);
}
	
void dessin_diagonale(void(*couleur)(int), int index,int x, int y,int length){
	
	(*couleur)(index); 
	dessin_carre_plein(x,y,length/3);
	dessin_carre_plein(x+2*(length/3),y,length/3);
	dessin_carre_plein(x+length/3,y+length/3,length/3);
	dessin_carre_plein(x,y+2*(length/3),length/3);
	dessin_carre_plein(x+2*(length/3),y+2*(length/3),length/3);
	(*couleur)(index+6); 
	dessin_carre_plein(x+length/3,y,length/3);
	dessin_carre_plein(x,y+length/3,length/3);
	dessin_carre_plein(x+2*(length/3),y+length/3,length/3);
	dessin_carre_plein(x+length/3,y+2*(length/3),length/3);
}
	
void dessin_grille(void(*couleur)(int), int index, int x, int y, int length){
	
	(*couleur)(index); 
	dessin_carre_plein(x+length/3,y,length/3);
	dessin_carre_plein(x,y+length/3,length/3);
	dessin_carre_plein(x+2*(length/3),y+length/3,length/3);
	dessin_carre_plein(x+length/3,y+2*(length/3),length/3);
	dessin_carre_plein(x+length/3,y+length/3,length/3);
	(*couleur)(index+6); 
	dessin_carre_plein(x,y,length/3);
	dessin_carre_plein(x+2*(length/3),y,length/3);
	dessin_carre_plein(x,y+2*(length/3),length/3);
	dessin_carre_plein(x+2*(length/3),y+2*(length/3),length/3);
	
} 
*/
void dessin_monde_blanc(){
	
	(*ptcr)-> set_source_rgb(1., 1., 1.);
	(*ptcr)-> paint();
}

void dessin_monde(int length){			
	
	// fond noir
	(*ptcr)-> set_source_rgb(0., 0., 0.);
	(*ptcr)-> paint();
	
	// bordures blanches  
	(*ptcr)-> set_source_rgb(1., 1., 1.);
	(*ptcr)-> set_line_width(1.0);

	dessin_ligne_vertical(0.5,0.,length,true);
	dessin_ligne_horizontal(0.,0.5,length,true);
	dessin_ligne_horizontal(0.,length-0.5,length,true);
	dessin_ligne_vertical(length-0.5,length,length,false);
	
	(*ptcr)->stroke();
	// cellules blanches  
	
	(*ptcr)-> set_source_rgb(0.5, 0.5, 0.5);
	//cr->set_source_rgb(0.7,0.9,0.6);
	(*ptcr)-> set_line_width(0.25);
	for(int i(0); i <= length ; ++i){  //sera appelé avec length +1 ?
		dessin_ligne_horizontal(0,i,length,true);
	}   
	for(int i(0); i <= length; ++i){
		dessin_ligne_vertical(i,0,length,true);
	}
	(*ptcr) -> stroke();
	
}
 
	
