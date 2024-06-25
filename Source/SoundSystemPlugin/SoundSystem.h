#pragma once

#include "CoreMinimal.h"

#include "SoundSystem.generated.h"




class UAudioComponent;
class USoundEntryAsset;

static constexpr int32 INVALID_HANDLE_ID = -1;
class FSoundPlaySpec {
public:
	FSoundPlaySpec() 
		: instigator(nullptr), entryRef(nullptr), ownerRef(nullptr), componentRef(nullptr), ID(INVALID_HANDLE_ID), oneShot(false)
	{
	}
	FSoundPlaySpec(const ASoundSystem* system, const USoundEntryAsset* entry, const AActor* owner, UAudioComponent* component, int32 id, bool isOneShot)
		: instigator(system), entryRef(entry), ownerRef(owner), componentRef(component), ID(id), oneShot(isOneShot)
	{
	}

	FSoundPlaySpec(const FSoundPlaySpec& other) noexcept {
		*this = other;
	}
	FSoundPlaySpec& operator=(const FSoundPlaySpec& other) noexcept {
		if (this == &other)
			return *this;

		this->ID = other.ID;
		this->oneShot = other.oneShot;
		this->instigator = other.instigator;
		this->entryRef = other.entryRef;
		this->ownerRef = other.ownerRef;
		this->componentRef = other.componentRef;

		return *this;
	}

	FSoundPlaySpec(FSoundPlaySpec&& other) noexcept {
		*this = std::move(other);
	}
	FSoundPlaySpec& operator=(FSoundPlaySpec&& other) noexcept {
		if (this == &other)
			return *this;

		this->ID = std::move(other.ID);
		this->oneShot = std::move(other.oneShot);
		this->instigator = std::move(other.instigator);
		this->entryRef = std::move(other.entryRef);
		this->ownerRef = std::move(other.ownerRef);
		this->componentRef = std::move(other.componentRef);

		return *this;
	}

public:
	inline constexpr int32 GetID() const noexcept { return ID; }
	inline constexpr bool IsOneShot() const noexcept { return oneShot; }

public:
	const ASoundSystem* instigator;
	const USoundEntryAsset* entryRef;
	const AActor* ownerRef;
	UAudioComponent* componentRef;

private:
	int32 ID;
	bool oneShot;
};

USTRUCT(BlueprintType)
struct FSoundPlaySpecHandle {

	GENERATED_BODY()

public:
	//FSoundPlaySpecHandle()
	//	: instigator(nullptr), entryRef(nullptr), ownerRef(nullptr), componentRef(nullptr), valid(false), ID(INVALID_HANDLE_ID)
	//{
	//}
	//FSoundPlaySpecHandle(const ASoundSystem* system, const USoundEntryAsset* entry, const AActor* owner, const UAudioComponent* component, bool isValid, int32 id)
	//	: instigator(system), entryRef(entry), ownerRef(owner), componentRef(component), valid(isValid), ID(id)
	//{
	//}

	//FSoundPlaySpecHandle(const FSoundPlaySpecHandle& other) {
	//	*this = other;
	//}
	//FSoundPlaySpecHandle& operator=(const FSoundPlaySpecHandle& other) {
	//	if (this == &other)
	//		return *this;

	//	this->valid			= other.valid;
	//	this->ID			= other.ID;
	//	this->instigator	= other.instigator;
	//	this->entryRef		= other.entryRef;
	//	this->ownerRef		= other.ownerRef;
	//	this->componentRef	= other.componentRef;

	//	return *this;
	//}

	//FSoundPlaySpecHandle(FSoundPlaySpecHandle&& other) {
	//	*this = std::move(other);
	//}
	//FSoundPlaySpecHandle& operator=(FSoundPlaySpecHandle&& other) {
	//	if (this == &other)
	//		return *this;

	//	this->valid         = std::move(other.valid);
	//	this->ID		    = std::move(other.ID);
	//	this->instigator	= std::move(other.instigator);
	//	this->entryRef		= std::move(other.entryRef);
	//	this->ownerRef		= std::move(other.ownerRef);
	//	this->componentRef  = std::move(other.componentRef);

