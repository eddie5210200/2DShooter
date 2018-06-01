#include "Sprite.h"
// Used code by Kevin M. Smith - CS 134 SJSU
// Edmond Chew


BaseObject::BaseObject() {
	trans = ofVec3f(0, 0, 0);
	scale = ofVec3f(1, 1, 1);
	rot = 0;
}

void BaseObject::setPosition(ofVec3f pos) {
	trans = pos;
}

ofVec3f BaseObject::getPosition() {
	return trans;
}

//
// Basic Sprite Object
//
Sprite::Sprite() {
	speed = 0;
	velocity = ofVec3f(0, 0, 0);
	velocity = ofVec3f(0, 0);
	hasTarget = false;
	lifespan = -1;      // lifespan of -1 => immortal 
	birthtime = 0;
	bSelected = false;
	haveImage = false;
	name = "UnamedSprite";
	width = 20;
	height = 20;
	wave = 0;
	angle = 0;
	oscillationMagnitude = 100;
	oscillationAngle = 0;
	radius = 1;
	haveRadius = false;
	gate = 0;
	targetAngle = 0;
}

// Return a sprite's age in milliseconds
//
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//  Set an image for the sprite. If you don't set one, a rectangle
//  gets drawn.
//
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}

void Sprite::setRadius(float rad) {
	radius = rad;
	haveRadius = true;
}

//  Render the sprite
//
void Sprite::draw() {
	ofPushMatrix();
	ofSetColor(255, 255, 255, 255);
	// draw image centered and add in translation amount
	//
	if (haveImage) {
		switch (angle) {
		case 0:
		case 180:
			ofTranslate(trans.x + oscillationMagnitude * sin(oscillationAngle), trans.y);
			break;
		case 90:
		case 270:
			ofTranslate(trans.x, trans.y + oscillationMagnitude * sin(oscillationAngle));
			break;
		default:
			break;
		}
		ofRotateZ(angle);
		image.draw(-width / 2.0, -height / 2.0);
	}
	else {
		//presets
		ofFill();
		ofTranslate(trans);
		//angle
		if (hasTarget) ofRotateZ(targetAngle);
		else ofRotateZ(angle);
		//color
		if (type == 1) ofSetColor(255, 165, 0);
		else ofSetColor(255, 0, 0);


		//indicates explosion
		if (haveRadius) {
			if (gate > 0) {
				ofSetColor(0, 165, 0);
				ofDrawEllipse(-radius / 2.0, -radius / 2.0, radius, radius);
			}
			else {

				haveRadius = false;
			}
		}
		else {
			// in case no image is supplied, draw something.
			// 
			ofDrawEllipse(-width / 2.0, -height / 2.0, width, height);
		}
	}
	ofPopMatrix();
}



//  Add a Sprite to the Sprite System
//
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}

// remove all sprites within a given dist of point, return number removed
//
int SpriteSystem::removeNear(ofVec3f point, float dist) {
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;
	int count = 0;

	while (s != sprites.end()) {
		ofVec3f v = s->trans - point;
		if (v.length() < dist) {
			tmp = sprites.erase(s);
			count++;
			s = tmp;
		}
		else s++;
	}
	return count;
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteSystem::update() {

	if (sprites.size() == 0) return;

	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite and osciallation
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].trans += sprites[i].velocity / ofGetFrameRate();
		if (sprites[i].gate > 0) sprites[i].gate--;
		/*
		sprites[i].oscillationAngle += 1 / ofGetFrameRate();
		if (sprites[i].oscillationAngle > 360) sprites[i].oscillationAngle -= 360;
		*/
	}


}

//  Render all the sprites
//
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}
//set the target for the attack
void SpriteSystem::setTarget(ofVec3f target) {
	for (int i = 0; i < sprites.size(); i++) {
		if (!sprites[i].hasTarget) {
			//update new target
			sprites[i].target = target;
			sprites[i].hasTarget = true;
			//set velocity towards it
			sprites[i].targetAngle = (atan2f((sprites[i].trans.y - target.y), (sprites[i].trans.x - target.x)) - PI / 2) * 180 / PI;
			ofVec3f dir = ofVec3f(-(sprites[i].trans.x - target.x), -(sprites[i].trans.y - target.y));
			sprites[i].velocity = dir.getNormalized() * 200;
		}
	}
}
