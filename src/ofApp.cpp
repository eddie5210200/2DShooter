#include "ofApp.h"
#include "Emitter.h"
//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" to create a very
// minimal shooter video arcade game.
//
// Game will run for 200 seconds.  To start a new game, press mouse button
//
// 
// Used code by Kevin M. Smith - CS 134 SJSU
// Edmond Chew
//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(0, 0, 0);
	score = 0;
	life = 3;
	level = 1;
	start = true;
	// Record when the game starts - game will run for 10 sec
	//
	gameStartTime = ofGetElapsedTimeMillis();

	// Create and setup emitters  (you can play with parameters
	// I have another example program that creates a GUI for these
	// so with that you can get an idea of velocity/rate effects.
	//

	numEmitters = 8;
	for (int i = 0; i < numEmitters; i++) {
		//emitters default
		Emitter *invaders = new Emitter(new SpriteSystem());
		//invaders->setChildImage(ofImage("Enemy.png"));
		invaders->setLifespan(20000);
		invaders->setType(2);
		invaders->setRate(1);
		invaders->setChildSize(invaders->childWidth, invaders->childHeight);
		switch (i) {
			//Northern Emitters
		case 0:
			invaders->setPosition(ofVec3f(0, -30, 0));
			invaders->setVelocity(ofVec3f(100, 0, 0));
			invaders->setChildVelocity(ofVec3f(0, 100, 0));
			invaders->setChildAngle(0);
			break;
		case 1:
			invaders->setPosition(ofVec3f(ofGetWindowWidth(), -30, 0));
			invaders->setVelocity(ofVec3f(-100, 0, 0));
			invaders->setChildVelocity(ofVec3f(0, 100, 0));
			invaders->setChildAngle(0);
			break;
			//Southern Emitters
		case 2:
			invaders->setPosition(ofVec3f(0, ofGetWindowHeight() + 30, 0));
			invaders->setVelocity(ofVec3f(100, 0, 0));
			invaders->setChildVelocity(ofVec3f(0, -100, 0));
			invaders->setChildAngle(180);
			break;
		case 3:
			invaders->setPosition(ofVec3f(ofGetWindowWidth(), ofGetWindowHeight() + 30, 0));
			invaders->setVelocity(ofVec3f(-100, 0, 0));
			invaders->setChildVelocity(ofVec3f(0, -100, 0));
			invaders->setChildAngle(180);
			break;
			//Western Emitters
		case 4:
			invaders->setPosition(ofVec3f(-30, 0, 0));
			invaders->setVelocity(ofVec3f(0, 100, 0));
			invaders->setChildVelocity(ofVec3f(100, 0, 0));
			invaders->setChildAngle(270);
			break;
		case 5:
			invaders->setPosition(ofVec3f(-30, ofGetWindowHeight(), 0));
			invaders->setVelocity(ofVec3f(0, -100, 0));
			invaders->setChildVelocity(ofVec3f(100, 0, 0));
			invaders->setChildAngle(270);
			break;
			//Eastern Emitters
		case 6:
			invaders->setPosition(ofVec3f(ofGetWindowWidth() + 30, 0, 0));
			invaders->setVelocity(ofVec3f(0, 100, 0));
			invaders->setChildVelocity(ofVec3f(-100, 0, 0));
			invaders->setChildAngle(90);
			break;
		case 7:
			invaders->setPosition(ofVec3f(ofGetWindowWidth() + 30, ofGetWindowHeight(), 0));
			invaders->setVelocity(ofVec3f(0, -100, 0));
			invaders->setChildVelocity(ofVec3f(-100, 0, 0));
			invaders->setChildAngle(90);
			break;

		default:
			break;
		}
		emitters.push_back(invaders);

	}
	numEnemy = 0;
	gun = new Emitter(new SpriteSystem());
	loadSound.load("explode.wav");
	background.load("2DShooterBackground.png");
	//turret set
	gun->setPosition(ofVec3f(ofGetWindowWidth() / 2, ofGetWindowHeight() / 4 * 3, 0));
	gun->setChildVelocity(ofVec3f(0, -1000, 0));
	gun->setVelocity(ofVec3f(0, 0, 0));
	gun->setRate(20);
	gun->setLifespan(500);
	//gun->setImage(ofImage("Turret.png"));
	//gun->setChildImage(ofImage("Missile.png"));
	gun->setType(1);
	gun->setChildAngle(0);
	// start them up
	//


}

