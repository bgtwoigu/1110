ʹ����֪; ���ļ�Ϊ�˽��Ӧ�õ���Դ�ļ��ظ������ı������⡣
����ļ�  res_rename.sh

ʹ��˵�� 
1. ��res_rename.sh����Ҫ���ĵ��ļ��з�ͬһ��Ŀ¼
2. �ļ���dos·���� ../res_rename.shִ�к����е��ļ����ֳ���mod�ȼ����ļ�����������ø�Ϊ$(Ŀ¼��)_$(�ļ���)
3. staticbrewapp.min ��������д  
CAH006_EFS_FILES = \
    $(notdir $(wildcard $(CAH006_DIRS)/*.xml)) \
    $(notdir $(wildcard $(CAH006_DIRS)/*.html)) \
    $(notdir $(wildcard $(CAH006_DIRS)/*.fwm))
$(foreach filename, $(CAH006_EFS_FILES), $(filename)|fs:/mod/cah006/$(subst cah006_,,$(filename))) 



������������ϵeric0593@gmail.com