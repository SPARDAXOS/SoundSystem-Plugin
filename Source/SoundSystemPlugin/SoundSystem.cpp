
#include "SoundSystem.h"
#include "SoundsBundleAsset.h"
#include "SoundEntryAsset.h"

#include "Components/AudioComponent.h"


//Start
void ASoundSystem::BeginPlay() {
	Super::BeginPlay();

	CreateInitialComponentsPool();
}


//Public API
bool ASoundSystem::PlaySFX(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept {
	if (!entry || !outer)
		return false;

	if (entry->isTrack)
		return false;

	if (entry->is3D)
		return PlaySFX3D_Internal(*entry, outer, position, entry->oneShot);
	else
		return PlaySFX2D_Internal(*entry, outer, entry->oneShot);
}
bool ASoundSystem::PlaySFX2D(const USoundEntryAsset* entry, const AActor* outer) noexcept {
	if (!entry || !outer)
		return false;

	if (entry->isTrack || entry->is3D || entry->oneShot)
		return false;

	return PlaySFX2D_Internal(*entry, outer, false);
}
bool ASoundSystem::PlaySFX2DOneShot(const USoundEntryAsset* entry, const AActor* outer) noexcept {
	if (!entry || !outer)
		return false;

	if (entry->isTrack || entry->is3D || !entry->oneShot)
		return false;

	return PlaySFX2D_Internal(*entry, outer, true);
}
bool ASoundSystem::PlaySFX3D(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept {
	if (!entry || !outer)
		return false;

	if (entry->isTrack || !entry->is3D || entry->oneShot)
		return false;

	return PlaySFX3D_Internal(*entry, outer, position, false);
}
bool ASoundSystem::PlaySFX3DOneShot(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept {
	if (!entry || !outer)
		return false;

	if (entry->isTrack || !entry->is3D || !entry->oneShot)
		return false;

	return PlaySFX3D_Internal(*entry, outer, position, true);
}
const FSoundPlaySpecHandle* ASoundSystem::PlayTrack(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept {
	if (!entry || !outer)
		return nullptr;

	if (!entry->isTrack)
		return nullptr;

	if (entry->is3D)
		return PlayTrack3D_Internal(*entry, outer, position, entry->oneShot);
	else
		return PlayTrack2D_Internal(*entry, outer, entry->oneShot);
}
const FSoundPlaySpecHandle* ASoundSystem::PlayTrack2D(const USoundEntryAsset* entry, const AActor* outer) noexcept {
	if (!entry || !outer)
		return nullptr;

	if (!entry->isTrack || entry->is3D || entry->oneShot)
		return nullptr;

	return PlayTrack2D_Internal(*entry, outer, false);
}
const FSoundPlaySpecHandle* ASoundSystem::PlayTrack2DOneShot(const USoundEntryAsset* entry, const AActor* outer) noexcept {
	if (!entry || !outer)
		return nullptr;

	if (!entry->isTrack || entry->is3D || !entry->oneShot)
		return nullptr;

	return PlayTrack2D_Internal(*entry, outer, true);
}
const FSoundPlaySpecHandle* ASoundSystem::PlayTrack3D(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept {
	if (!entry || !outer)
		return nullptr;

	if (!entry->isTrack || !entry->is3D || entry->oneShot)
		return nullptr;

	return PlayTrack3D_Internal(*entry, outer, position, false);
}
const FSoundPlaySpecHandle* ASoundSystem::PlayTrack3DOneShot(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept {
	if (!entry || !outer)
		return nullptr;

	if (!entry->isTrack || !entry->is3D || !entry->oneShot)
		return nullptr;

	return PlayTrack3D_Internal(*entry, outer, position, true);
}
bool ASoundSystem::StopTrack(const FSoundPlaySpecHandle*& handle) noexcept {
	if (!handle || !handle->IsValid() || handle->instigator != this)
		return false;

	StopAndRemoveActiveTrack(handle, *handle->entryRef);
	RemoveDistributedHandle(handle);
	handle = nullptr; //Invalidate handle.
	return true;
}
bool ASoundSystem::PauseTrack(const FSoundPlaySpecHandle*& handle) noexcept {
	if (!handle)
		return false;

	return SetTrackPauseState_Internal(*handle, true);
}
bool ASoundSystem::ResumeTrack(const FSoundPlaySpecHandle*& handle) noexcept {
	if (!handle)
		return false;

	return SetTrackPauseState_Internal(*handle, false);
}


//Removal
bool ASoundSystem::RemoveDistributedHandle(const FSoundPlaySpecHandle* handle) noexcept {
	if (distributedHandles.Num() <= 0 || !handle || handle->instigator != this)
		return false;

	int targetIndex = -1;
	for (int i = 0; i < distributedHandles.Num(); i++) {
		FSoundPlaySpecHandle* currentHandle = &distributedHandles[i];
		if (currentHandle->GetID() == handle->GetID()) {
			targetIndex = i;
			break;
		}
	}

	if (targetIndex == -1)
		return false;

	distributedHandles.RemoveAt(targetIndex);
	return true;
}
bool ASoundSystem::StopAndRemoveActiveTrack(const FSoundPlaySpecHandle* handle, const USoundEntryAsset& entry) noexcept {
	if (activeTracks.Num() <= 0 || !handle || handle->instigator != this)
		return false;

	int targetIndex = -1;
	for (int i = 0; i < activeTracks.Num(); i++) {
		FSoundPlaySpec* spec = &activeTracks[i];
		if (spec->GetID() == handle->GetID()) {
			targetIndex = i;
			break;
		}
	}

	if (targetIndex == -1)
		return false;

	FSoundPlaySpec* targetSpec = &activeTracks[targetIndex];
	if (entry.fadeOut)
		targetSpec->componentRef->FadeOut(entry.fadeOutDuration, 0.0f, entry.fadeOutCurve);
	else
		targetSpec->componentRef->Stop();

	activeTracks.RemoveAt(targetIndex);

	return true;
}


//Settings Setup
void ASoundSystem::SetupSFXSettings(UAudioComponent& component, const USoundEntryAsset& entry) const noexcept {
	component.SetSound(entry.file);
	component.VolumeMultiplier = FMath::RandRange(entry.volumeMin, entry.volumeMax) * sfxVolume * masterVolume;
	component.PitchMultiplier = FMath::RandRange(entry.pitchMin, entry.pitchMax);
}
void ASoundSystem::SetupTrackSettings(UAudioComponent& component, const USoundEntryAsset& entry) const noexcept {
	component.SetSound(entry.file);
	component.VolumeMultiplier = FMath::RandRange(entry.volumeMin, entry.volumeMax) * trackVolume * masterVolume;
	component.PitchMultiplier = FMath::RandRange(entry.pitchMin, entry.pitchMax);
}
void ASoundSystem::Setup2DSettings(UAudioComponent& component, const USoundEntryAsset& entry) const noexcept {
	//Note: Its own function in case any additional settings are added in the future.
	component.bAllowSpatialization = false;
}
void ASoundSystem::Setup3DSettings(UAudioComponent& component, const USoundEntryAsset& entry, const FVector& position) const noexcept {
	//Note: Its own function in case any additional settings are added in the future.
	component.bAllowSpatialization = true;
	component.AttenuationSettings = entry.AttenuationSettings;
	component.SetWorldLocation(position);
}


//Processing
bool ASoundSystem::PlaySFX2D_Internal(const USoundEntryAsset& entry, const AActor* owner, bool oneshot) noexcept {
	UAudioComponent* targetComponent = ProcessSFXPlayRequest(entry, owner, oneshot);
	if (!targetComponent)
		return false;

	Setup2DSettings(*targetComponent, entry);
	targetComponent->Play();

	return true;
}
bool ASoundSystem::PlaySFX3D_Internal(const USoundEntryAsset& entry, const AActor* owner, const FVector& position, bool oneshot) noexcept {
	UAudioComponent* targetComponent = ProcessSFXPlayRequest(entry, owner, oneshot);
	if (!targetComponent)
		return false;

	Setup3DSettings(*targetComponent, entry, position);
	targetComponent->Play();

	return true;
}
bool ASoundSystem::SetTrackPauseState_Internal(const FSoundPlaySpecHandle& handle, bool state) noexcept {
	auto spec = FindTrack(handle.entryRef, handle.ownerRef);
	if (!spec || spec->componentRef->bIsPaused == state)
		return false;

	spec->componentRef->SetPaused(state);
	return true;
}
const FSoundPlaySpecHandle* ASoundSystem::PlayTrack2D_Internal(const USoundEntryAsset& entry, const AActor* owner, bool oneshot) noexcept {
	FSoundPlaySpecHandle* newSpecHandle = nullptr;
	UAudioComponent* targetComponent = ProcessTrackPlayRequest(newSpecHandle, entry, owner, oneshot);
	if (!targetComponent || !newSpecHandle)
		return nullptr;

	Setup2DSettings(*targetComponent, entry);

	if (entry.fadeIn)
		targetComponent->FadeIn(entry.fadeInDuration, targetComponent->VolumeMultiplier, 0.0f, entry.fadeInCurve);
	else
		targetComponent->Play();

	return newSpecHandle;
}
const FSoundPlaySpecHandle* ASoundSystem::PlayTrack3D_Internal(const USoundEntryAsset& entry, const AActor* owner, const FVector& position, bool oneshot) noexcept {
	FSoundPlaySpecHandle* newSpecHandle = nullptr;
	UAudioComponent* targetComponent = ProcessTrackPlayRequest(newSpecHandle, entry, owner, oneshot);
	if (!targetComponent || !newSpecHandle)
		return nullptr;

	Setup3DSettings(*targetComponent, entry, position);

	if (entry.fadeIn)
		targetComponent->FadeIn(entry.fadeInDuration, targetComponent->VolumeMultiplier, 0.0f, entry.fadeInCurve);
	else
		targetComponent->Play();

	return newSpecHandle;
}
UAudioComponent* ASoundSystem::ProcessSFXPlayRequest(const USoundEntryAsset& entry, const AActor* owner, bool oneshot) noexcept {
	if (oneshot) {
		if (FindSFX(&entry, owner))
			return nullptr;
	}

	UAudioComponent* availableComponent = GetAvailableAudioComponent();
	if (!availableComponent) {
		availableComponent = CreateNewAudioComponent();
		if (!availableComponent)
			return nullptr;
	}

	SetupSFXSettings(*availableComponent, entry);
	if (!availableComponent->OnAudioFinished.IsBound()) //For oneshot bug. 
		availableComponent->OnAudioFinished.AddDynamic(this, &ASoundSystem::SoundRequestCompletedCallback);
	activeSFX.Add({ this, &entry, owner, availableComponent, currentIDCounter, oneshot });
	IncrementIDCounter();

	return availableComponent;
}
UAudioComponent* ASoundSystem::ProcessTrackPlayRequest(FSoundPlaySpecHandle*& handle, const USoundEntryAsset& entry, const AActor* owner, bool oneshot) noexcept {
	if (oneshot) {
		if (IsTrackActive(&entry, owner))
			return nullptr;
	}

	UAudioComponent* availableComponent = GetAvailableAudioComponent();
	if (!availableComponent) {
		availableComponent = CreateNewAudioComponent();
		if (!availableComponent)
			return nullptr;
	}

	SetupTrackSettings(*availableComponent, entry);
	int32 index = activeTracks.Add({ this, &entry, owner, availableComponent, currentIDCounter, oneshot });
	handle = RegisterNewSoundPlaySpecHandle(activeTracks[index]);
	return availableComponent;
}


//Creation
void ASoundSystem::CreateInitialComponentsPool() noexcept {
	if (initialPoolSize <= 0)
		return;

	audioComponents.Reserve(initialPoolSize);
	for (int i = 0; i < initialPoolSize; i++) {
		if (!CreateNewAudioComponent())
			return;
	}
}
UAudioComponent* ASoundSystem::CreateNewAudioComponent() noexcept {
	if (audioComponents.Num() >= poolMaxSize) {
		UE_LOG(LogTemp, Warning, TEXT("Unable to create new audio component!\nConsider increasing the pool size."));
		return nullptr;
	}

	UAudioComponent* newComponent = NewObject<UAudioComponent>(this, "AudioSource" + audioComponents.Num());
	if (!newComponent)
		return nullptr;

	newComponent->bAutoActivate = false;
	newComponent->SetupAttachment(GetRootComponent());
	newComponent->RegisterComponent();

	audioComponents.Emplace(newComponent);

	return newComponent;
}
FSoundPlaySpecHandle* ASoundSystem::RegisterNewSoundPlaySpecHandle(const FSoundPlaySpec& spec) noexcept {

	//int32 handleIndex = distributedHandles.Add({ this, spec.entryRef, spec.ownerRef, spec.componentRef, true, currentIDCounter });
	FSoundPlaySpecHandle newHandle;

	int32 handleIndex = distributedHandles.Add(newHandle);
	IncrementIDCounter();
	return &distributedHandles[handleIndex];
}


//Utility
UAudioComponent* ASoundSystem::GetAvailableAudioComponent() const noexcept {
	if (audioComponents.IsEmpty())
		return nullptr;

	for (auto component : audioComponents) {
		if (!component->IsPlaying())
			return component;
	}

	return nullptr;
}
const FSoundPlaySpec* ASoundSystem::FindSFX(const USoundEntryAsset* entry, const AActor* owner) const noexcept {
	if (activeSFX.IsEmpty() || !entry || !owner)
		return nullptr;

	for (int i = 0; i < activeSFX.Num(); i++) {
		if (activeSFX[i].ownerRef == owner && entry == activeSFX[i].entryRef && activeSFX[i].IsOneShot())
			return &activeSFX[i];
	}

	return nullptr;
}
const FSoundPlaySpec* ASoundSystem::FindTrack(const USoundEntryAsset* entry, const AActor* owner) const noexcept {
	if (activeTracks.IsEmpty() || !entry || !owner)
		return nullptr;

	for (int i = 0; i < activeTracks.Num(); i++) {
		if (activeTracks[i].ownerRef == owner && entry == activeTracks[i].entryRef)
			return &activeTracks[i];
	}

	return nullptr;
}
void ASoundSystem::SetMasterVolume(float value) noexcept {
	if (masterVolume == value)
		return;

	UpdateMasterVolume(value);
	masterVolume = value;
}
void ASoundSystem::SetTrackVolume(float value) noexcept {
	if (trackVolume == value)
		return;

	UpdateTrackVolume(value);
	trackVolume = value;
}
void ASoundSystem::SetSFXVolume(float value) noexcept {
	if (sfxVolume == value)
		return;

	UpdateSFXVolume(value);
	sfxVolume = value;
}
bool ASoundSystem::IsSFXActive(const USoundEntryAsset* entry, const AActor* owner) const noexcept {
	if (FindSFX(entry, owner))
		return true;
	return false;
}
bool ASoundSystem::IsTrackActive(const USoundEntryAsset* entry, const AActor* owner) const noexcept {
	if (FindTrack(entry, owner))
		return true;
	return false;
}
bool ASoundSystem::IsTrackPaused(const USoundEntryAsset* entry, const AActor* owner) const noexcept {
	auto track = FindTrack(entry, owner);
	if (!track)
		return false;
	return track->componentRef->bIsPaused;
}


//Audio
void ASoundSystem::UpdateMasterVolume(const float& value) noexcept {
	if (activeTracks.IsEmpty() && activeSFX.IsEmpty())
		return;

	if (!activeTracks.IsEmpty()) {
		for (auto& track : activeTracks) {
			track.componentRef->VolumeMultiplier /= masterVolume;
			track.componentRef->VolumeMultiplier *= value;
			track.componentRef->AdjustVolume(0.0f, track.componentRef->VolumeMultiplier);
		}
	}

	if (!activeSFX.IsEmpty()) {
		for (auto& sfx : activeSFX) {
			sfx.componentRef->VolumeMultiplier /= masterVolume;
			sfx.componentRef->VolumeMultiplier *= value;
			sfx.componentRef->AdjustVolume(0.0f, sfx.componentRef->VolumeMultiplier);
		}
	}
}
void ASoundSystem::UpdateTrackVolume(const float& value) noexcept {
	if (activeTracks.IsEmpty())
		return;

	for (auto& track : activeTracks) {
		track.componentRef->VolumeMultiplier /= trackVolume;
		track.componentRef->VolumeMultiplier *= value;
		track.componentRef->AdjustVolume(0.0f, track.componentRef->VolumeMultiplier);
	}
}
void ASoundSystem::UpdateSFXVolume(const float& value) noexcept {
	if (activeSFX.IsEmpty())
		return;

	for (auto& sfx : activeSFX) {
		sfx.componentRef->VolumeMultiplier /= sfxVolume;
		sfx.componentRef->VolumeMultiplier *= value;
		sfx.componentRef->AdjustVolume(0.0f, sfx.componentRef->VolumeMultiplier);
	}
}


//Cleanup
void ASoundSystem::StopAllTracks() noexcept {
	if (activeTracks.Num() <= 0)
		return;

	for (auto& track : activeTracks)
		track.componentRef->Stop();

	activeTracks.Empty();
	distributedHandles.Empty();
}
void ASoundSystem::StopAllSFX() noexcept {
	if (activeSFX.Num() <= 0)
		return;

	for (auto& sfx : activeSFX)
		sfx.componentRef->Stop();

	activeSFX.Empty();
}
void ASoundSystem::StopAll() noexcept {
	StopAllTracks();
	StopAllSFX();
}


//Callbacks
void ASoundSystem::SoundRequestCompletedCallback() noexcept {
	int targetIndex = -1;
	for (int i = 0; i < activeSFX.Num(); i++) {
		if (!activeSFX[i].componentRef->IsPlaying()) {
			targetIndex = i;
			break;
		}
	}

	if (targetIndex != -1) {
		activeSFX[targetIndex].componentRef->OnAudioFinished.Clear();
		activeSFX.RemoveAt(targetIndex);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Unable to find SFX Request to remove!"));
}

