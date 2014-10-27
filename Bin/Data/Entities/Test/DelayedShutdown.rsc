/*
 * Requires "DelayedShutdownTime" variable in Config.plst
 */

class DelayedShutdown : EntityController
{
    void Update(float dt)
    {
        float t0 = Game.GetRunTime();
        if(Game.VarExists("DelayedShutdownTime"))
        {
            float t1 = Game.GetVarf("DelayedShutdownTime");
            if(t0 >= t1) { Print("Automatic shutdown.\n"); Game.SendMessage(SHUTDOWN); }
        }
    }
};
