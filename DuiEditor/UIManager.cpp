#include "StdAfx.h"
#include "UIManager.h"

#include "DuiEditor.h"

CUIManager::CUIManager(void)
{
	m_nSplitterMode = SPLIT_UPDOWN;

	m_pDoc			= NULL;
	m_pCmdHistory	= new CDuiEditorCommandHistory;
	m_pCmdHistory->SetUIManager(this);
	m_pDesignView	= NULL;
	m_pManager		= NULL;
	m_pUiWindow		= NULL;
	m_pUiTracker	= NULL;
	m_pTreeView		= NULL;
}


CUIManager::~CUIManager(void)
{
	delete m_pCmdHistory; m_pCmdHistory = NULL;
}

BOOL CUIManager::IsUiWindow(xml_node node)
{
	if(CompareString(node.name(), _T("Window")))
		return TRUE;
	return FALSE;
}

xml_node CUIManager::FindContainer(xml_node node) //获取控件对应的容器
{
	xml_node nodeContainner = node.parent();
	while (nodeContainner)
	{
		if(g_duiProp.IsBaseFromContainer(nodeContainner.name()))
		{
			return nodeContainner;
		}
		nodeContainner = nodeContainner.parent();
	}
	
	return xml_node((xml_node_struct *)GetUiFrom()->GetTag());
}

BOOL CUIManager::SelectItem(xml_node node)
{
	CControlUI *pControlUI = (CControlUI *)node.get_tag();
//	if(!pControlUI)	return FALSE;
	return SelectItem(pControlUI);
}

BOOL CUIManager::SelectItem(CControlUI *pControl)
{
	if(!pControl)	
	{
		GetUiTracker()->RemoveAll();
		GetUiWindow()->Invalidate();
		return FALSE;
	}

	//往上回溯，碰到TabLayout都切换过来
	CControlUI *pControlTemp = pControl;
	CControlUI *pParent = pControl->GetParent();
	while(pParent)
	{
		CTabLayoutUI *pTabLayout = static_cast<CTabLayoutUI*>(pParent->GetInterface(DUI_CTR_TABLAYOUT));
		if(pTabLayout)
		{
			pTabLayout->SelectItem(pControlTemp);
		}
		pControlTemp = pParent;
		pParent = pParent->GetParent();
	}

	//判断当前按钮是否绑定了TabLayout 和 BindTabIndex
	if(pControl->GetInterface(DUI_CTR_BUTTON))
	{
		CButtonUI *pButton = static_cast<CButtonUI*>(pControl->GetInterface(DUI_CTR_BUTTON));
		CString strTabLayoutName = pButton->GetBindTabLayoutName();
		int nBindIndex = pButton->GetBindTabLayoutIndex();
		if(!strTabLayoutName.IsEmpty() && nBindIndex >= 0)
		{
			CTabLayoutUI *pTabLayout = dynamic_cast<CTabLayoutUI *>(GetManager()->FindControl(strTabLayoutName));
			if(pTabLayout)
			{
				pTabLayout->SelectItem(nBindIndex);
			}
		}
	}

	if(::GetKeyState(VK_CONTROL)>=0) GetUiTracker()->RemoveAll();
	GetUiTracker()->Add(xml_node((xml_node_struct *)pControl->GetTag()), pControl->GetPos());
	GetUiWindow()->Invalidate();
	return TRUE;
}

BOOL CUIManager::UpdateControlUI(CControlUI *pControl)
{
	xml_node nodeControl((xml_node_struct *)pControl->GetTag());

	xml_node nodeContainer = FindContainer(nodeControl);
	CContainerUI *pContainerUI = (CContainerUI *)nodeContainer.get_tag();
	if(pContainerUI != NULL)
	{
		pContainerUI->SetPos(pContainerUI->GetPos());
		pContainerUI->NeedUpdate();
	}
	else
	{
		pControl->SetPos(pControl->GetPos());
		pControl->NeedUpdate();
	}
	GetUiTracker()->UpdateRect(nodeControl);
	GetUiWindow()->Invalidate();

	//改动属性之后，即时刷新xmlpane
	if(GetCodeView())
	{
		CLockWindowUpdate lock(GetCodeView());
		GetCodeView()->LoadDocument();
		GetCodeView()->SelectXmlNode(pControl);
	}
	return TRUE;
}

