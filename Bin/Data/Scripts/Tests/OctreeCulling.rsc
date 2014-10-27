int main()
{
    Game.Spawn("Entities/Test/BasicObserver.plst");
    for(int i = 0;i < 150;i++) Game.Spawn("Entities/Test/Math.plst");
    Game.ToggleDebugCullingDisplay();
    return 0;
}
