//Gui
//Bourgeat Fleur: 90%
//Huynh Melvyn: 10%

#include <iostream>
#include <string>
#include <fstream>
#include "gui.h"
#include "graphic.h"
#include "fourmiliere.h" 



using namespace std;

constexpr unsigned taille_dessin(500);

static Frame default_frame = {0., 128., 0., 128., 1, 128, 128};   
static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
									Frame frame);


Gui::Gui(Simulation simulation)  
: s(simulation),
  m_Box_Top(Gtk::ORIENTATION_HORIZONTAL), 
  m_Box_Right(Gtk::ORIENTATION_VERTICAL, 10),
  m_Box_Left(Gtk::ORIENTATION_VERTICAL, 10),
  m_Box1(Gtk::ORIENTATION_VERTICAL, 10),			
  m_Box2(Gtk::ORIENTATION_VERTICAL, 10),
  m_Box3(Gtk::ORIENTATION_VERTICAL, 10),

  m_Button_Exit("exit"),
  m_Button_Open("open"),
  m_Button_Save("save"),
  m_Button_Previous("previous"),
  m_Button_Next("next"),
  m_Button_Start("start"),
  m_Button_Step("step"),
  m_Label_General("General"),
  m_Label_Info("Info"),
  m_Label_Nb_food("Nb food: 0"), 
  m_Label_Anthill("Anthill info "),
  m_Label_None_selected("None selected"),
 
  timer_added(false),
  disconnect(false),
  timeout_value(500),
  etat_courant(0),
  m_Area (simulation)
{
	set_title("TCHANZ");
	set_border_width(0);
	
	add(m_Box_Top);
	
	m_Box_Top.pack_start(m_Box_Left);
	m_Box_Top.pack_start(m_Box_Right);
	m_Area.set_size_request(taille_dessin,taille_dessin);
	m_Box_Right.pack_start(m_Area);
	m_Box_Left.pack_start(m_Box3);
	m_Box_Left.pack_start(m_Box1);
	m_Box_Left.pack_start(m_Separator1);
	m_Box_Left.pack_start(m_Box2);
	m_Box_Left.pack_start(m_Separator2);
	
	
	m_Box3.pack_start(m_Label_General);
	m_Box3.pack_start(m_Button_Exit);
	m_Box3.pack_start(m_Button_Open);
	m_Box3.pack_start(m_Button_Save);
	m_Box3.pack_start(m_Button_Start);
	m_Box3.pack_start(m_Button_Step);
	
	m_Box1.set_border_width(10);			
    m_Box2.set_border_width(10);
	
	m_Box1.pack_start(m_Label_Info);
	m_Box1.pack_start(m_Label_Nb_food);
	
	m_Box2.pack_start(m_Label_Anthill);
	m_Box2.pack_start(m_Button_Previous);
	m_Box2.pack_start(m_Button_Next);
	m_Box2.pack_start(m_Label_None_selected);
	

	m_Button_Exit.signal_clicked().connect(sigc::mem_fun(*this,
              &Gui::on_button_clicked_Exit) );
    m_Button_Start.signal_clicked().connect(sigc::mem_fun(*this,
			  &Gui::on_button_clicked_Start));
	m_Button_Step.signal_clicked().connect(sigc::mem_fun(*this,
			  &Gui::on_button_clicked_Step));
	m_Button_Next.signal_clicked().connect(sigc::mem_fun(*this,
			  &Gui::on_button_clicked_Next));
	m_Button_Previous.signal_clicked().connect(sigc::mem_fun(*this,
			  &Gui::on_button_clicked_Previous));
	m_Button_Open.signal_clicked().connect(sigc::mem_fun(*this,
			  &Gui::on_button_clicked_Open));
	m_Button_Save.signal_clicked().connect(sigc::mem_fun(*this,
			  &Gui::on_button_clicked_Save));
	show_all_children();
	
	if (s.get_erreur() or s.get_monde_vide()) {
	  m_Area.set_empty (true);
	  m_Area.refresh();
  } 
};

Gui::~Gui()
{
}

void Gui::on_button_clicked_Exit(){
	hide();
}


void Gui::on_button_clicked_Open() 
{
	Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);
	
	dialog.run();
	
	std::string filename = dialog.get_filename();
	m_Area.set_empty(false);
	s.destruction();
	Simulation s1 (filename);
	s1.lecture(s1);
	set_simulation(s1);
	m_Area.set_sim (s1);
	m_Area.refresh();
	if (s1.get_erreur()) {
	  m_Area.set_empty (true);
	  m_Area.refresh();
	}
  
}


void Gui::on_button_clicked_Save()
{
	ofstream myfile ("etat_de_la_simulation.txt");
	if (myfile.is_open()) {
		myfile << s.get_food().size() << "#nb food " << endl;
		s.save(myfile);
		myfile.close();
	}
}