BOOL CUIManager::UpdateControlPos(xml_node node, CRect rect, BOOL bUpdateWithHeight)
{
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl)	return FALSE;

	//改变主窗口大小
	if(pControl == GetUiFrom()) 
	{
		GetUiFrom()->SetInitSize(rect.Width(), rect.Height());
		SetScrollSize();

		CString temp;
		temp.Format(_T("%d,%d"), rect.Width(), rect.Height());
		g_duiProp.AddAttribute(node, _T("size"), temp, m_pDesignView);
		return TRUE;
	}

	//float控件
	if(pControl->IsFloat())
	{
		if(!pControl->GetParent())	return FALSE;

		CRect rcParent = pControl->GetParent()->GetPos();
		CRect rcControl;
		rcControl.left = rect.left - rcParent.left;
		rcControl.right = rcControl.left + rect.Width();
		rcControl.top = rect.top - rcParent.top;//rcParent.top - rect.top;//rect.top - rcParent.top;
		rcControl.bottom = rcControl.top + rect.Height();

		pControl->SetAttribute(_T("pos"), RectToString(rcControl)); //不能调用pControl->SetPos();
		UpdateControlUI(pControl);

		CRect rcNewPos = rcControl; //pControl->GetPos();
		g_duiProp.AddAttribute(node, _T("pos"), rcNewPos, m_pDesignView);
		if(bUpdateWithHeight)
		{
			g_duiProp.AddAttribute(node, _T("width"), rcNewPos.Width(), NULL);
			g_duiProp.AddAttribute(node, _T("height"), rcNewPos.Height(), NULL);
		}

		return TRUE;
	}

	//非float控件比较特殊, UI库自动布局.
	CDuiRect rcDoc(node.attribute(_T("pos")).as_string(_T("0,0,0,0")));
	rcDoc.left = 0;
	rcDoc.right = rect.Width();
	rcDoc.top = 0;
	rcDoc.bottom = rect.Height();

	pControl->SetAttribute(_T("pos"), rcDoc.ToString());	//不能调用pControl->SetPos();
	UpdateControlUI(pControl);

	if(bUpdateWithHeight)
	{
		g_duiProp.AddAttribute(node, _T("pos"), rcDoc, m_pDesignView);
		g_duiProp.AddAttribute(node, _T("width"), rcDoc.GetWidth(), NULL);
		g_duiProp.AddAttribute(node, _T("height"), rcDoc.GetHeight(), NULL);
	}
	return TRUE;
}


BOOL CUIManager::UpdateControlWidth(xml_node node, int width)
{
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl)	return FALSE;

	//改变主窗口大小
	if(pControl == GetUiFrom()) 
	{
		SIZE szForm = GetUiFrom()->GetInitSize();
		szForm.cx = width;
		GetUiFrom()->SetInitSize(szForm.cx, szForm.cy);
		SetScrollSize();

		CString temp;
		temp.Format(_T("%d,%d"), szForm.cx, szForm.cy);
		g_duiProp.AddAttribute(node, _T("size"), temp, m_pDesignView);
		return TRUE;
	}

	if(pControl->IsFloat())
	{
		CRect rc = pControl->GetPos();
		rc.right = rc.left + width;
		return UpdateControlPos(node, rc);
	}

	//非float控件比较特殊, UI库自动布局.
	CDuiRect rcDoc(node.attribute(_T("pos")).as_string(_T("0,0,0,0")));
	rcDoc.right = width;

	pControl->SetAttribute(_T("pos"), rcDoc.ToString());	//不能调用pControl->SetPos();
	UpdateControlUI(pControl);

	g_duiProp.AddAttribute(node, _T("pos"), rcDoc, m_pDesignView);
	g_duiProp.AddAttribute(node, _T("width"), rcDoc.GetWidth(), NULL);
	g_duiProp.AddAttribute(node, _T("height"), rcDoc.GetHeight(), NULL);
	return TRUE;
}

