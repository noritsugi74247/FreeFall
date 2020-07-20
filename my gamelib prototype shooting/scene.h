#ifndef INCLUDED_SCENE
#define INCLUDED_SCENE


class Scene
{
protected:



    int  timer=0;          // タイマー
    int  start_timer=0;
    int  mission_timer=0;
    bool tutorial=false;
    Scene* nextScene=nullptr;   // 次のシーン

public:
    float fade=0.0f;
    float wide=0.0f;
    float posX=0.0f;
    int  state=0;          // 状態
    const float fadein = 0.12f;
    const float wide_dis = 35.0f;

    Scene* execute();   // 実行処理

    virtual void init()
    { // 初期化処理
        state = 0;
        timer = 0;
        nextScene = nullptr;
        
        start_timer = 0;
        mission_timer = 0;

    };
    virtual void uninit() {};   // 終了処理
    virtual void update() {};   // 更新処理
    virtual void draw() {};   // 描画処理

    void changeScene(Scene* scene) { nextScene = scene; }   // シーン変更処理
    Scene* getScene() const { return nextScene; }           // nextSceneのゲッター

    int  get_state() { return state; }
    int  get_start_timer() { return start_timer; }
    void up_state() { state++; }
    void set_state(int set) { state = set; }

    void set_tutorial(bool set) { tutorial = set; }
};


class SceneManager
{
public:
    void execute(Scene*);  // 実行処理
};


#endif // !INCLUDED_SCENE
