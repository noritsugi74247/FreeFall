#ifndef INCLUDED_SCENE
#define INCLUDED_SCENE


class Scene
{
protected:



    int  timer;          // �^�C�}�[
    int  start_timer;
    int  mission_timer;
    bool tutorial;
    Scene* nextScene;   // ���̃V�[��

public:
    float fade;
    float wide;
    float posX;
    int  state;          // ���
    const float fadein = 0.12f;
    const float wide_dis = 35.0f;

    Scene* execute();   // ���s����

    virtual void init()
    { // ����������
        state = 0;
        timer = 0;
        nextScene = nullptr;
        
        start_timer = 0;
        mission_timer = 0;

    };
    virtual void uninit() {};   // �I������
    virtual void update() {};   // �X�V����
    virtual void draw() {};   // �`�揈��

    void changeScene(Scene* scene) { nextScene = scene; }   // �V�[���ύX����
    Scene* getScene() const { return nextScene; }           // nextScene�̃Q�b�^�[

    int  get_state() { return state; }
    int  get_start_timer() { return start_timer; }
    void up_state() { state++; }
    void set_state(int set) { state = set; }

    void set_tutorial(bool set) { tutorial = set; }
};


class SceneManager
{
public:
    void execute(Scene*);  // ���s����
};


#endif // !INCLUDED_SCENE
