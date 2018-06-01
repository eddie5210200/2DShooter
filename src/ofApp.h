#pragma once

#include "ofMain.h"
#include "Sprite.h"
#include "Emitter.h"
//Edmond Chew
// 4/9/18
// Use wasd, or arrow keys to move around
// Use z, x to move your orbiting turret around to shoot enemies.

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown } MoveDir;
class Emitter;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void checkCollisions();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y) {}
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button) {}
	void mouseEntered(int x, int y) {}
	void mouseExited(int x, int y) {}
	void windowResized(int w, int h) {}
	void dragEvent(ofDragInfo dragInfo) {}
	void gotMessage(ofMessage msg) {}
	void keyProcess();

	bool is_left_pressed, is_right_pressed, is_down_pressed, is_space_pressed, is_up_pressed;
	bool z_pressed, x_pressed;
	bool b_pressed; //experimental purposes
	vector<Emitter *> emitters; //main emitters for enemy
	vector<DeathEmitter *> death;
	vector<EnemyEmitter *> enemy;
	int numEmitters;
	int numDeath;
	int numEnemy;
	Emitter *gun;
	int score;
	int level;
	int life;
	float gameStartTime;
	bool gameOver = false;
	bool start;
	ofVec3f mouseLast;
	ofSoundPlayer loadSound;
	ofImage background;
};
