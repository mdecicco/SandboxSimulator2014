void SetSettings()
{
    Game.SetGameTimeRate(1.0f / 300.0f);
    Game.SetTimeOfDay(2.0f / 12.0f);
}
void CreateBaseScene()
{
    Game.Spawn("Entities/Sandbox/Camera.plst");
    Game.Spawn("Entities/Sandbox/Map.plst");
    Game.Spawn("Entities/Sandbox/Sky.plst");
    Game.Spawn("Entities/Sandbox/GUI/FPSDisplay.plst");
    Game.Spawn("Entities/Test/TestAnimation.plst");
}
int main()
{
    SetSettings();
    CreateBaseScene();
    return 0;
}