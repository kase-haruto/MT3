#pragma once
#include"Plane.h"
#include"Transform.h"
#include"Vector3.h"

/// <summary>
/// 立方体空間
/// </summary>
class CubeSpace{
	Transform transform_;
	Plane plane;
public:

	CubeSpace();
	~CubeSpace();

	void Initialize();
	void Update();
	void Draw();
};

