


#include "ofMain.h"
#include "Emitter.h"

// Used code by Kevin M. Smith - CS 134 SJSU
// Edmond Chew

//  Create a new Emitter - needs a SpriteSystem
//
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = 3000;    // milliseconds
	started = false;
	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveSound = false;
	haveImage = false;
	velocity = ofVec3f(0, 20, 0);
	childVelocity = ofVec3f(0, 100, 0);
	drawable = true;
	width = 20;
	height = 20;
	childWidth = 5;
	childHeight = 10;
	type = 0;
	currentAngle = 0;
	turretAngle = 180;
	orbDist = height + 10;
	explodeTimer = 0;
}

//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	// draw sprite system
	//
	sys->draw();
	if (drawable) {
		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + trans.x, -image.getHeight() / 2.0 + trans.y);
		}
		else {

			//ofFill();
			float ang = turretAngle * PI / 180;
			switch (type) {
			case 1:
				//player
				ofPushMatrix();
				ofTranslate(trans);
				ofRotateZ(currentAngle);
				ofSetColor(255, 165, 0);
				ofDrawRectangle(-width / 2, -height / 2, width, height);
				ofPopMatrix();

				//turret
				ofPushMatrix();
				ofTranslate(-cosf(ang) * orbDist, -sinf(ang) * orbDist, ang);
				ofSetColor(255, 255, 255);
				ofDrawEllipse(trans.x, trans.y, width / 3, height / 3);

				ofPopMatrix();
				break;
			default:
				break;
			}
			//draw enemy

		}
	}

}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (!started) return;
	//flip the velocity if out of bounds
	if (getPosition().x < -50 || getPosition().x - 50 > ofGetWindowWidth() || getPosition().y < -50 || getPosition().y - 50 > ofGetWindowHeight())
		velocity *= ofVec3f(-1, -1, -1);
	//move emitters
	setPosition(getPosition() + velocity * 1 / 60);
	float time = ofGetElapsedTimeMillis();
	if (rate != 0 && (time - lastSpawned) > (1000.0 / rate)) {
		Sprite sprite;
		float ang = turretAngle * PI / 180;
		switch (type) {
		case 1:
			// spawn a new sprite
			if (haveChildImage) sprite.setImage(childImage);
			if (haveSound) sound.play();
			sprite.velocity = ofVec3f(childVelocity.y * cosf(ang), childVelocity.y * sinf(ang));
			sprite.lifespan = lifespan;
			sprite.type = 1;
			sprite.setPosition(ofVec3f(trans.x + width / 6 - cosf(ang) * orbDist, trans.y + height / 6 - sinf(ang) * orbDist));
			sprite.angle = (turretAngle + 90) % 360;
			sprite.birthtime = time;
			sprite.width = childWidth;
			sprite.height = childHeight;
			sys->add(sprite);
			lastSpawned = time;
			break;
		case 2:
			//spawn enemy
			spawnEnemy = true;
			lastSpawned = time;
			break;
		case 3:
			// spawn a new sprite
			if (haveChildImage) sprite.setImage(childImage);
			if (haveSound) sound.play();
			sprite.velocity = childVelocity;
			sprite.lifespan = lifespan;
			sprite.type = 3;
			sprite.setPosition(trans);
			sprite.angle = currentAngle;
			sprite.birthtime = time;
			sprite.width = childWidth;
			sprite.height = childHeight;
			sys->add(sprite);
			lastSpawned = time;
			break;
		}
	}
	sys->update();
	//revolving square
	if (type == 1) {
		currentAngle += 1;
		if (currentAngle > 360) currentAngle -= 360;
		if (turretAngle < 0 || turretAngle > 360) turretAngle %= 360;
	}
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}

void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(ofVec3f v) {
	velocity = v;
}

void Emitter::setChildVelocity(ofVec3f v) {
	childVelocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
	childWidth = img.getWidth();
	childHeight = img.getHeight();
}

void Emitter::setChildSound(ofSoundPlayer sound) {
	this->sound = sound;
	haveSound = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
	haveImage = true;
}
// Sets rate of spawn
void Emitter::setRate(float r) {
	rate = r;
}

float Emitter::maxDistPerFrame() {
	return  velocity.length() / ofGetFrameRate();
}

void Emitter::setTarget(ofVec3f target) {
	if (explodeTimer == 0) {
		sys->setTarget(target);
	}
}


//DeathEmitter
DeathEmitter::DeathEmitter(SpriteSystem *s) : Emitter(s) {
	explode = true;
	explodeTimer = 0;
	groupSize = 8;
}

DeathEmitter::DeathEmitter(SpriteSystem *s, ofPoint spawn) : Emitter(s) {
	setPosition(spawn);
	explode = true;
	explodeTimer = 0;
	groupSize = 8;
}
//spawning the explosion
void DeathEmitter::spawn() {
	Sprite sprite;
	ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1));
	float speed = velocity.length();
	sprite.velocity = dir.getNormalized() * speed;
	sprite.setPosition(trans);
	sprite.lifespan = lifespan * 20;
	sprite.gate = 100;
	sprite.birthtime = ofGetElapsedTimeMillis();
	sprite.setRadius(4);
	sprite.width = childWidth;
	sprite.height = childHeight;
	sprite.angle = currentAngle;

	// add to system
	//
	sys->add(sprite);
}

