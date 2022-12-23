/*
 *  This Source Code Form is subject to the terms of the Mozilla Public License,
 *  v. 2.0. If a copy of the MPL was not distributed with this file, You can
 *  obtain one at http://mozilla.org/MPL/2.0/.
 *
 *  The original code is copyright (c) 2022, open.mp team and contributors.
 */

// Required for most of open.mp.
#include <sdk.hpp>

// This is the private implementation of the public interface.  We must know the interface.
#include "interface.hpp"

// If this data is to be used in other components only share an ABI stable base class.
class WeatherComponent final
	// This class is an implementation of the publicly shared `IWeatherComponent` interface.
	: public IWeatherComponent
	// The implementation includes player connection events to know when new players join.
	, public PlayerConnectEventHandler
	// The implementation includes pawn script events to know when new scripts load.
	, public PawnEventHandler
	// The implementation includes server tick events to periodically check for weather updates.
	, public CoreEventHandler
{
private:
	// Hold a reference to the main server core.
	ICore* core_ = nullptr;

	// We use the pawn componet to add and remove script load listeners.
	IPawnComponent* pawn_;

public:
	// Implementations of the various methods from the public API.
	IWeatherRegion* createWeatherRegion(StringView name, StringView location) override;
	
	void destroyWeatherRegion(IWeatherRegion*) override;
	
	IWeatherRegion* getWeatherRegion(StringView name) override;

	// Required component methods.
	StringView componentName() const override;

	SemanticVersion componentVersion() const override;

	void onLoad(ICore* c) override;

	void onInit(IComponentList* components) override;

	void onReady() override;

	void onFree(IComponent* component) override;

	void free() override;

	void reset() override;
	
	// Connect event methods.
	void onPlayerConnect(IPlayer& player) override;
	
	// Pawn event methods.
	void onAmxLoad(IPawnScript& script) override;

	void onAmxUnload(IPawnScript& script) override;

	// Update event methods.
	void onTick(Microseconds elapsed, TimePoint now) override;
	
	// More methods to be used only in this component, with more implementation details knowledge.

	// When this component is destroyed we need to tell any linked components this it is gone.
	~WeatherComponent();
};