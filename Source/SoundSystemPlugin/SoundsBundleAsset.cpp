
#include "SoundsBundleAsset.h"
#include "SoundEntryAsset.h"




const USoundEntryAsset* USoundsBundleAsset::FindEntry(FName key) {
	if (entries.IsEmpty())
		return nullptr;

	for (const auto& entry : entries) {
		if (entry && entry->key == key)
			return entry;
	}

	return nullptr;
}