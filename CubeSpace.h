#pragma once
#include"Plane.h"
#include"Transform.h"
#include"Vector3.h"

/// <summary>
/// —§•û‘Ì‹óŠÔ
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

