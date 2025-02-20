#pragma once
#include "Component.h"
#include "GameObject.h"

class Movement : public Component {
public:
	int speed = 10;

	GameObject g;


	Update() override {
		auto transform = g.GetComponent<Transform>();


	}

private:
};