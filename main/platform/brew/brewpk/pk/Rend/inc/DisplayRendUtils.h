#ifndef DISPLAYREND_UTILS_H
#define DISPLAYREND_UTILS_H
typedef enum
{
    DISPLAYREND_TYPE_FADE = 0,      // ����
    DISPLAYREND_TYPE_SHOW_HORZ,     // ˮƽ��ʾ
    DISPLAYREND_TYPE_SHOW_VERT,     // ��ֱ��ʾ
    DISPLAYREND_TYPE_MOVE_HORZ,     // ˮƽ�ƶ�
    DISPLAYREND_TYPE_MOVE_VERT,     // ��ֱ�ƶ�
    #if !defined(FEATURE_VERSION_W516) && !defined(FEATURE_VERSION_W208S) && !defined(FEATURE_VERSION_W027)
    DISPLAYREND_TYPE_FLY_HORZ,      // ˮƽ����
    DISPLAYREND_TYPE_FLY_VERT,      // ��ֱ����
    #endif
    DISPLAYREND_TYPE_ROTAT_HORZ,    // ˮƽ��ת
    DISPLAYREND_TYPE_ROTAT_VERT,    // ��ֱ��ת
    DISPLAYREND_TYPE_BLINDS_VERT,   // ��ֱ��Ҷ��
    DISPLAYREND_TYPE_BLINDS_INTER,  // �����Ҷ��
    DISPLAYREND_TYPE_GROUP,         // ��������ֵ
    DISPLAYREND_TYPE_RAND,          // ���
    DISPLAYREND_TYPE_ONEBYONE,      // ���
    DISPLAYREND_TYPE_MAX,
}DisplayRendType;

extern void DisplayRend_Enable(boolean bEnable);
extern void DisplayRend_Push(void);
extern void DisplayRend_Start(boolean bIn);
extern void DisplayRend_StartEx(DisplayRendType eType, boolean bIn, int nDelay);
extern void DisplayRend_SetConfig(DisplayRendType eType);
extern void DisplayRend_Done(void);
#endif //DISPLAYREND_UTILS_H