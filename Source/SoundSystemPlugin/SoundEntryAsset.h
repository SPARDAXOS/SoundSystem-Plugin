#pragma once

#include "SoundEntryAsset.generated.h"


enum class EAudioFaderCurve : uint8;



UCLASS()
class USoundEntryAsset : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	USoundEntryAsset();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	FName key;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	TObjectPtr<USoundBase> file;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General")
	bool oneShot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Volume")
	float volumeMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Volume")
	float volumeMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pitch")
	float pitchMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pitch")
	float pitchMin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Track")
	bool isTrack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Track|FadeIn", meta = (EditCondition = "isTrack", DisplayAfter = "isTrack", EditConditionHides))
	bool fadeIn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Track|FadeIn", meta = (EditCondition = "fadeIn && isTrack", DisplayAfter = "fadeIn", EditConditionHides))
	float fadeInDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Track|FadeIn", meta = (EditCondition = "fadeIn && isTrack", DisplayAfter = "fadeInDuration", EditConditionHides))
	EAudioFaderCurve fadeInCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Track|FadeOut", meta = (EditCondition = "isTrack", DisplayAfter = "isTrack", EditConditionHides))
	bool fadeOut;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Track|FadeOut", meta = (EditCondition = "fadeOut && isTrack", DisplayAfter = "fadeOut", EditConditionHides))
	float fadeOutDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Track|FadeOut", meta = (EditCondition = "fadeOut && isTrack", DisplayAfter = "fadeOutDuration", EditConditionHides))
	EAudioFaderCurve fadeOutCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "3D")
	bool is3D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "3D", meta = (EditCondition = "is3D", DisplayAfter = "is3D", EditConditionHides))
	TObjectPtr<USoundAttenuation> AttenuationSettings;
};