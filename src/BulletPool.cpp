#include "BulletPool.h"
//#include <iostream>
#include <vector>
//using namespace std;
//#include "Bullet.h"

BulletPool::BulletPool(int size) {
	for (int i = 0; i < size; i++) {
		Bullet* bullet = new Bullet();
		inactive.push_back(bullet);
		all.push_back(bullet);
	}
}

BulletPool::~BulletPool()
= default;

Bullet* BulletPool::Spawn() {
	Bullet* bullet = NULL;
	if (inactive.size() > 0) {
		bullet = inactive.back();
		bullet->Reset();
		bullet->active = true;
		inactive.pop_back();
		active.push_back(bullet);
	}
	return bullet;
}

void BulletPool::Despawn(Bullet* bullet) {
	bullet->active = false;
	inactive.push_back(bullet);
	for (std::vector<Bullet*>::iterator myiter = active.begin(); myiter != active.end(); myiter++) {
		if (*myiter == bullet) {
			active.erase(myiter);
			return;
		}
	}
}