//--------------------------------------------------------------
//
void ofApp::update() {
	// processes any key movements
	//
	keyProcess();
	if (!start && !gameOver) {
		gun->update();
		for (int i = 0; i < numEmitters; i++) {
			emitters[i]->update();
            //spawn enemies at the emitters
			if (emitters[i]->spawnEnemy && emitters[i]->started) {
				emitters[i]->spawnEnemy = false;
				EnemyEmitter *e = new EnemyEmitter(new SpriteSystem(), emitters[i]->trans, ofPoint(ofGetWindowWidth() / 5 + ofRandom(4 * ofGetWindowWidth() / 5), ofGetWindowHeight() / 5 + ofRandom(4 * ofGetWindowHeight() / 5)));
				//e->setTarget(ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2));
				e->start();
				enemy.push_back(e);
				numEnemy++;
			}
		}
        //update enemies and get the player's location to figure its attack pattern
		for (int i = 0; i < numEnemy; i++) {
			if (enemy[i]->getPlayer())
				enemy[i]->player = (gun->trans);
			enemy[i]->update();

		}
        //update dead enemies' missiles
		for (int i = 0; i < numDeath; i++) {
			death[i]->update();
			death[i]->setTarget(gun->getPosition());
			if (death[i]->remove()) {
				death.erase(death.begin() + i);
				numDeath--;
			}
		}

		// check for collisions between missiles and invaders
		//  
		checkCollisions();

		// game runs for 20 seconds; check to see if over and stop
		// emitters.
		//
		float t = ofGetElapsedTimeMillis();
		if ((t - gameStartTime > (200 * 1000)) || life <= 0) {
			gameOver = true;
			gun->stop();
			for (int i = 0; i < numEmitters; i++) {
				emitters[i]->stop();
			}
		}
		//update difficulty by score
		if (score / (10 * level) >= 1) {
			level++;
			switch (level) {
			case 2:
                emitters[0]->setRate(2);
                emitters[1]->setRate(2);
                break;
			case 3:
				emitters[5]->start();
				emitters[7]->start();

                break;
			case 4:
				emitters[5]->setRate(2);
                emitters[7]->setRate(2);
                break;
			case 5:
				emitters[4]->start();
				emitters[6]->start();
                break;
            case 6:
                emitters[4]->setRate(2);
                emitters[6]->setRate(2);
                break;
            case 7:
                emitters[2]->start();
				emitters[3]->start();
                break;
			default:
				break;
			}
		}
	}
}


