
// GJTalk.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GJTalk.h" 
#include "LoginFrame.h"
#include "WebFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "MessageFilterHelper.h"

// CGJTalkApp

BEGIN_MESSAGE_MAP(CGJTalkApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_THREAD_MESSAGE(DM_CROSSTHREAD_NOTIFY,&CGJTalkApp::OnCrossThreadNotify)
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

	CTrayIcon trayIcon;

	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
	CPaintManagerUI::SetResourcePath(_T("C:\\Users\\suppo_000\\Desktop\\GJTalkSkin\\GJTalk"));
	m_pContext=new GJContext;		

	m_pContext->init("localhost");

	HRESULT hr=::CoInitialize(NULL);





	MessageFilterHelper *msgFilter=new MessageFilterHelper(); 
	CPaintManagerUI::AddGlobalMessageFilter(msgFilter);

	m_pContext->GetLoginFrame().CenterWindow();
	m_pContext->GetLoginFrame().ShowWindow();

	CPaintManagerUI::MessageLoop();
	if(m_pContext)
		delete m_pContext;
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	return FALSE;
}

void CGJTalkApp::OnCrossThreadNotify(WPARAM wParam,LPARAM lParam)
{
	switch (wParam)
	{
	case 1:
		m_pContext->onConnect();
		break;
	case 2:
		m_pContext->onDisconnect((gloox::ConnectionError)lParam);
		break;
	case 3:
		m_pContext->onReceiveSelfVCard();
		break;
	default:
		break;
	}

}