void Gui::on_button_clicked_Previous() 
{
	if (etat_courant==1) {
		etat_courant = 0;
		m_Label_None_selected.set_text("None selected");
	}else {
		if (etat_courant==0) {
			etat_courant = s.get_vector().size()+1;
		}
		--etat_courant;
		string food,nbC,nbD,nbP,id;
		id = to_string(etat_courant-1);
		food = to_string((s.get_vector()[etat_courant-1])->get_total_food());
		nbC = to_string((s.get_vector()[etat_courant-1])->get_nbC());
		nbD = to_string((s.get_vector()[etat_courant-1])->get_nbD());
		nbP = to_string((s.get_vector()[etat_courant-1])->get_nbP());
		m_Label_None_selected.set_text("id: "+id+ '\n' +"Total food: "+food+ '\n' +"nbC: "+nbC+ '\n' +"nbD: "
										+nbD+ '\n' +"nbP: "+nbP);
	}
}

void Gui::on_button_clicked_Next() 
{
	if (etat_courant== s.get_vector().size()) {
		etat_courant = 0;
		m_Label_None_selected.set_text("None selected");
	} else {
		++etat_courant;
		string food,nbC,nbD,nbP,id;
		id = to_string(etat_courant-1);
		food = to_string((s.get_vector()[etat_courant-1])->get_total_food());
		nbC = to_string((s.get_vector()[etat_courant-1])->get_nbC());
		nbD = to_string((s.get_vector()[etat_courant-1])->get_nbD());
		nbP = to_string((s.get_vector()[etat_courant-1])->get_nbP());
		m_Label_None_selected.set_text("id: "+id+ '\n' +"Total food: "+food+ '\n' +"nbC: "+nbC+ '\n' +"nbD: "
										+nbD+ '\n' +"nbP: "+nbP);
	}
		
}


void Gui::on_button_clicked_Step()
{
	if (! (timer_added)) {
		on_timeout();
	}

}


bool Gui::on_key_press_event(GdkEventKey * key_event)
{
	if(key_event->type == GDK_KEY_PRESS)
	{
		switch(gdk_keyval_to_unicode(key_event->keyval))
		{
			case 's':
				on_button_clicked_Start();
				break;
			case '1':
				on_button_clicked_Step();
				break;
			case 'n':
				on_button_clicked_Next();
				break;
			case 'p':
				on_button_clicked_Previous();
				break;
			case 'q':
				on_button_clicked_Exit();
		}
	}
	
	return Gtk::Window::on_key_press_event(key_event);
}

void Gui::on_button_clicked_Start()
{
  if(not timer_added)
  {	  
	  Glib::signal_timeout().connect( sigc::mem_fun(*this, &Gui::on_timeout),
									  timeout_value );
	  timer_added = true;
	  m_Button_Start.set_label("Stop");
  }
  else
  {
    m_Button_Start.set_label("Start");
	disconnect  = true;   
    timer_added = false;  
  }
}

bool Gui::on_timeout()
{

  if(disconnect)
  {
	  disconnect = false; 
	  
	  return false;  
  }
	 
	 if (s.get_erreur()) {
		m_Area.set_empty (true);
		m_Area.refresh();
	} 
	s.mise_a_jour();
	m_Area.set_sim(s);
	string nb_food;
	nb_food = to_string(s.get_food().size());
    m_Label_Nb_food.set_text("Nb food: "+nb_food);
    m_Area.refresh();

  return true; 
}

void Gui::set_simulation (Simulation sim) {
	s = sim;
}


MyArea::MyArea(Simulation simulation)
: sim(simulation),
  empty (false)
{}

MyArea::~MyArea()
{
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 			
									Frame frame)
{ 
	cr->translate(frame.width/2, frame.height/2);									
	
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
             -frame.height/(frame.yMax - frame.yMin));
 
	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}

void MyArea::adjustFrame()
{
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	
	frame.width  = width;  
	frame.height = height;
	   
    double new_aspect_ratio((double)width/height);         
    if( new_aspect_ratio > default_frame.asp) {   
	    frame.yMax = default_frame.yMax ;					
	    frame.yMin = default_frame.yMin ;	
				
	    double delta(default_frame.xMax - default_frame.xMin);		
	    double mid((default_frame.xMax + default_frame.xMin)/2);

	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta ;     
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta ;		  	  
    } else  { 
	    frame.xMax = default_frame.xMax ;
	    frame.xMin = default_frame.xMin ;
	
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);  

	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta ;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta ;		  	  
    } 
}


void MyArea::draw_frame(const Cairo::RefPtr<Cairo::Context>& cr)			
{

	cr->set_line_width(10.0);
	cr->set_source_rgb(0., 0.7, 0.2);
	cr->rectangle(0,0,get_allocation().get_width(),get_allocation().get_height());  
	cr->stroke();
}


bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	adjustFrame();
	orthographic_projection(cr, frame); 
	graphic_set_context(cr); 

	if(empty == true){
	dessin_monde(128);
	}
	else{
	dessin_monde(128);
	sim.dessiner();
	}

return true;
}


void MyArea::refresh()
{
	auto win = get_window();
	if(win) {
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						 get_allocation().get_height()); 
		win->invalidate_rect(r,false);
	}
}

void MyArea::set_empty (bool vrai) {
	empty = vrai;
}

void MyArea::set_sim (Simulation s) {
	sim = s;
}
