#pragma once

#include "CoreMinimal.h"
#include "GridPathfindingParams.h"
#include "GridPainter/GridPainter.h"
#include "GridManager.generated.h"

class UGrid;

/**
*
*/
UCLASS(Transient)
class GRIDRUNTIME_API AGridManager : public AActor
{
	GENERATED_BODY()

public:
	AGridManager();
	virtual ~AGridManager();

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual void SetGridSize(float GridSize);

	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual float GetGridSize() const;

	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual int GetDistance(const UGrid* Start, const UGrid* Dest);

	/** Grid-based pathfinding */
	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual bool FindPath(const FGridPathFindingRequest& Request, TArray<UGrid*>& Result);

	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual void GetGridsByBound(const FBox& Bound, TArray<UGrid*>& Grids);

	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual void GetGridsByRange(UGrid* Center, int Range, TArray<UGrid*>& Grids);

	/** Get all grids belong to this coord */
	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual void GetGridsByCoord(const FIntVector& Coord, TArray<UGrid*>& Grids);

	/** if you don't need multi-level grid support, you can use this function, otherwise using GetGridsByCoord*/
	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual UGrid* GetGridByCoord(const FIntVector& Coord);

	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual UGrid* GetGridByPosition(const FVector& Position);

	UFUNCTION(BlueprintCallable, Category = "GridManager")
	virtual void ClearAllGridInfo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "GridManager")
	void GetNeighbors(UGrid* Center, TArray<UGrid*>& Grids);
	virtual void GetNeighbors_Implementation(UGrid* Center, TArray<UGrid*>& Grids);

	//////////////////////////////////////////////////////////////////////////
	// properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn=true), Category = "GridManager")
	TSubclassOf<UGridPathFinder> PathFinderClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn=true), Category = "GridManager")
	TSubclassOf<UGridInfo> GridInfoClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn=true), Category = "GridManager")
	TSubclassOf<UGridPainter> GridPainterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "GridManager")
	int32 TraceTestDistance;

	void SetGridPainter(TSubclassOf<UGridPainter> PainterClass);

	UGridPainter* GetGridPainter();

	void LineTraceTest(const FVector& Center, TArray<FHitResult>& Results);

protected:
	virtual void PostInitGridManager();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn=true), Category = "GridManager")
	float GridSize;

	UPROPERTY()
	UGridPainter* GridPainter;

#ifdef WITH_EDITOR
	friend class FEdModeGridEditor;
#endif
};
