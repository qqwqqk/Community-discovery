#include "adosql.h"
#include <iostream>
using namespace std;

void ADOLINK::Connect(void)
{
	HRESULT hr = CoInitialize(NULL);  //��ʼ��COM����
	try{
		m_pConnection.CreateInstance(__uuidof(Connection));  //�������Ӷ���
		//Link SQL server succeed
		//m_pConnection->ConnectionString = "Provider=SQLOLEDB.1;Password=1012;Persist Security Info=True;User ID=sa;Initial Catalog=SuperNet;Data Source=EYDAP9CXCGOF06L"; //�뽫���ݿ���ӦID��Password����		
		
		//Link Access 2007 succeed  
		m_pConnection->ConnectionString = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=database.accdb"; //�뽫���ݿ���ӦID��Password����

		//���ӷ����������ݿ�
		HRESULT hr = m_pConnection->Open("", "", "", 0);
		if (hr != S_OK)
			cout << "Can not connect to the specified database!" << endl;
	}
	catch (_com_error e){
		cout << e.Description() << endl;
	}
}
void ADOLINK::ExitConnect(void)
{
	if (m_pRecordset != NULL){
		m_pRecordset->Close();
		m_pConnection->Close();
	}
	::CoUninitialize();  //�ͷŻ���
}
_RecordsetPtr& ADOLINK::GetRecordset(_bstr_t SQL)
{
	m_pRecordset = NULL;
	ADOLINK link;
	try{
		if (m_pConnection == NULL)
			Connect();
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecordset->Open((_bstr_t)SQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error e){
		cout << e.Description() << endl;
		m_pRecordset = NULL;
		return m_pRecordset;
	}
	return m_pRecordset;
}