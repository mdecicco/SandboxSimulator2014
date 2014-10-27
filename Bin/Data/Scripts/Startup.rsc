int main()
{
    int r = Game.Execute("Scripts/SpawnConditionalScripts.rsc");
    if(r != 0) { Print("Unable to spawn conditional scripts. Function exit code: " + r + ".\n"); return r; }
    
    return Game.Execute("Scripts/EngineSandbox.rsc");
}
