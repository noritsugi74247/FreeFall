//------< インクルード >----------------------------------------------------------
#include "viewsetting.h"
#include "vector.h"
#include "mylib.h"

//******************************************************************************
//
//      ViewSettings
//
//******************************************************************************

namespace mylib
{
    //--------------------------------------------------------------
    //  コンストラクタ
    //--------------------------------------------------------------
    ViewSettings::ViewSettings()
    {
        initialize();
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    ViewSettings::~ViewSettings()
    {
    }

    //--------------------------------------------------------------
    //  初期設定
    //--------------------------------------------------------------
    void ViewSettings::initialize()
    {
        scaleFactor = 1.0f;
        setViewLeftTop(VECTOR2(0, 0));
    }

    //--------------------------------------------------------------
    //  スケールを設定
    //--------------------------------------------------------------
    void ViewSettings::setScaleFactor(float s)
    {
        scaleFactor = s;
    }

    //--------------------------------------------------------------
    //  画面中央のワールド座標を設定
    //--------------------------------------------------------------
    void ViewSettings::setViewCenter(VECTOR2 c)
    {
        vCenter = c;
#ifdef GAMELIB_PLUS_UP
        vLeftTop = vCenter + VECTOR2(-system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT / 2) / scaleFactor;
        vLeftBottom = vCenter + VECTOR2(-system::SCREEN_WIDTH / 2, -system::SCREEN_HEIGHT / 2) / scaleFactor;
#else
        vLeftTop = vCenter + VECTOR2(-system::SCREEN_WIDTH / 2, -system::SCREEN_HEIGHT / 2) / scaleFactor;
        vLeftBottom = vCenter + VECTOR2(-system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT / 2) / scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  画面左上のワールド座標を設定
    //--------------------------------------------------------------
    void ViewSettings::setViewLeftTop(VECTOR2 lt)
    {
        vLeftTop = lt;
#ifdef GAMELIB_PLUS_UP
        vCenter = vLeftTop + VECTOR2(system::SCREEN_WIDTH / 2, -system::SCREEN_HEIGHT / 2) / scaleFactor;
        vLeftBottom = vLeftTop + VECTOR2(0, -system::SCREEN_HEIGHT) / scaleFactor;
#else
        vCenter = vLeftTop + VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT / 2) / scaleFactor;
        vLeftBottom = vLeftTop + VECTOR2(0, system::SCREEN_HEIGHT) / scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  画面左下のワールド座標を設定
    //--------------------------------------------------------------
    void ViewSettings::setViewLeftBottom(VECTOR2 lb)
    {
        vLeftBottom = lb;
#ifdef GAMELIB_PLUS_UP
        vCenter = vLeftBottom + VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT / 2) / scaleFactor;
        vLeftTop = vLeftBottom + VECTOR2(0, system::SCREEN_HEIGHT) / scaleFactor;
#else
        vCenter = vLeftBottom - VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT / 2) / scaleFactor;
        vLeftTop = vLeftBottom - VECTOR2(0, system::SCREEN_HEIGHT) / scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  画面内の任意の座標のワールド座標を設定
    //--------------------------------------------------------------
    void ViewSettings::setViewByPosition(VECTOR2 screenPos, VECTOR2 worldPos)
    {
#ifdef GAMELIB_PLUS_UP
        screenPos.y = system::SCREEN_HEIGHT - screenPos.y;
        VECTOR2 lb = worldPos - screenPos / scaleFactor;
        setViewLeftBottom(lb);
#else
        VECTOR2 lt = worldPos - screenPos / scaleFactor;
        setViewLeftTop(lt);
#endif
    }

    //--------------------------------------------------------------
    //  ワールド座標をスクリーン座標に変換
    //--------------------------------------------------------------
    VECTOR2 ViewSettings::worldToScreen(const VECTOR2& wPos)
    {
#ifdef GAMELIB_PLUS_UP
        return (wPos - vLeftBottom) * scaleFactor;
#else
        return (wPos - vLeftTop) * scaleFactor;
#endif
    }

    VECTOR3 ViewSettings::worldToScreen(const VECTOR3& wPos)
    {
#ifdef GAMELIB_PLUS_UP
        return (wPos - vLeftBottom) * scaleFactor;
#else
        return (wPos - vLeftTop) * scaleFactor;
#endif
    }

    //--------------------------------------------------------------
    //  スクリーン座標をワールド座標に変換
    //--------------------------------------------------------------
    VECTOR2 ViewSettings::screenToWorld(VECTOR2 sPos)
    {
#ifdef GAMELIB_PLUS_UP
        sPos.y = system::SCREEN_HEIGHT - sPos.y;
        return vLeftBottom + sPos / scaleFactor;
#else
        return vLeftTop + sPos / scaleFactor;
#endif
    }

}
