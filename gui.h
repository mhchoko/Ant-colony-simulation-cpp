#ifndef GUI_H
#define GUI_H

#include <gtkmm.h> 
#include "simulation.h"

struct Frame // Model Framing and window parameters
{
	double xMin; // frame parameters
	double xMax;
	double yMin;
	double yMax;
	double asp;  // frame aspect ratio   //-> ratio ????
	int height;  // window height
	int width;   // window width
};

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea(Simulation simulation);			//constructeur et destructeur
  virtual ~MyArea();  
  void adjustFrame();	
  void set_empty(bool vrai);
  void refresh();
  void set_sim(Simulation s);

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  void draw_frame(const Cairo::RefPtr<Cairo::Context>& cr);
private:
  Frame frame;
  Simulation sim;
  bool empty;
 
};

 
class Gui : public Gtk::Window
{
public:
	Gui(Simulation simulation); //constructeur
    virtual ~Gui(); //destructeur
    void set_simulation (Simulation s);
    void set_area (MyArea new_area);

protected:
	Simulation s;
	
	//Signal handlers:
	void on_button_clicked_Exit();
	void on_button_clicked_Open();
	void on_button_clicked_Save();
	void on_button_clicked_Previous();
	void on_button_clicked_Next();
	void on_button_clicked_Start();
	void on_button_clicked_Step();
	void on_button_add_timer();
	void on_button_quit();

	bool on_timeout(); 
	
	//lien touches clavier
	bool on_key_press_event(GdkEventKey * key_event);
	
	//Child widgets:
	Gtk::Box m_Box_Top, m_Box_Right, m_Box_Left, m_Box1, m_Box2, m_Box3;
	Gtk::Button m_Button_Exit;
	Gtk::Button m_Button_Open;
	Gtk::Button m_Button_Save;
	Gtk::Button m_Button_Previous;
	Gtk::Button m_Button_Next;
	Gtk::Button m_Button_Start;
	Gtk::Button m_Button_Step;	
	Gtk::Separator m_Separator1;
	Gtk::Separator m_Separator2;
	Gtk::Label m_Label_General;
	Gtk::Label m_Label_Info;
	Gtk::Label m_Label_Nb_food; //dont le titre sera Info
	Gtk::Label m_Label_Anthill;
	Gtk::Label m_Label_None_selected;
	
	bool timer_added;
	bool disconnect;
	const int timeout_value;
	size_t etat_courant;
	MyArea m_Area;

};

#endif //GUI_H
