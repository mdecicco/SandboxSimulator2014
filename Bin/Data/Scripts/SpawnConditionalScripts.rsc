int main()
{
    if(Game.VarExists("PrintEntityCount"))
    {
        if(Game.GetVarb("PrintEntityCount")) Game.Spawn("Entities/Test/PeriodicEntityCountPrinter.plst");
    }
    if(Game.VarExists("PrintFramerate"))
    {
        if(Game.GetVarb("PrintFramerate")) Game.Spawn("Entities/Test/PeriodicFrameratePrinter.plst");
    }
    if(Game.VarExists("DelayedShutdownTime"))
    {
        if(Game.GetVarf("DelayedShutdownTime") > 0.0f) Game.Spawn("Entities/Test/DelayedShutdown.plst");
    }
    
    return 0;
}
