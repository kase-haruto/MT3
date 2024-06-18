#include "Vector3.h"
#include "Matrix4x4.h"
#include "Camera.h"
#include <string>

class OBB{
private:
    Vector3 center_;
    Vector3 orientation_[3];
    Vector3 rotation_;
    Vector3 size_; 
    uint32_t color_;
    Matrix4x4 matWorld_;

public:
    OBB();
    ~OBB();

    void Initialize(Vector3 center, Vector3 rotate, Vector3 size);
    void Update();
    void Draw(Camera* camera);
    void UpdateUI(std::string label);

    Vector3 GetCenter() const;
    Vector3* GetOrientation() const;
    Vector3 GetSize() const;
    Matrix4x4 GetMatWorld()const;
    void SetCenter(const Vector3 center);
    void SetOrientation(const Vector3* orientation);
    void SetSize(const Vector3 size);

    uint32_t GetColor() const;
    void SetColor(const uint32_t color);
};

