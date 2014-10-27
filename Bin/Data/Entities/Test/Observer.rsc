class TestObserver : EntityController
{
    void Initialize()
    {
        tm = 0.0f;
        self.Rotate(1.0f,0.0f,0.0f,-45.0f);
        self.Translate(0.0f,40,40);
    }
    void Update(float dt)
    {
        tm += dt;
        //self.Rotate(0.0f,1.0f,0.0f,4.0f);
        //self.Translate(0.0f,sin(tm * 0.1f) * 10.0f,sin(tm * 0.1f) * 10.0f);
    }
    Scalar tm;
};