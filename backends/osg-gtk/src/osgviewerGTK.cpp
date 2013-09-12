/*
  This file is part of the Modelica3D package.
  
  Copyright (C) 2012-current year  Christoph Höger and Technical University of Berlin

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

  Main Author 2010-2013, Christoph Höger
 */

#include <string.h>
#include <sys/time.h>

#include <iostream>
#include <string>
#include <osg/Stats>
#include <osgDB/ReadFile>
#include <osgGA/NodeTrackerManipulator>

#include "osggtkdrawingarea.h"
#include "osgviewerGTK.hpp"
#include "osg_interpreter.hpp"

/* Implementation based on OSG GTK Example code */

const char* HELP_TEXT =
		"Use CTRL or SHIFT plus right-click to pull menu\n"
		"\n"
		"<b>Modelica3D 2012</b>"
		;

class OSG_GTK_Mod3DViewer : public OSGGTKDrawingArea {
	GtkWidget* _menu;
	
	double currentTime;			// time since start of animation in s
	double tOffset;				// offset
	double timeScaler;          // scale time to slow things down or speed things up
	timeval startTime;			// to store start of simulation
	unsigned int _tid;

	const proc3d::animation_queue& stored_animation;
	proc3d::animation_queue animation;
	std::map<std::string, ref_ptr<PositionAttitudeTransform>> nodes;
	std::map<std::string, ref_ptr<Material>> materials;
	const osg::ref_ptr<osg::Group> scene_content;
	const proc3d_osg_interpreter interpreter;

	bool _setFocus(GtkWidget* widget) {
		std::string name(gtk_label_get_label(GTK_LABEL(gtk_bin_get_child(GTK_BIN(widget)))));
		if (nodes.count(name) == 0) {
			std::cerr << "cannot find node: " << name << std::endl;
			return false;
		}
		osg::PositionAttitudeTransform * node = nodes[name];
		osg::ref_ptr<osgGA::NodeTrackerManipulator> camTracker = new osgGA::NodeTrackerManipulator();
		osg::Vec3d pos = node->getPosition();
		camTracker->setHomePosition(pos + osg::Vec3d(1,1,1), pos, osg::Vec3d(0,0,1), false);
		camTracker->setTrackNode(node->getChild(0));
		camTracker->setTrackerMode(osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION);
		camTracker->setRotationMode(osgGA::NodeTrackerManipulator::TRACKBALL);
		setCameraManipulator(camTracker); 
		return true;
	}

	bool _clicked(GtkWidget* widget) {
		const char* text = gtk_label_get_label(
				GTK_LABEL(gtk_bin_get_child(GTK_BIN(widget)))
		);

		if(not strncmp(text, "Close", 5)) gtk_main_quit();

		else if(not strncmp(text, "Open File", 9)) {
			GtkWidget* of = gtk_file_chooser_dialog_new(
					"Please select an OSG file...",
					GTK_WINDOW(gtk_widget_get_toplevel(getWidget())),
					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL,
					GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN,
					GTK_RESPONSE_ACCEPT,
					NULL
			);

			if(gtk_dialog_run(GTK_DIALOG(of)) == GTK_RESPONSE_ACCEPT) {
				char* file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(of));

				osg::ref_ptr<osg::Node> model = osgDB::readNodeFile(file);

				if(model.valid()) {
					setSceneData(model.get());

					queueDraw();
				}

				g_free(file);
			}

			gtk_widget_destroy(of);
		}

		// Assume we're wanting FPS toggling.
		else {
			if(not _tid) {
				// start animation
				// get current time
				gettimeofday(&startTime, NULL);
				// instantiate timer
				_tid = g_timeout_add(
						15,  // 15ms equals roughly 60fps
						(GSourceFunc)(OSG_GTK_Mod3DViewer::timeout),
						this
				);

				gtk_button_set_label(GTK_BUTTON(widget), "Pause");
			}

			else {
				// pause animation
				tOffset = currentTime;
				g_source_remove(_tid);
				gtk_button_set_label(GTK_BUTTON(widget), "Start");

				_tid = 0;
			}
		}

		return true;
	}

protected:
	// Check right-click release to see if we need to popup our menu.
	bool gtkButtonRelease(double, double, unsigned int button) {
		if(button == 3 and (stateControl() or stateShift())) gtk_menu_popup(
				GTK_MENU(_menu),
				0,
				0,
				0,
				0,
				button,
				0
		);

		return true;
	}

	// Our "main" drawing pump. Since our app is just a model viewer, we use
	// click+motion as our criteria for issuing OpenGL refreshes.
	bool gtkMotionNotify(double, double) {
		if(stateButton()) queueDraw();

		return true;
	}

