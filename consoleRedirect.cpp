//start-cmd����̨��ʼ��
	AllocConsole();                     //�򿪿���̨��������ʾ������Ϣ  
	SetConsoleTitleA("Debug Win Output");           //���ñ���  
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);      //��ȡ����̨������  
	INT hCrt = _open_osfhandle((INT)hCon, 2);     //ת��ΪC�ļ�������  
	
	FILE * hf = _fdopen(hCrt, "w");           //ת��ΪC�ļ���  
	setvbuf(hf, NULL, _IONBF, 0);              //�޻���  
	*stdout = *hf;                      //�ض����׼���
//end-cmd����̨��ʼ��