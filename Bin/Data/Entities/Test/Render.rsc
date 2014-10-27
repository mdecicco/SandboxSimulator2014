class Render : EntityController
{
    void Initialize()
    {
        SeedRandom(-1);
        Position = Vector3(30.0f,0.0f,0.0f);
        Target = Vector3(0.0f,0.0f,0.0f);
        Velocity = Vector3(Random(-1.0f,1.0f),Random(-1.0f,1.0f),Random(-1.0f,1.0f)) * 15.0f;
        tm = 0.0f;
        LastTime = 0.0f;
        Scale = 1.0f;
        self.SetPrimitiveType(TRIANGLE_STRIP);
    }
    void Update(float dt)
    {
        int vc = self.GetVertexCount();
        Scalar t = Game.GetRunTime();
        Scalar Delta = t - LastTime;
        tm += Delta;
        LastTime = t;
        t *= 0.5f;
        
        if(tm > 3.0f)
        {
            Target = Vector3(Random(-10.0f,10.0f),Random(-10.0f,10.0f),Random(-10.0f,10.0f));
            tm = 0.0f;
        }
        
        Vector3 tDirection = Target - Position;
        Scalar Distance = tDirection.Magnitude();
        tDirection.Normalize();
        
        Acceleration = tDirection * (400.0f / (Distance * Distance));
        Scalar pmag = Position.Magnitude();
        if(pmag > 80.0f) Acceleration += Position.Normalized() * -(5000.0f / (pmag * pmag));
        
        Velocity += Acceleration;
        
        self.AddVertex(Position * Scale);
        self.AddNormal(ColorFunc(t));
        Position += Velocity * Delta;
        self.AddVertex(Position * Scale);
        self.AddNormal(ColorFunc(t));
        
        Vector3 mDirection = Velocity.Normalized();
        Vector3 p1 = (Position + mDirection).Normalized();
        Vector3 s = Position.Normalized().Cross(p1).Normalized();
    
        /*
        self.AddVertex((Position + s) * Scale);
        self.AddNormal(ColorFunc(t));
        
        self.AddVertex((Position - s) * Scale);
        self.AddNormal(ColorFunc(t));
        */
        
        /*
        for(int i = 0;i < vc;i++)
        {
            Vector3 vp = self.GetVertex(i);
            self.SetVertex(i,vp + Vector3(Random(-1.0f,1.0f),Random(-1.0f,1.0f),Random(-1.0f,1.0f)) * 0.05f);
        }
        */
    }
    
    Scalar Scale;
    Scalar tm;
    Scalar LastTime;
    Vector3 Position;
    Vector3 Velocity;
    Vector3 Acceleration;
    Vector3 Target;
};