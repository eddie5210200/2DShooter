#pragma once
//----------------------------------------------------------------------------------
//
// Basic Sprite Emitter Class
//
// Used code by Kevin M. Smith - CS 134 SJSU
// Edmond Chew

#include "Sprite.h"


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public BaseObject {
public:
	Emitter(SpriteSystem *);
	void draw();
	void start();
	void stop();
	void setLifespan(float);    // in milliseconds
	void setVelocity(ofVec3f);  // pixel/sec
	void setChildVelocity(ofVec3f);
	void setChildAngle(int degree) { childSetAngle = degree; };
	void setChildImage(ofImage);
	void setChildSound(ofSoundPlayer);
	void setChildSize(float w, float h) { childWidth = w; childHeight = h; }
	void setImage(ofImage);
	void setRate(float);
	void setType(int i) { type = i; };
	float maxDistPerFrame();
	void update();
	SpriteSystem *sys;
	float rate;
	ofVec3f childVelocity;
	ofVec3f velocity;
	float lifespan;
	bool started;
	float lastSpawned;
	ofImage childImage;
	ofImage image;
	ofSoundPlayer sound;
	bool drawable;
	bool haveChildImage;
	bool haveSound;
	bool haveImage;
	float width, height;
	float childWidth, childHeight;
	int type; //1-player, 2-enemy
	int childSetAngle;
	int currentAngle, turretAngle;
	float orbDist;
	float explodeTimer;
	void setTarget(ofVec3f);
	bool spawnEnemy;
};

//exploding on enemy death, then fires at player's last location
class DeathEmitter : public Emitter {
public:
	DeathEmitter(SpriteSystem *);
	DeathEmitter(SpriteSystem *, ofPoint);
	int groupSize;
	bool explode;
	void spawn();
	void update();
	bool remove();
};

//on screen enemy that shoots
class EnemyEmitter : public Emitter {
public:
	EnemyEmitter(SpriteSystem *);
	EnemyEmitter(SpriteSystem *, ofPoint, ofPoint);
	void draw();
	void update();
	//extraneous for now
	void setChildOscillate(float f) { childOscillate = f; };
	ofPoint target;
	ofPoint player;
	bool firstMove;
	ofVec3f saveVelocity;
	int turn; //shooting direction
	bool move;
	float childOscillate;
	bool getPlayer();
	Emitter *emit[3];
	bool death(ofVec3f, float);
};