void DeathEmitter::update() {
	if (!started) return;
	//explode outwards
	float time = ofGetElapsedTimeMillis();
	if (explode) {
		// explode once
		for (int i = 0; i < groupSize; i++)
			spawn();
		explode = false;
		explodeTimer = 100;
	}
	else {
		//delay to fire
		if (explodeTimer > 0) explodeTimer--;
	}
	sys->update();
	if (sys->sprites.size() == 0) stop();
}

bool DeathEmitter::remove() {
	if (sys->sprites.size() == 0) {
		return true;
	}
	return false;
}

//EnemyEmitter
EnemyEmitter::EnemyEmitter(SpriteSystem *s) : Emitter(s) {
	//childOscillate = 100;
	target = ofPoint(-1, -1);
	move = false;
	//saveVelocity = ofVec3f(-(trans.x - target.x), -(trans.y - target.y));
	velocity = ofVec3f(0, 0, 0);
	turn = 0;
	type = 2;
	firstMove = false;
}

EnemyEmitter::EnemyEmitter(SpriteSystem *s, ofPoint spawn, ofPoint p) : Emitter(s) {
	//childOscillate = 100;
	target = p;
	player = ofPoint(-1, -1);
	setPosition(spawn);
	move = false;
	saveVelocity = ofVec3f(-(trans.x - target.x), -(trans.y - target.y));
	velocity = ofVec3f(0, 0, 0);
	turn = 0;
	type = 2;
	firstMove = false;
	childVelocity = ofVec3f(0, -100, 0);
}

void EnemyEmitter::draw() {
	if (drawable) {
		ofPushMatrix();
		if (move) {
			ofTranslate(trans.x, trans.y, turretAngle);
			//enemy 
			ofSetColor(0, 255, 0);
			ofDrawEllipse(-width / 2, -height / 2, width, height);
			ofPopMatrix();
		}
		else {
			//draw triangle enemy
			ofTranslate(trans.x, trans.y);
			//ofSetColor(0, 255, 0);
			//ofRotate(currentAngle);
			//ofDrawTriangle(-width, -height / 2, width, -height / 2, 0, height);
			ofSetColor(0, 255, 255);
			ofDrawEllipse(0, 0, width * 2, height * 2);
			ofPopMatrix();
		}
		ofPushMatrix();
		for (int i = 0; i < 3 && firstMove; i++) {
			emit[i]->draw();
		}
		ofPopMatrix();
	}
}

void EnemyEmitter::update() {
	if (!started) return;
	//extra: curve to point
	//circle to point
	if (move) {
		if (velocity == ofVec3f(0, 0, 0)) {
			velocity = saveVelocity.getNormalized() * 5;
			//printf("%f, %f\n", velocity.x, velocity.y);
		}
		trans += velocity;
		//first movement
		if (trans.distance(target) < 20 && !firstMove) {
			move = false;
			velocity = ofVec3f(0, 0, 0);
			firstMove = true;
			currentAngle = (atan2f((trans.y - player.y), (trans.x - player.x)) - PI / 2) * 180 / PI;
			//check the turning rad
			((int)round(ofRandom(1))) ? turn = 1 : turn = -1;
			turn *= 30;
			//create 3 emit points
			for (int i = 0; i < 3; i++) {
				Emitter *invaders = new Emitter(new SpriteSystem());
				invaders->setLifespan(20000);
				invaders->setType(3);
				invaders->setRate(200);
				invaders->setChildSize(invaders->childWidth, invaders->childHeight);
				invaders->setChildAngle(currentAngle);
				switch (i) {
				case 0:
					invaders->setPosition(ofVec3f(trans.x, trans.y));
					invaders->start();
					emit[0] = invaders;
					break;
				case 1:
					invaders->setPosition(ofVec3f(trans.x, trans.y));
					invaders->start();
					emit[1] = invaders;
					break;
				case 2:
					invaders->setPosition(ofVec3f(trans.x, trans.y));
					invaders->start();
					emit[2] = invaders;
					break;
				}
			}
		}
	}
	else {
		//emit +/- 30 degrees, x3
		if (turn < 0) {
			turn++;
			currentAngle++;
		}
		else if (turn > 0) {
			turn--;
			currentAngle--;
		}
		else {
			//turn off emitters
			for (int i = 0; i < 3 && firstMove; i++)
				emit[i]->setRate(0);
			move = true;
		}
	}
    //update the firing emitters
	for (int i = 0; i < 3 && firstMove; i++) {
		float ang = (currentAngle * PI / 180);
		emit[i]->setChildAngle(currentAngle);
		switch (i) {
		case 0:
			ang = (currentAngle * PI / 180);
			emit[i]->setPosition(ofVec3f(trans.x, trans.y));
			break;
		case 1:
			ang = (currentAngle * PI / 180) + PI - PI / 3;
			emit[i]->setPosition(ofVec3f(trans.x, trans.y ));
			break;
		case 2:
			ang = (currentAngle * PI / 180) + PI + PI / 3;
			emit[i]->setPosition(ofVec3f(trans.x, trans.y ));
			break;
		}
		emit[i]->setChildVelocity(ofVec3f(childVelocity.y * cosf(ang), childVelocity.y * sinf(ang)).getNormalized() * 200);
		emit[i]->update();
		//printf("%i\n", emit[i]->sys->sprites.size());
	}
}

//check if the player has been targeted
bool EnemyEmitter::getPlayer() {
	if (trans.distance(target) < 20 && !firstMove && player == ofPoint(-1, -1)) {
		return true;
	} return false;
}
//check if enemy is hit
bool EnemyEmitter::death(ofVec3f point, float dist) {
	ofVec3f v = trans - point;
	//hit
	if (v.length() < dist) {
		return true;
	}
	return false;
}
