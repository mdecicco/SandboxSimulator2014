class MathTest : EntityController
{
    void Initialize()
    {
        tm = 0.0f;
        LastTime = 0.0f;
        self.SetPrimitiveType(POINTS);
        
        File@ fp = Game.OpenFile("Models/Bunny.obj","r");
        while(!fp.AtEnd())
        {
            char c = fp.GetC();
            if(c == 'v')
            {
                fp.GetC();
                Vector3 v = fp.ParseVector3() - Vector3(5.0f,0.0f,2.0f);
                self.AddVertex(v);
                self.AddNormal(ColorFunc(v.y / 5.0f));
            }
        }
        Game.CloseFile(fp);
        self.Scale(Random(1.0f,2.0f));
        self.Translate(Random(-1.0f,1.0f) * 500.0f,Random(-1.0f,1.0f) * 500.0f,Random(-1.0f,1.0f) * 500.0f);
    }
    void Update(float dt)
    {
        int vc = self.GetVertexCount();
        Scalar t = Game.GetRunTime();
        Scalar Delta = t - LastTime;
        tm += Delta;
        LastTime = t;
        
        /* Brute force (very slow)
        Matrix4 m = Quaternion(Vector3(Random(-1.0f,1.0f),Random(-1.0f,1.0f),Random(-1.0f,1.0f)),(sin(tm) + 2.0f) * 2.0f).ToMatrix();

        for(int i = 0;i < vc;i++)
        {
            Vector3 vp = self.GetVertex(i);
            self.SetVertex(i,m * vp);
        }
        */
        
        self.Rotate(Random(-1.0f,1.0f),Random(-1.0f,1.0f),Random(-1.0f,1.0f),90.0f * dt);
        self.Translate(self.GetOrientation() * Vector3(0.0f,0.0f,50.0f * dt));
    }

    Scalar tm;
    Scalar LastTime;
};