class Sky : EntityController
{
    void Initialize()
    {
        self.SetTexParam(0,TEXTURE_MAG_FILTER,NEAREST);
    }
    void Update(float dt)
    {
        Entity@ Camera = Game.GetCurrentCamera();
        self.Identity();
        self.Scale(Camera.GetFar() * 1.9f);
        self.Translate(Camera.GetPosition());
        self.SetPrimitiveType(TRIANGLES);
    }
};
