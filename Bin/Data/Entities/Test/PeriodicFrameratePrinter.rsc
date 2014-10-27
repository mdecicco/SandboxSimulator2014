class PeriodicFrameratePrinter : EntityController
{
    void Update(float dt)
    {
        Print("Framerate: " + Game.GetFramerate() + " f/s.\n");
    }
};
