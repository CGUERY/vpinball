#include "stdafx.h"
#include "Properties/LightseqStatesProperty.h"
#include <WindowsX.h>

LightseqStatesProperty::LightseqStatesProperty(VectorProtected<ISelect> *pvsel) : BasePropertyDialog(IDD_PROPLIGHTSEQ_STATE, pvsel)
{
    m_posXEdit.SetDialog(this);
    m_posYEdit.SetDialog(this);
    m_updateIntervalEdit.SetDialog(this);
}

void LightseqStatesProperty::UpdateVisuals(const int dispid/*=-1*/)
{
    for (int i = 0; i < m_pvsel->Size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == NULL) || (m_pvsel->ElementAt(i)->GetItemType() != eItemLightSeq))
            continue;
        LightSeq *const lightseq = (LightSeq *)m_pvsel->ElementAt(i);

        if (dispid == 9 || dispid == -1)
            PropertyDialog::SetFloatTextbox(m_posXEdit, lightseq->GetX());
        if (dispid == 10 || dispid == -1)
            PropertyDialog::SetFloatTextbox(m_posYEdit, lightseq->GetY());
        if (dispid == IDC_LIGHTSEQ_UPDATE_INTERVAL_EDIT || dispid == -1)
            PropertyDialog::SetIntTextbox(m_updateIntervalEdit, lightseq->GetUpdateInterval());

        if (dispid == DISPID_Collection || dispid == -1)
        {
            char szT[MAX_PATH];
            WideCharToMultiByte(CP_ACP, 0, lightseq->m_d.m_wzCollection, -1, szT, MAX_PATH, NULL, NULL);

            PropertyDialog::UpdateCollectionComboBox(lightseq->GetPTable(), m_collectionCombo, szT);
        }

        //only show the first element on multi-select
        break;

    }
}

void LightseqStatesProperty::UpdateProperties(const int dispid)
{
    for (int i = 0; i < m_pvsel->Size(); i++)
    {
        if ((m_pvsel->ElementAt(i) == NULL) || (m_pvsel->ElementAt(i)->GetItemType() != eItemLightSeq))
            continue;
        LightSeq *const lightseq = (LightSeq *)m_pvsel->ElementAt(i);
        switch (dispid)
        {
            case 9:
                CHECK_UPDATE_VALUE_SETTER(lightseq->SetX, lightseq->GetX, PropertyDialog::GetFloatTextbox, m_posXEdit, lightseq);
                break;
            case 10:
                CHECK_UPDATE_VALUE_SETTER(lightseq->SetY, lightseq->GetY, PropertyDialog::GetFloatTextbox, m_posYEdit, lightseq);
                break;
            case IDC_LIGHTSEQ_UPDATE_INTERVAL_EDIT:
                CHECK_UPDATE_VALUE_SETTER(lightseq->SetUpdateInterval, lightseq->GetUpdateInterval, PropertyDialog::GetIntTextbox, m_updateIntervalEdit, lightseq);
                break;
            case DISPID_Collection:
            {
                PropertyDialog::StartUndo(lightseq);
                char szText[MAX_PATH];
                PropertyDialog::GetComboBoxText(m_collectionCombo, szText);
                UNICODE_FROM_ANSI(lightseq->m_d.m_wzCollection, szText, lstrlen(szText));
                lightseq->m_d.m_wzCollection[lstrlen(szText)] = 0;
                PropertyDialog::EndUndo(lightseq);
                break;
            }
            default:
                break;
        }
    }
    UpdateVisuals(dispid);
}

BOOL LightseqStatesProperty::OnInitDialog()
{
    m_posXEdit.AttachItem(9);
    m_posYEdit.AttachItem(10);
    m_updateIntervalEdit.AttachItem(IDC_LIGHTSEQ_UPDATE_INTERVAL_EDIT);
    AttachItem(DISPID_Collection, m_collectionCombo);
    UpdateVisuals();
    return TRUE;
}
