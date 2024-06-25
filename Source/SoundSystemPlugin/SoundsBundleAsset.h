#pragma once

#include "SoundsBundleAsset.generated.h"



class USoundEntryAsset;

UCLASS()
class USoundsBundleAsset : public UPrimaryDataAsset {
	GENERATED_BODY()

public:
	inline const TArray<USoundEntryAsset*>& GetEntries() const noexcept { return entries; }
	const USoundEntryAsset* FindEntry(FName key);

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TArray<USoundEntryAsset*> entries;
};