public:
	OSG_GTK_Mod3DViewer(const proc3d::animation_queue& q):
		OSGGTKDrawingArea (),
		_menu             (gtk_menu_new()),
		_tid              (0),
		stored_animation  (q),
		scene_content(new osg::Group()),
		interpreter(scene_content, nodes, materials),
		timeScaler(1.0) {
		scene_content->setName("root");

		gtk_widget_show_all(_menu);
		setSceneData(scene_content);
		getCamera()->setStats(new osg::Stats("omg"));


		restart_animation();
	}

	~OSG_GTK_Mod3DViewer() {}

	void setup_scene(const std::queue<proc3d::SetupOperation>& s) {
		std::queue<proc3d::SetupOperation> setup(s);
		while(!setup.empty()) {
			const proc3d::SetupOperation& op = setup.front();
			boost::apply_visitor( interpreter, op );
			setup.pop();
		}

		// add menu item for each object
		for(std::map<std::string, ref_ptr<PositionAttitudeTransform>>::iterator i = nodes.begin(); i!= nodes.end(); i++) {
			std::cout << "adding menu item for node: " << i->first << std::endl;
			GtkWidget* item = gtk_menu_item_new_with_label((i->first).c_str());
			gtk_menu_shell_append(GTK_MENU_SHELL(_menu), item);
			g_signal_connect(G_OBJECT(item), "activate", G_CALLBACK(OSG_GTK_Mod3DViewer::setFocus), this);
		}
		gtk_widget_show_all(_menu);

		/* activate first frame */
		currentTime = 0.0;
		if (animation.empty())
			tOffset = 0.0;
		else
			tOffset = proc3d::time_of(animation.top());		// only useful, if startTime != 0.0
		advance_animation();
	}

	void restart_animation() {
		animation = proc3d::animation_queue(stored_animation);
		currentTime = 0.0;
		if (animation.empty())
			tOffset = 0.0;
		else
			tOffset = proc3d::time_of(animation.top());		// only useful, if startTime != 0.0
		gettimeofday(&startTime, NULL);
	}

	void advance_animation() {
		timeval now;
		long seconds, useconds;

		// find out how many seconds have passed
		gettimeofday(&now, NULL);
	    seconds  = now.tv_sec  - startTime.tv_sec;
	    useconds = now.tv_usec - startTime.tv_usec;
		currentTime = tOffset + timeScaler*(seconds + 1e-6*useconds);

		// std::cout << "Update at t=" << currentTime << std::endl;

		if (animation.empty()) {
			restart_animation();
		} else {
			AnimOperation op = animation.top();
			// std::cout << "Anim command for t= " << proc3d::time_of(op) << std::endl;
			while (proc3d::time_of(op) <= currentTime && !animation.empty()) {
				boost::apply_visitor( interpreter, op );
				animation.pop();
				op = animation.top();
			}
		}

		queueDraw();
	}

	// Public so that we can use this as a callback in main().
	static bool clicked(GtkWidget* widget, gpointer self) {
		return static_cast<OSG_GTK_Mod3DViewer*>(self)->_clicked(widget);
	}
	
	// Public so that we can use this as a callback in main().
	static bool setFocus(GtkWidget* widget, gpointer self) {
		return static_cast<OSG_GTK_Mod3DViewer*>(self)->_setFocus(widget);
	}

	static bool timeout(void* self) {
		/* issue re-drawing */
		static_cast<OSG_GTK_Mod3DViewer*>(self) -> advance_animation();
		return true;
	}
};

int run_viewer(const proc3d::AnimationContext& context) {

	std::cout << "Starting GTK based viewer " << std::endl;
	std::cout << "Setup queue: " << context.setupOps.size() << " entries." << std::endl;
	std::cout << "Animation queue: " << context.deltaOps.size() << " entries." << std::endl;

	gtk_init(0, NULL);
	gtk_gl_init(0, NULL);

	OSG_GTK_Mod3DViewer da(context.deltaOps);
	da.setup_scene(context.setupOps);

	if(da.createWidget(640, 480)) {

		GtkWidget* window    = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		GtkWidget* vbox1     = gtk_vbox_new(false, 3);
		GtkWidget* vbox2     = gtk_vbox_new(false, 3);
		GtkWidget* hbox      = gtk_hbox_new(false, 3);
		GtkWidget* label     = gtk_label_new("");
		GtkWidget* buttons[] = {
				//gtk_button_new_with_label("Open File"),
				gtk_button_new_with_label("Start"),
				gtk_button_new_with_label("Close")
		};

		gtk_label_set_use_markup(GTK_LABEL(label), true);
		gtk_label_set_label(GTK_LABEL(label), HELP_TEXT);

		for(unsigned int i = 0; i < sizeof(buttons) / sizeof(GtkWidget*); i++) {
			gtk_box_pack_start(
					GTK_BOX(vbox2),
					buttons[i],
					false,
					false,
					0
			);

			g_signal_connect(
					G_OBJECT(buttons[i]),
					"clicked",
					G_CALLBACK(OSG_GTK_Mod3DViewer::clicked),
					&da
			);
		}

		gtk_window_set_title(GTK_WINDOW(window), "Modelica3D OSG - GTK Viewer");

		gtk_box_pack_start(GTK_BOX(hbox), vbox2, true, true, 2);
		gtk_box_pack_start(GTK_BOX(hbox), label, true, true, 2);

		gtk_box_pack_start(GTK_BOX(vbox1), da.getWidget(), true, true, 2);
		gtk_box_pack_start(GTK_BOX(vbox1), hbox, false, false, 2);

		gtk_container_set_reallocate_redraws(GTK_CONTAINER(window), true);
		gtk_container_add(GTK_CONTAINER(window), vbox1);

		g_signal_connect(
				G_OBJECT(window),
				"delete_event",
				G_CALLBACK(gtk_main_quit),
				0
		);

		gtk_widget_show_all(window);
		gtk_main();
	}

	else return 1;

	return 0;
}

extern "C" {

void* osg_gtk_alloc_context() {
	return new GTKAnimationContext();
}

void osg_gtk_free_context(void* context) {
	delete (GTKAnimationContext*) context;
}

}

