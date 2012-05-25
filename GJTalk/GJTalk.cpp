
// GJTalk.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GJTalk.h" 
#include "LoginFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGJTalkApp

BEGIN_MESSAGE_MAP(CGJTalkApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGJTalkApp ����

CGJTalkApp::CGJTalkApp() 
{

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGJTalkApp ����

CGJTalkApp theApp;


// CGJTalkApp ��ʼ��

BOOL CGJTalkApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	// SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath()+_T("skin"));
	GJContext context;


	while(true)
	{
		context.init("localhost");
		CLoginFrame loginFrame(context);
		loginFrame.Create(NULL,_T("������"),UI_WNDSTYLE_FRAME,0L);
		loginFrame.CenterWindow();
		loginFrame.ShowModal();
		if(!context.isSignedIn())
			break;

		CMainFrame mainFrame(context);
		mainFrame.Create(NULL,_T("������"),UI_WNDSTYLE_FRAME,0L);
		mainFrame.ShowModal();
		if(context.MainFrameCloseReason!=GJContext::MainFrameCloseReasons::SwitchUser) 
			break;  
	}
	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

