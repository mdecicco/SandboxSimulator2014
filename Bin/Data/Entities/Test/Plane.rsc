class Plane : EntityController
{
    void Initialize()
    {
        Scalar Scale = 90;
        Scalar Tiling = -1;
        Segments = int(Scale);
        Scalar Step = Scale / (Scalar(Segments));
        Scalar CoordStep = (Step / Scale) * Tiling;
        Scalar Offset = -(Scale * 0.5f);
        self.SetPrimitiveType(POINTS);
        for(int x = 0;x < Segments;x++)
        {
            Scalar xCoord = x * CoordStep;
            for(int z = 0;z < Segments;z++)
            {
                Scalar yCoord = z * CoordStep;
                
                self.AddVertex((x * Step) + Offset,0,(z * Step) + Offset);
                self.AddNormal(0.0f,1.0f,0.0f);
                self.AddTexCoord(xCoord,yCoord);
            }
        }
        tm = 0.0f;
    }
    void Update(float dt)
    {
        tm += dt;
        if(tm < 15.0f)
        {
            for(int x = 0;x < Segments;x++)
            {
                for(int z = 0;z < Segments;z++)
                {
                    int idx = z + (x * Segments);
                    Vector3 p = self.GetVertex(idx);
                    
                    Scalar r = sin(tm * 0.1f) * 20.0f;
                    
                    Scalar t0 = sin(((x / Scalar(Segments)) * r) + (tm * 3.0f));
                    Scalar c0 = abs(sin(tm)) * 6.0f;
                    
                    Scalar t1 = sin(((z / Scalar(Segments)) * r) + (tm * 3.0f));
                    Scalar c1 = abs(sin(tm)) * 6.0f;
                    
                    Scalar t2 = sin((x / Scalar(Segments)) * 3.14159f);
                    Scalar c2 = 1.5f;
                    
                    p.y = (t0 * c0) + (t1 * c1) + (t2 * c2);
                    
                    self.SetVertex(idx,p);
                    self.SetNormal(idx,ColorFunc(p.y / (c0 + c1 + c2)));
                }
            }
        }
        else
        {
            for(int i = 0;i < self.GetVertexCount();i++) self.SetVertex(i,self.GetVertex(i) + Vector3(Random(-1.0f,1.0f),Random(-1.0f,1.0f),Random(-1.0f,1.0f)) * (tm - 15.0f));
        }
        //self.Rotate(0,1,0,22.5f * dt);
    }
    Scalar tm;
    int Segments;
};
