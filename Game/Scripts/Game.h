#pragma once
#include "Core/Object.h"
#include "Transform/Vector.h"

class Game : public Object
{
public:
	Game();
	~Game();

	void Start() override;
	void Restart() override;
	void FixedUpdate() override;
	void Update(float aDeltatime) override;
	void Delete() override;
	void Serialize(Storage& storage) override;

	void RegisterMario();
	void RegisterEnemies();
	void SetPoweredUp(bool aPoweredUp);
	void ResizeCollider(size_t colliderIndex,size_t blockIndex);
	void StopMusic();

	ObjectHandle mario;
	ObjectHandle background;
	AudioCompHandle music;

	std::vector<ObjectHandle> coinBlocks;
	std::vector<ObjectHandle> bricksBlocks;
	//a floor is a row of blocks (this makes it possible to move without getting stuck on block edges)
	std::vector<PhysicsCompHandle> blockFloors;
	//all of the blocks that make up a floor
	std::vector<std::vector<ObjectHandle>> floorBlocksContainer;
	
	//enemies
	std::vector<ObjectHandle> enemies;


	bool first = true;
};

INTERFACE_START(Game)
PUBLIC_FUNCTION(RegisterMario, Game)
PUBLIC_FUNCTION(RegisterEnemies, Game)
PUBLIC_FUNCTION(SetPoweredUp, Game)
PUBLIC_FUNCTION(ResizeCollider, Game)
PUBLIC_FUNCTION(StopMusic, Game)
INTERFACE_END()