BOOL CUIManager::UpdateControlHeight(xml_node node, int height)
{
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(!pControl)	return FALSE;

	//改变主窗口大小
	if(pControl == GetUiFrom()) 
	{
		SIZE szForm = GetUiFrom()->GetInitSize();
		szForm.cy = height;
		GetUiFrom()->SetInitSize(szForm.cx, szForm.cy);
		SetScrollSize();

		CString temp;
		temp.Format(_T("%d,%d"), szForm.cx, szForm.cy);
		g_duiProp.AddAttribute(node, _T("size"), temp, m_pDesignView);
		return TRUE;
	}

	if(pControl->IsFloat())
	{
		CRect rc = pControl->GetPos();
		rc.bottom = rc.top + height;
		return UpdateControlPos(node, rc);
	}

	//非float控件比较特殊, UI库自动布局.
	CDuiRect rcDoc(node.attribute(_T("pos")).as_string(_T("0,0,0,0")));
	rcDoc.bottom = height;

	pControl->SetAttribute(_T("pos"), rcDoc.ToString());	//不能调用pControl->SetPos();
	UpdateControlUI(pControl);

	g_duiProp.AddAttribute(node, _T("pos"), rcDoc, m_pDesignView);
	g_duiProp.AddAttribute(node, _T("width"), rcDoc.GetWidth(),	NULL);
	g_duiProp.AddAttribute(node, _T("height"), rcDoc.GetHeight(),	NULL);
	return TRUE;
}

BOOL CUIManager::UpdateControlUI(xml_node node)
{
	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(pControl)
	{
		for (xml_attribute attr=node.first_attribute(); attr; attr=attr.next_attribute())
		{
			pControl->SetAttribute(attr.name(), attr.value());
		}

		return UpdateControlUI(pControl);
	}
	return FALSE;
}

BOOL CUIManager::UpdateControlUI(xml_node node, xml_attribute attr)
{
#ifndef DUILIB_VERSION_ORIGINAL
	//更新Style属性
	if(CompareString(node.name(), _T("Style")))
	{
		CString strStyleName = node.attribute(_T("name")).as_string();
		CString strStyleValue = node.attribute(_T("value")).as_string();
		bool bShared = node.attribute(_T("shared")).as_bool();
		GetManager()->AddStyle(strStyleName, strStyleValue, bShared);
		return TRUE;
	}
#endif

	//更新Font属性
	if( CompareString(node.name(), _T("Font")) )
	{
		int id = -1;
		LPCTSTR pFontName = NULL;
		int size = 12;
		bool bold = false;
		bool underline = false;
		bool italic = false;
		bool defaultfont = false;
		bool shared = false;

		LPCTSTR pstrName = NULL;
		LPCTSTR pstrValue = NULL;
		LPTSTR pstr = NULL;

		for( xml_attribute attr = node.first_attribute(); attr; attr=attr.next_attribute() ) {
			pstrName = attr.name();
			pstrValue = attr.value();
			if( _tcsicmp(pstrName, _T("id")) == 0 ) {
				id = _tcstol(pstrValue, &pstr, 10);
			}
			else if( _tcsicmp(pstrName, _T("name")) == 0 ) {
				pFontName = pstrValue;
			}
			else if( _tcsicmp(pstrName, _T("size")) == 0 ) {
				size = _tcstol(pstrValue, &pstr, 10);
			}
			else if( _tcsicmp(pstrName, _T("bold")) == 0 ) {
				bold = (_tcsicmp(pstrValue, _T("true")) == 0);
			}
			else if( _tcsicmp(pstrName, _T("underline")) == 0 ) {
				underline = (_tcsicmp(pstrValue, _T("true")) == 0);
			}
			else if( _tcsicmp(pstrName, _T("italic")) == 0 ) {
				italic = (_tcsicmp(pstrValue, _T("true")) == 0);
			}
			else if( _tcsicmp(pstrName, _T("default")) == 0 ) {
				defaultfont = (_tcsicmp(pstrValue, _T("true")) == 0);
			}
			else if( _tcsicmp(pstrName, _T("shared")) == 0 ) {
				shared = (_tcsicmp(pstrValue, _T("true")) == 0);
			}
		}
		if( id >= 0 ) {
			GetManager()->AddFont(id, pFontName, size, bold, underline, italic, shared);
#ifdef DUILIB_VERSION_ORIGINAL
			if( defaultfont ) GetManager()->SetDefaultFont(pFontName, size, bold, underline, italic, shared);
#else
			if( defaultfont ) GetManager()->SetDefaultFont(pFontName, GetManager()->GetDPIObj()->Scale(size), bold, underline, italic, shared);
#endif
		}
	}


	CControlUI *pControl = (CControlUI *)node.get_tag();
	if(pControl)
	{
		if(pControl == GetUiFrom()) //改变窗口大小
		{
			if(_tcscmp(attr.name(), _T("size")) == 0)
			{
				CDuiSize sz(attr.value());
				GetUiFrom()->SetInitSize(sz.cx, sz.cy);
				SetScrollSize();
			}
		}
		else if(_tcscmp(attr.name(), _T("pos")) == 0)
		{
			CDuiRect rc(attr.value());
			return UpdateControlPos(node, rc, TRUE);
		}
		else if(_tcscmp(attr.name(), _T("width")) == 0)
		{
			return UpdateControlWidth(node, attr.as_int());
		}
		else if(_tcscmp(attr.name(), _T("height")) == 0)
		{
			return UpdateControlHeight(node, attr.as_int());
		}
		else
			pControl->SetAttribute(attr.name(), attr.value());

		return UpdateControlUI(pControl);
	}

	return FALSE;
}

