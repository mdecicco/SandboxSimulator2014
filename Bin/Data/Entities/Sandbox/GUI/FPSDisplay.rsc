class FPSDisplay : EntityController
{
    void Update(Scalar dt)
    {
        self.ClearVertices();
        self.AddText("Fonts/CourierNew17px/CourierNew17px.fnt",Vector3(0,0,0),0,"" + Game.GetFramerate() + " f/s | " + Game.GetVisibleEntityCount() + "/" + Game.GetEntityCount() + ".");
    }
};