//--------------------------------------------------------------
void ofApp::draw() {
	if (start) {
        //intro screen
		ofDrawBitmapString("Click Spacebar to Begin\n Move with arrow keys or wasd. Z, X to rotate the turret.\n Enemy don't hurt only bullets do.", ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
	}
	else {
		background.draw(0, 0);
		// if game is over, just draw a label in middle of screen
		//
		if (gameOver) {
			ofDrawBitmapString("GAME OVER", ofPoint(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2));
		}
		else {
			gun->draw();
			for (int i = 0; i < numEmitters; i++) {
				emitters[i]->draw();
			}
			for (int i = 0; i < numDeath; i++) {
				death[i]->draw();
			}
			for (int i = 0; i < numEnemy; i++) {
				enemy[i]->draw();
			}
		}

		// draw current score
		//
		ofSetColor(255, 255, 255);
		string scoreText;
		scoreText += "Score: " + std::to_string(score) + "\n";
		scoreText += "Life: " + std::to_string(life);
		ofDrawBitmapString(scoreText, ofPoint(10, 20));
	}
}

//  This is a simple O(M x N) collision check
//  For each missle check to see which invaders you hit and remove them
//
void ofApp::checkCollisions() {

	// find the distance at which the two sprites (missles and invaders) will collide
	// detect a collision when we are within that distance.
	//
	for (int j = 0; j < numEnemy; j++) {
		//bullet on enemy collision
		float collisionDist = gun->childHeight / 2 + enemy[j]->height / 2;
		//
		for (int i = 0; i < gun->sys->sprites.size(); i++) {
			if (enemy[j]->death(gun->sys->sprites[i].trans, collisionDist)) {
				numDeath++;
				DeathEmitter *d = new DeathEmitter(new SpriteSystem(), enemy[j]->trans);
				loadSound.play();
				d->setLifespan(1000);
				d->start();
				death.push_back(d);
				score++;
				enemy[j]->setPosition(ofVec3f(-100, -100));
			}
		}
		//player on enemy collision
		//with enemy shots
		collisionDist = gun->height / 2 + enemy[j]->childHeight / 2;
		for (int i = 0; i < 3 && enemy[j]->firstMove; i++)
			if (enemy[j]->emit[i]->sys->removeNear(gun->trans, collisionDist) > 0)
				life--;
	}
	for (int j = 0; j < numDeath; j++) {
		//death shots
		float collisionDist = gun->height / 2 + death[j]->childHeight / 2;
		if (death[j]->sys->removeNear(gun->trans, collisionDist) > 0)
			life--;
	}
	if (life < 0) {
		life = 0;
		gameOver = true;
	}
}



//  support dragging of gun with mouse
//
void ofApp::mouseDragged(int x, int y, int button) {
	/*
	ofPoint mouse_cur = ofPoint(x, y);
	ofVec3f delta = mouse_cur - mouseLast;
	//keep in bounds
	if ((gun->trans.y + delta.y) < ofGetWindowHeight() && (gun->trans.y + delta.y) > 0 && (gun->trans.x + delta.x) < ofGetWindowWidth() && (gun->trans.x + delta.x) > 0) {
		gun->trans += delta;
		mouseLast = mouse_cur;
	}
	*/
}

//--------------------------------------------------------------
// Start game if not running; otherwise track mouse last coord
// 
void ofApp::mousePressed(int x, int y, int button) {
	/*
	if (!gun->started) {
		gameStartTime = ofGetElapsedTimeMillis();
		gameOver = false;
		gun->start();
		for (int i = 0; i < numEmitters; i++) {
			emitters[i]->start();
		}
		score = 0;
	}
	mouseLast = ofPoint(x, y);
	*/
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (gun->started) {
		//movements can be done with w, a, s, d or arrow keys.
		switch (key) {
		case OF_KEY_LEFT:
		case 'a':
			is_left_pressed = true;
			break;
		case OF_KEY_RIGHT:
		case 'd':
			is_right_pressed = true;
			break;
		case OF_KEY_UP:
		case 'w':
			is_up_pressed = true;
			break;
		case OF_KEY_DOWN:
		case 's':
			is_down_pressed = true;
			break;
		case 'x':
			x_pressed = true;
			break;
		case 'z':
			z_pressed = true;
			break;
		case 'b':
			b_pressed = true;
			break;
		default:
			break;
		}
	}
    if(key == ' '){
		start = false;
		emitters[0]->start();
		emitters[1]->start();
		gun->start();
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (gun->started) {
		switch (key) {
		case OF_KEY_LEFT:
		case 'a':
			is_left_pressed = false;
			break;
		case OF_KEY_RIGHT:
		case 'd':
			is_right_pressed = false;
			break;
		case OF_KEY_UP:
		case 'w':
			is_up_pressed = false;
			break;
		case OF_KEY_DOWN:
		case 's':
			is_down_pressed = false;

			break;
		case 'x':
			x_pressed = false;
			break;
		case 'z':
			z_pressed = false;
			break;
		default:
			break;
		}
	}
}
//------ Update position if key were used
void ofApp::keyProcess() {
	if (is_left_pressed || is_right_pressed || is_down_pressed || is_up_pressed || z_pressed || x_pressed) {
		float temp_x = 0, temp_y = 0;
		if (is_left_pressed) temp_x -= 5;
		if (is_right_pressed) temp_x += 5;
		if (is_down_pressed) temp_y += 5;
		if (is_up_pressed) temp_y -= 5;
		int ang = 0;
		if (z_pressed) ang -= 1;
		if (x_pressed) ang += 1;
		ofPoint updated = ofPoint(gun->trans.x + temp_x, gun->trans.y + temp_y);
		if (updated.x < 0 || updated.x > ofGetWindowWidth())
			updated.x = (updated.x < 0) ? updated.x = 0 : updated.x = ofGetWindowWidth();
		if (updated.y < 0 || updated.y > ofGetWindowHeight())
			updated.y = (updated.y < 0) ? updated.y = 0 : updated.y = ofGetWindowHeight();
		ofVec3f delta = updated - ofPoint(gun->trans.x, gun->trans.y);
		gun->trans += delta;
		gun->turretAngle += ang;
	}
	//testing explosion

}
