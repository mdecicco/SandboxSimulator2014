class Sphere : EntityController
{
    void Initialize()
    {
        self.SetTexParam(0,TEXTURE_MAG_FILTER,NEAREST);
    }
    void Update(Scalar dt)
    {
        //self.ApplyImpulse(Vector3(Random(-0.5f,0.5f),Random(-0.5f,0.5f),Random(-0.5f,0.5f)) * 200.0f,Vector3(0.0f,0.0f,0.0f));
    }
    void OnEvent(const Event& in E)
    {
        if(E.ID() == 45)
        {
            self.ApplyImpulse(E.GetParamv3(0),E.GetParamv3(1));
        }
    }
};