BOOL CUIManager::DeleteControl(xml_node node)
{
	if(g_duiProp.IsWindowForm(node.name()))
		return FALSE;

	CControlUI *pControl = (CControlUI *)node.get_tag();

	xml_node nodeContainer = node.parent();
	if(g_duiProp.IsBaseFromContainer(nodeContainer.name()))
	{
		CContainerUI *pContainer = (CContainerUI *)nodeContainer.get_tag();
		pContainer->Remove(pControl);
		pContainer->NeedUpdate();
	}

	GetTreeView()->DeleteXmlNode(node);

	GetCmdHistory()->AddDeleteControl(node);

	if(nodeContainer)
	{
		nodeContainer.remove_child(node);
	}

	
	return TRUE;
}



void CUIManager::SetScrollSize()
{
	CSize szForm = GetManager()->GetInitSize();

	if(GetDesignView()->m_bViewRuleBar)
	{
		szForm.cx += RULEBAR_SIZE_X;
		szForm.cy += RULEBAR_SIZE_Y;
	}
	m_pDesignView->SetScrollSizes(MM_TEXT, szForm);
	m_pDesignView->GetParent()->SendMessage(WM_SIZE);
}

void CUIManager::SetZoom(int zoom)
{
#ifndef DUILIB_VERSION_ORIGINAL
	GetManager()->SetDPI(zoom);
	GetManager()->ResetDPIAssets();

	CRect rc;
	GetUiWindow()->GetWindowRect(rc);

	if(GetDesignView()->m_bViewRuleBar)
		GetUiWindow()->MoveWindow(RULEBAR_SIZE_X, RULEBAR_SIZE_Y, rc.Width(), rc.Height(), TRUE);
	else
		GetUiWindow()->MoveWindow(0, 0, rc.Width(), rc.Height(), TRUE);

	GetUiFrom()->SetInitSize(rc.Width(), rc.Height());
	SetScrollSize();

	GetDesignView()->SendMessage(WM_SIZE);
#endif
}

void CUIManager::SetSplitterMode(int nMode)
{
	m_nSplitterMode = nMode;
}

int  CUIManager::GetSplitterMode() const
{
	return m_nSplitterMode;
}