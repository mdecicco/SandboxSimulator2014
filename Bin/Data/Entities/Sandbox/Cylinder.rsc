class Cylinder : EntityController
{
    void Initialize()
    {
        self.SetTexParam(0,TEXTURE_MAG_FILTER,NEAREST);
    }
    void Update(Scalar dt)
    {
        self.ApplyImpulse(Vector3(Random(-0.5f,0.5f),Random(0.0f,0.5f),Random(-0.5f,0.5f)) * 150.0f,Vector3(0.0f,0.0f,0.0f));
    }
};
