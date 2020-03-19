// Physics.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "PhysicsPlugin.h"
#include "Logger.h"
#include "PhysicsComponent.h"
#include "ContactListener.h"

#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING

void PhysicsPlugin::Start()
{
	Logger::Add("Physics");

	b2Vec2 gravity(0.0f, 20.0f);
	world = std::make_shared<b2World>(gravity);
	contactListener = std::make_shared<PairContactListener>();
	world->SetContactListener(contactListener.get());
	PhysicsComponent::world = world;
	PhysicsComponent::contactListener = contactListener;
}

void PhysicsPlugin::FixedUpdate()
{
	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world->Step(timeStep, velocityIterations, positionIterations);
}

void PhysicsPlugin::Update(float aDeltatime)
{
	_CRT_UNUSED(aDeltatime);
}

void PhysicsPlugin::Delete()
{
	PhysicsComponent::bodies.clear();
	contactListener->collisionDetails.clear();
}

PLUGIN_END(PhysicsPlugin)