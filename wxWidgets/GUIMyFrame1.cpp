#include "GUIMyFrame1.h"
#include <vector>
#include <fstream>
#include "vecmat.h"

struct Point {
    float x, y, z;
    Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
    int R, G, B;
    Color(int _R, int _G, int _B) : R(_R), G(_G), B(_B) {}
};

struct Segment {
    Point begin, end;
    Color color;
    Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
    :
    MyFrame1(parent)
{
    m_button_load_geometry->SetLabel(_("Wczytaj Geometri\u0119"));
    m_staticText25->SetLabel(_("Obr\u00F3t X:"));
    m_staticText27->SetLabel(_("Obr\u00F3t Y:"));
    m_staticText29->SetLabel(_("Obr\u00F3t Z:"));

    WxSB_TranslationX->SetRange(0, 200); WxSB_TranslationX->SetValue(100);
    WxSB_TranslationY->SetRange(0, 200); WxSB_TranslationY->SetValue(100);
    WxSB_TranslationZ->SetRange(0, 200); WxSB_TranslationZ->SetValue(100);

    WxSB_RotateX->SetRange(0, 360); WxSB_RotateX->SetValue(0);
    WxSB_RotateY->SetRange(0, 360); WxSB_RotateY->SetValue(0);
    WxSB_RotateZ->SetRange(0, 360); WxSB_RotateZ->SetValue(0);

    WxSB_ScaleX->SetRange(1, 200); WxSB_ScaleX->SetValue(100);
    WxSB_ScaleY->SetRange(1, 200); WxSB_ScaleY->SetValue(100);
    WxSB_ScaleZ->SetRange(1, 200); WxSB_ScaleZ->SetValue(100);

    WxPanel->Bind(wxEVT_PAINT, &GUIMyFrame1::OnPanelPaint, this);
}

void GUIMyFrame1::WxPanel_Repaint(wxUpdateUIEvent& event)
{
    Repaint();
}

void GUIMyFrame1::m_button_load_geometry_click(wxCommandEvent& event)
{
    wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (WxOpenFileDialog.ShowModal() == wxID_OK)
    {
        double x1, y1, z1, x2, y2, z2;
        int r, g, b;

        std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
        if (in.is_open())
        {
            data.clear();
            while (!in.eof())
            {
                in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
                data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
            }
            in.close();
        }
    }
}

void GUIMyFrame1::Scrolls_Updated(wxScrollEvent& event)
{
    WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
    WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
    WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

    WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
    WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
    WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

    WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
    WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
    WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));


    Repaint();
}


void GUIMyFrame1::Repaint()
{
    WxPanel->Refresh();
}

void GUIMyFrame1::OnPanelPaint(wxPaintEvent& event)
{
    wxPaintDC dc(WxPanel);

    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();

    dc.SetPen(*wxBLACK_PEN);

    for (Segment s : data)
    {
		Vector4 p1, p2;
		p1.Set(s.begin.x, s.begin.y, s.begin.z);
		p2.Set(s.end.x, s.end.y, s.end.z);

        ////////
        Matrix4 mTranslate = Matrix4::Identity();
        Matrix4 mRotateX = Matrix4::Identity();
        Matrix4 mRotateY = Matrix4::Identity();
        Matrix4 mRotateZ = Matrix4::Identity();
        Matrix4 mScale = Matrix4::Identity();
        Matrix4 mTransform = Matrix4::Identity();

		mTranslate.data[0][3] = (WxSB_TranslationX->GetValue() - 100) / 50.0;
		mTranslate.data[1][3] = (WxSB_TranslationY->GetValue() - 100) / 50.0;
		mTranslate.data[2][3] = (WxSB_TranslationZ->GetValue() - 100) / 50.0;
		double angleX = WxSB_RotateX->GetValue() * 3.14159265 / 180.0;
		double angleY = WxSB_RotateY->GetValue() * 3.14159265 / 180.0;
		double angleZ = WxSB_RotateZ->GetValue() * 3.14159265 / 180.0;
		mRotateX.data[1][1] = cos(angleX); mRotateX.data[1][2] = -sin(angleX);
		mRotateX.data[2][1] = sin(angleX); mRotateX.data[2][2] = cos(angleX);
		mRotateY.data[0][0] = cos(angleY); mRotateY.data[0][2] = sin(angleY);
		mRotateY.data[2][0] = -sin(angleY); mRotateY.data[2][2] = cos(angleY);
		mRotateZ.data[0][0] = cos(angleZ); mRotateZ.data[0][1] = -sin(angleZ);
		mRotateZ.data[1][0] = sin(angleZ); mRotateZ.data[1][1] = cos(angleZ);
		mScale.data[0][0] = WxSB_ScaleX->GetValue() / 100.0;
		mScale.data[1][1] = WxSB_ScaleY->GetValue() / 100.0;
		mScale.data[2][2] = WxSB_ScaleZ->GetValue() / 100.0;
		mTransform = mTranslate * mRotateZ * mRotateY * mRotateX * mScale;
		p1 = mTransform * p1;
		p2 = mTransform * p2;
		///////
		int x1_2D = static_cast<int>((p1.GetX() / (p1.GetZ() + 5.0)) * 200.0 + WxPanel->GetSize().GetWidth() / 2);
		int y1_2D = static_cast<int>((p1.GetY() / (p1.GetZ() + 5.0)) * 200.0 + WxPanel->GetSize().GetHeight() / 2);
		int x2_2D = static_cast<int>((p2.GetX() / (p2.GetZ() + 5.0)) * 200.0 + WxPanel->GetSize().GetWidth() / 2);
		int y2_2D = static_cast<int>((p2.GetY() / (p2.GetZ() + 5.0)) * 200.0 + WxPanel->GetSize().GetHeight() / 2);
		dc.SetPen(wxPen(wxColour(s.color.R, s.color.G, s.color.B)));
        dc.DrawLine(x1_2D, y1_2D, x2_2D, y2_2D);
    }
}