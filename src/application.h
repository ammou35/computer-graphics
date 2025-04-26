#pragma once

#include "ofMain.h"
#include "renderer.h"
#include "guiManager.h"

class Application : public ofBaseApp{

	public:
		Renderer renderer;

		float time_current;
		float time_last;
		float time_elapsed;

		bool is_key_press_up;
		bool is_key_press_down;
		bool is_key_press_left;
		bool is_key_press_right;
		bool is_key_press_q;
		bool is_key_press_w;
		bool is_key_press_e;
		bool is_key_press_a;
		bool is_key_press_s;
		bool is_key_press_d;

		void setup() override;
		void update() override;
		void draw() override;
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void set_ui_visible(const bool& is_visible);
		bool get_ui_visible() const;

	private:
		GuiManager guiManager;
		bool ui_visible;
};
