#pragma once
//----------------------------------------------------------------------------------
//
// Basic Sprite Class
//
//
//  Kevin M. Smith - CS 134 SJSU
#include "ofMain.h"

// This is a base object that all drawable object inherit from
// It is possible this will be replaced by ofNode when we move to 3D
//
class BaseObject {
public:
	BaseObject();
	ofVec2f trans, scale;
	float	rot;
	bool	bSelected;
	void setPosition(ofVec3f);
	ofVec3f getPosition();
};

//  General Sprite class  (similar to a Particle)
//
class Sprite : public BaseObject {
public:
	Sprite();
	void draw();
	float age();
	void setImage(ofImage);
	void setRadius(float);
	float speed;    //   in pixels/sec
	ofVec3f velocity; // in pixels/sec
	ofVec3f target;	//target to go to
	float targetAngle;
	bool hasTarget; //has a valid target to go to
	ofImage image;
	float birthtime; // elapsed time in ms
	float lifespan;  //  time in ms
	float gate;		// change pattern
	string name;
	bool haveImage;
	float width, height;
	int angle;
	float oscillationMagnitude;
	float oscillationAngle;
	int wave;
	int type; //1-player, 2-enemy
	float radius;
	bool haveRadius;
};

//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteSystem : public BaseObject {
public:
	void add(Sprite);
	void remove(int);
	void update();
	int removeNear(ofVec3f point, float dist);
	void draw();
	void setTarget(ofVec3f);
	vector<Sprite> sprites;
};
