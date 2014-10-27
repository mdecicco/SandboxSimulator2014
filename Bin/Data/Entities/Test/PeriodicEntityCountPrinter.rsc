class PeriodicEntityCountPrinter : EntityController
{
    void Update(float dt)
    {
        Print("Entity count: " + Game.GetEntityCount() + ".\n");
    }
};
