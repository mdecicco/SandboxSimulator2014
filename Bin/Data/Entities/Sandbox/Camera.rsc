class Camera
{
    void Initialize()
    {
        tm = 0.0f;
        self.SubscribeToInput();
        m_MoveSpeed = 300.0f;
        m_TurnSpeed = 900.0f;
        self.SetFar(1000.0f);
        self.SetGravity(Vector3(0,0,0));
        self.SetSleepWhenInactive(false);
        m_UseController = false;
        m_DoShoot = false;
        m_ShootSpd = 10.0f;
    }
    void ShootSphere()
    {
        Entity@ E = Game.Spawn("Entities/Sandbox/Sphere.plst",self.GetPosition() + (self.GetOrientation() * Vector3(0.0f,0.0f,-1.0f)));
        //E.ApplyImpulse(self.GetOrientation() * Vector3(0.0f,0.0f,-1000.0f),Vector3(0,0,0));
        Event e(45);
        e.AddParamv3(self.GetOrientation() * Vector3(0.0f,0.0f,-800.0f));
        e.AddParamv3(Vector3(0,0,0));
        E.Send(e);
    }
    void OnEvent(const Event& in E)
    {
    }
    void OnKeyDown(int Key)
    {
        if(Key == 0) Game.SendMessage(SHUTDOWN);
        if(m_UseController) return;
        if(Key == 32)
        {
            m_DoShoot = true;
        }
        else if(Key == 87) //w
        {
            m_LeftAnalogPos.y = -0.5f;
        }
        else if(Key == 83) //s
        {
            m_LeftAnalogPos.y = 0.5f;
        }
        else if(Key == 65) //a
        {
            m_LeftAnalogPos.x = -0.5f;
        }
        else if(Key == 68) //d
        {
            m_LeftAnalogPos.x = 0.5f;
        }
        else if(Key == 9) //up
        {
            m_R2_Held = true;
        }
        else if(Key == 8) //down
        {
            m_L2_Held = true;
        }
        else if(Key == 73) //i
        {
            m_AddTime = true;
        }
        else if(Key == 75) //k
        {
            m_SubTime = true;
        }
        else if(Key == 80) //p
        {
            Game.ToggleDebugPhysicsDisplay();
        }
    }
    void OnKeyUp(int Key)
    {
        if(m_UseController) return;
        if(Key == 32)
        {
            m_DoShoot = false;
        }
        else if(Key == 87) //w
        {
            m_LeftAnalogPos.y = 0.0f;
        }
        else if(Key == 83) //s
        {
            m_LeftAnalogPos.y = 0.0f;
        }
        else if(Key == 65) //a
        {
            m_LeftAnalogPos.x = 0.0f;
        }
        else if(Key == 68) //d
        {
            m_LeftAnalogPos.x = 0.0f;
        }
        else if(Key == 9) //up
        {
            m_R2_Held = false;
        }
        else if(Key == 8) //down
        {
            m_L2_Held = false;
        }
        else if(Key == 73) //i
        {
            m_AddTime = false;
        }
        else if(Key == 75) //k
        {
            m_SubTime = false;
        }
    }
    void OnCursorMove(const Vector2& in Pos,const Vector2& in Delta)
    {
        if(m_UseController) return;
        m_CursorMoving = true;
        m_RightAnalogPos = Delta;
    }
    void OnGamepadAttached(uint DeviceID)
    {
        m_UseController = true;
    }
    void OnButtonDown(uint DeviceID,uint ButtonID)
    {
        //Print("Dev: " + DeviceID + " Button: " + ButtonID + ".\n");
        if(ButtonID == 8) m_L2_Held = true;
        else if(ButtonID ==  9) m_R2_Held = true;
        else if(ButtonID == 10) m_L1_Held = true;
        else if(ButtonID == 11) m_R1_Held = true;
        else if(ButtonID == 12) Game.SendMessage(SHUTDOWN);
        else if(ButtonID == 13) Game.ToggleDebugCullingDisplay();
        else if(ButtonID == 14) m_DoShoot = true;
        else if(ButtonID ==  4) m_AddTime = true;
        else if(ButtonID ==  6) m_SubTime = true;
    }
    void OnButtonUp(uint DeviceID,uint ButtonID)
    {
        if(ButtonID == 8) m_L2_Held = false;
        else if(ButtonID == 9) m_R2_Held = false;
        else if(ButtonID == 10) m_L1_Held = false;
        else if(ButtonID == 11) m_R1_Held = false;
        else if(ButtonID == 14) m_DoShoot = false;
        else if(ButtonID ==  4) m_AddTime = false;
        else if(ButtonID ==  6) m_SubTime = false;
    }
    void OnAxisMoved(uint DeviceID,uint AxisID,Scalar Pos,Scalar Delta)
    {
        m_CursorMoving = true;
        if(AxisID > 10) return;
        //Print("Dev: " + DeviceID + " Axis: " + AxisID + " Pos: " + Pos + " Delta: " + Delta + ".\n");
        if(AxisID == 0) m_LeftAnalogPos.x = Pos;
        else if(AxisID == 1) m_LeftAnalogPos.y = Pos;
        else if(AxisID == 2) m_RightAnalogPos.x = Pos;
        else if(AxisID == 3) m_RightAnalogPos.y = Pos;
    }
    void Update(float dt)
    {
        if(m_DoShoot)
        {
            m_ShootTmr += dt;
            if(m_ShootTmr >= 1.0f / m_ShootSpd) { ShootSphere(); m_ShootTmr = 0.0f; }
        }
        if(!m_CursorMoving && !m_UseController) m_RightAnalogPos = Vector2(0,0);
        if(m_LeftAnalogPos.Magnitude() > 0.1f)
        {
            self.SetLinearDamping(0.0f);
            Vector3 t = self.GetOrientation() * Vector3(m_LeftAnalogPos.x,0.0f,m_LeftAnalogPos.y) * m_MoveSpeed * dt;
            self.ApplyImpulse(t,Vector3(0,0,0));
            //self.Translate(t);
        }
        else self.SetLinearDamping(0.9999999f);
        Scalar rMag = m_RightAnalogPos.Magnitude();
        if(rMag > 0.1f && rMag < 30.0f)
        {
            self.SetAngularDamping(0.5f);
            self.SetAngularFactor(Vector3(1.0f,1.0f,1.0f));
            Quaternion q = self.GetOrientation();
            Vector3 x = (q * Vector3(1.0f,0.0f,0.0f)) * -m_RightAnalogPos.y * m_TurnSpeed * dt;
            Vector3 y = (q * Vector3(0.0f,1.0f,0.0f)) * -m_RightAnalogPos.x * m_TurnSpeed * dt;
            if(!m_UseController) { x *= 0.5f; y *= 0.5f; }
            //self.Rotate(0.0f,1.0f,0.0f,-m_RightAnalogPos.x * m_TurnSpeed * dt);
            //self.Rotate(1.0f,0.0f,0.0f, m_RightAnalogPos.y * m_TurnSpeed * dt);
            self.ApplyTorque(x + y);
        }
        else
        {
            self.SetAngularDamping(0.9999999f);
            self.SetAngularFactor(Vector3(0.0f,0.0f,0.0f));
        }
        if(m_R1_Held) m_ShootSpd  *= 1.05f;
        if(m_L1_Held) m_ShootSpd  *= 0.95f;
        if(m_R2_Held) m_MoveSpeed *= 1.05f;
        if(m_L2_Held) m_MoveSpeed *= 0.95f;
        if(m_MoveSpeed < 0.01f) m_MoveSpeed = 0.01f;
        m_CursorMoving = false;
        if(m_AddTime) Game.SetTimeOfDay(Game.GetTimeOfDay() + (1.0f / 240.0f));
        if(m_SubTime) Game.SetTimeOfDay(Game.GetTimeOfDay() - (1.0f / 240.0f));
    }
    Scalar tm;
    Scalar m_MoveSpeed;
    Scalar m_TurnSpeed;
    Scalar m_ShootTmr;
    Scalar m_ShootSpd;
    bool m_DoShoot;
    bool m_AddTime;
    bool m_SubTime;
    bool m_L1_Held;
    bool m_R1_Held;
    bool m_L2_Held;
    bool m_R2_Held;
    bool m_UseController;
    bool m_CursorMoving;
    Vector2 m_LeftAnalogPos;
    Vector2 m_RightAnalogPos;
};