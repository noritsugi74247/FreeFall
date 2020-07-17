#pragma once
#include "vector.h"

namespace mylib
{
    //==========================================================================
    //
    //      ViewSettingsクラス
    //
    //==========================================================================
    class ViewSettings
    {
    private:
        float   scaleFactor;    // 画面に描画されるイメージのスケール
        VECTOR2 vCenter;        // 画面の中心座標
        VECTOR2 vLeftTop;       // 画面左上の座標
        VECTOR2 vLeftBottom;    // 画面左下の座標

    public:
        ViewSettings();
        ~ViewSettings();

        void initialize();

        void setScaleFactor(float s);
        void setViewCenter(VECTOR2);
        void setViewLeftTop(VECTOR2);
        void setViewLeftBottom(VECTOR2);
        void setViewByPosition(VECTOR2, VECTOR2);

        float getScaleFactor() { return scaleFactor; }
        VECTOR2 getViewCenter() { return vCenter; }
        VECTOR2 getViewLeftTop() { return vLeftTop; }
        VECTOR2 getViewLeftBottom() { return vLeftBottom; }

        VECTOR2 worldToScreen(const VECTOR2&);
        VECTOR3 worldToScreen(const VECTOR3&);
        VECTOR2 screenToWorld(VECTOR2);
    };
}