	//	return *this;
	//}

public:
	inline constexpr bool IsValid() const noexcept { return valid; }
	inline constexpr int32 GetID() const noexcept { return ID; }

public:
	const ASoundSystem* instigator;
	const USoundEntryAsset* entryRef;
	const AActor* ownerRef;
	const UAudioComponent* componentRef;

private:
	bool valid;
	int32 ID;
};


UCLASS(Abstract)
class ASoundSystem : public AActor {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	/// <summary>
	/// Plays a sound clip as described by the Sound entry.
	/// All details regarding this request are pulled directly from the Sound entry.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool PlaySFX(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept;

	/// <summary>
	/// Plays a sound clip world wide as described by the Sound entry.
	/// The sound play request will be ignored if there is an already an active request with the same data.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool PlaySFX2DOneShot(const USoundEntryAsset* entry, const AActor* outer) noexcept;

	/// <summary>
	/// Plays a sound clip world wide as described by the Sound entry.
	/// It will attempt to perform the request regardless if a similar request already exists.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool PlaySFX2D(const USoundEntryAsset* entry, const AActor* outer) noexcept;

	/// <summary>
	/// Plays a sound clip at designated position as described by the Sound entry.
	/// The sound play request will be ignored if there is an already an active request with the same data.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool PlaySFX3DOneShot(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept;

