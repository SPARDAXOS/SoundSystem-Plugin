
#include "SoundEntryAsset.h"
#include "Components/AudioComponent.h"


USoundEntryAsset::USoundEntryAsset() {

	//General
	key = "None";
	file = nullptr;
	oneShot = false;

	//Volume
	volumeMax = 1.0f;
	volumeMin = 1.0f;

	//Pitch
	pitchMax = 1.0f;
	pitchMin = 1.0f;

	//Track
	isTrack = false;
	fadeIn = false;
	fadeInDuration = 1.0f;
	fadeInCurve = EAudioFaderCurve::Linear;
	fadeOut = false;
	fadeOutDuration = 1.0f;
	fadeOutCurve = EAudioFaderCurve::Linear;

	//3D
	is3D = false;
	AttenuationSettings = nullptr;
}