//projet
//Bourgeat Fleur: 50%
//Huynh Melvyn: 50%

#include <iostream>
#include <gtkmm/application.h>
#include "simulation.h"
#include "gui.h"


using namespace std;

int main(int argc, char * argv[]) {
	if(argc != 2) {
		Simulation s;
		auto app = Gtk::Application::create();
		Gui error(s);
		app->run(error);
	} 
			 
	string filename(argv[1]);
	Simulation s(filename);
	s.lecture(s);
	
	auto app = Gtk::Application::create();

	Gui gui(s);

	//Shows the window and returns when it is closed.
	app->run(gui); 
	
	return EXIT_SUCCESS;
}