	/// <summary>
	/// Plays a sound clip at designated position as described by the Sound entry.
	/// It will attempt to perform the request regardless if a similar request already exists.
	UFUNCTION(BlueprintCallable)
	bool PlaySFX3D(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept;

	/// <summary>
	/// Plays a track as described by the Sound entry.
	/// All details regarding this request are pulled directly from the Sound entry.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	const FSoundPlaySpecHandle* PlayTrack(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept;

	/// <summary>
	/// Plays a sound clip world wide as described by the Sound entry.
	/// The sound play request will be ignored if there is an already an active request with the same data.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	const FSoundPlaySpecHandle* PlayTrack2DOneShot(const USoundEntryAsset* entry, const AActor* outer) noexcept;

	/// <summary>
	/// Plays a track world wide as described by the Sound entry.
	/// It will attempt to perform the request regardless if a similar request already exists.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	const FSoundPlaySpecHandle* PlayTrack2D(const USoundEntryAsset* entry, const AActor* outer) noexcept;

	/// <summary>
	/// Plays a track at a designated position as described by the sound entry.
	/// The sound play request will be ignored if there is an already an active request with the same data.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	const FSoundPlaySpecHandle* PlayTrack3DOneShot(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept;

	/// <summary>
	/// Plays a track at a designated position as described by the sound entry.
	/// It will attempt to perform the request regardless if a similar request already exists.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	const FSoundPlaySpecHandle* PlayTrack3D(const USoundEntryAsset* entry, const AActor* outer, const FVector& position) noexcept;

	/// <summary>
	/// Stops the active track that is associated with the passed handle.
	/// </summary>
	bool StopTrack(const FSoundPlaySpecHandle*& handle) noexcept;

	/// <summary>
	/// Pauses the active track that is associated with the passed handle.
	/// </summary>
	bool PauseTrack(const FSoundPlaySpecHandle*& handle) noexcept;

	/// <summary>
	/// Resume the active track that is associated with the passed handle.
	/// </summary>
	bool ResumeTrack(const FSoundPlaySpecHandle*& handle) noexcept;

public: //Utility
	inline constexpr float GetMasterVolume() const noexcept { return masterVolume; }
	inline constexpr float GetTrackVolume() const noexcept { return trackVolume; }
	inline constexpr float GetSFXVolume() const noexcept { return sfxVolume; }

	inline constexpr float& GetMasterVolume() noexcept { return masterVolume; }
	inline constexpr float& GetTrackVolume() noexcept { return trackVolume; }
	inline constexpr float& GetSFXVolume() noexcept { return sfxVolume; }

	inline int32 GetActiveTracksCount() const noexcept { return activeTracks.Num(); }
	inline int32 GetActiveSFXCount() const noexcept { return activeSFX.Num(); }
	inline int32 GetActiveSoundsCount() const noexcept { return GetActiveTracksCount() + GetActiveSFXCount(); }
	inline int32 GetAudioComponentsCount() const noexcept { return audioComponents.Num(); }

	void SetMasterVolume(float value) noexcept;
	void SetTrackVolume(float value) noexcept;
	void SetSFXVolume(float value) noexcept;
	
	bool IsSFXActive(const USoundEntryAsset* entry, const AActor* owner) const noexcept;
	bool IsTrackActive(const USoundEntryAsset* entry, const AActor* owner) const noexcept;
	bool IsTrackPaused(const USoundEntryAsset* entry, const AActor* owner) const noexcept;

	void StopAllTracks() noexcept;
	void StopAllSFX() noexcept;
	void StopAll() noexcept;

private: //Implementation
	bool PlaySFX2D_Internal(const USoundEntryAsset& entry, const AActor* owner, bool oneshot) noexcept;
	bool PlaySFX3D_Internal(const USoundEntryAsset& entry, const AActor* owner, const FVector& position, bool oneshot) noexcept;
	bool SetTrackPauseState_Internal(const FSoundPlaySpecHandle& handle, bool state) noexcept;
	const FSoundPlaySpecHandle* PlayTrack2D_Internal(const USoundEntryAsset& entry, const AActor* owner, bool oneshot) noexcept;
	const FSoundPlaySpecHandle* PlayTrack3D_Internal(const USoundEntryAsset& entry, const AActor* owner, const FVector& position, bool oneshot) noexcept;
	UAudioComponent* ProcessSFXPlayRequest(const USoundEntryAsset& entry, const AActor* owner, bool oneshot) noexcept;
	UAudioComponent* ProcessTrackPlayRequest(FSoundPlaySpecHandle*& handle, const USoundEntryAsset& entry, const AActor* owner, bool oneshot) noexcept;

private: //Creation
	void CreateInitialComponentsPool() noexcept;
	UAudioComponent* CreateNewAudioComponent() noexcept;
	FSoundPlaySpecHandle* RegisterNewSoundPlaySpecHandle(const FSoundPlaySpec& spec) noexcept;

private: //Removal
	bool RemoveDistributedHandle(const FSoundPlaySpecHandle* handle) noexcept;
	bool StopAndRemoveActiveTrack(const FSoundPlaySpecHandle* handle, const USoundEntryAsset& entry) noexcept;

private: //Utility -Internal
	UAudioComponent* GetAvailableAudioComponent() const noexcept;
	const FSoundPlaySpec* FindSFX(const USoundEntryAsset* entry, const AActor* owner) const noexcept;
	const FSoundPlaySpec* FindTrack(const USoundEntryAsset* entry, const AActor* owner) const noexcept;

private: //Audio
	void UpdateMasterVolume(const float& value) noexcept;
	void UpdateTrackVolume(const float& value) noexcept;
	void UpdateSFXVolume(const float& value) noexcept;

private: //ID
	inline void IncrementIDCounter() noexcept { currentIDCounter++; }

private: //Settings Setup
	void SetupSFXSettings(UAudioComponent& component, const USoundEntryAsset& entry) const noexcept;
	void SetupTrackSettings(UAudioComponent& component, const USoundEntryAsset& entry) const noexcept;
	void Setup2DSettings(UAudioComponent& component, const USoundEntryAsset& entry) const noexcept;
	void Setup3DSettings(UAudioComponent& component, const USoundEntryAsset& entry, const FVector& position) const noexcept;

private: //Callbacks
	UFUNCTION()
	void SoundRequestCompletedCallback() noexcept;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundSystem|Volume", meta = (AllowPrivateAccess = "true"))
	float masterVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundSystem|Volume", meta = (AllowPrivateAccess = "true"))
	float trackVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundSystem|Volume", meta = (AllowPrivateAccess = "true"))
	float sfxVolume = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundSystem|Pool", meta = (AllowPrivateAccess = "true"))
	int32 initialPoolSize = 12;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundSystem|Pool", meta = (AllowPrivateAccess = "true"))
	int32 poolMaxSize = 64;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SoundSystem|Debugging", meta = (AllowPrivateAccess = "true"))
	TArray<UAudioComponent*> audioComponents;

private:
	TArray<FSoundPlaySpec> activeTracks;
	TArray<FSoundPlaySpec> activeSFX;
	TArray<FSoundPlaySpecHandle> distributedHandles;
	int32 currentIDCounter = 0;
};
