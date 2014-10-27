class TestObserver : EntityController
{
    void Initialize()
    {
        tm = 0.0f;
        //self.Rotate(1.0f,0.0f,0.0f,-35.0f);
        //self.Translate(0.0f,0.0f,-2.0f);
        self.SubscribeToInput();
        m_MoveSpeed = 100.0f;
        m_TurnSpeed = 90.0f;
    }
    void OnButtonDown(uint DeviceID,uint ButtonID)
    {
        Print("Dev: " + DeviceID + " Button: " + ButtonID + ".\n");
        if(ButtonID == 8) m_L2_Held = true;
        else if(ButtonID == 9) m_R2_Held = true;
        else if(ButtonID == 10) m_L1_Held = true;
        else if(ButtonID == 11) m_R2_Held = true;
        else if(ButtonID == 12) Game.SendMessage(SHUTDOWN);
        else if(ButtonID == 13) Game.ToggleDebugCullingDisplay();
    }
    void OnButtonUp(uint DeviceID,uint ButtonID)
    {
        Print("Dev: " + DeviceID + " Button: " + ButtonID + ".\n");
        if(ButtonID == 8) m_L2_Held = false;
        else if(ButtonID == 9) m_R2_Held = false;
        else if(ButtonID == 10) m_L1_Held = false;
        else if(ButtonID == 11) m_R2_Held = false;
    }
    void OnAxisMoved(uint DeviceID,uint AxisID,Scalar Pos,Scalar Delta)
    {
        if(AxisID > 10) return;
        //Print("Dev: " + DeviceID + " Axis: " + AxisID + " Pos: " + Pos + " Delta: " + Delta + ".\n");
        if(AxisID == 0) m_LeftAnalogPos.x = Pos;
        else if(AxisID == 1) m_LeftAnalogPos.y = Pos;
        else if(AxisID == 2) m_RightAnalogPos.x = Pos;
        else if(AxisID == 3) m_RightAnalogPos.y = Pos;
    }
    void Update(float dt)
    {
        if(m_LeftAnalogPos.Magnitude() > 0.1f)
        {
            self.Translate(self.GetOrientation() * Vector3(m_LeftAnalogPos.x,0.0f,m_LeftAnalogPos.y) * m_MoveSpeed * dt);
        }
        if(m_RightAnalogPos.Magnitude() > 0.1f)
        {
            self.Rotate(0.0f,1.0f,0.0f,-m_RightAnalogPos.x * m_TurnSpeed * dt);
            self.Rotate(1.0f,0.0f,0.0f,-m_RightAnalogPos.y * m_TurnSpeed * dt);
        }
        if(m_R2_Held) m_MoveSpeed *= 1.05f;
        if(m_L2_Held) m_MoveSpeed *= 0.95;
        if(m_MoveSpeed < 0.01f) m_MoveSpeed = 0.01f;
    }
    Scalar tm;
    Scalar m_MoveSpeed;
    Scalar m_TurnSpeed;
    bool m_L1_Held;
    bool m_R1_Held;
    bool m_L2_Held;
    bool m_R2_Held;
    Vector2 m_LeftAnalogPos;
    Vector2 m_RightAnalogPos;
};