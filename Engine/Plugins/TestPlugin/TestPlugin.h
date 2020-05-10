#include "PluginBase.h"

class TestPlugin : public PluginBase
{
public:
	void Start() override;
	void FixedUpdate() override;
	void Update(float deltatime) override;
	void Delete() override;
};