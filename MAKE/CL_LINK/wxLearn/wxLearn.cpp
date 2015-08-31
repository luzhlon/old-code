#include <wx/wxprec.h>
#include <wx/wx.h>

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title);

private:
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
wxEND_EVENT_TABLE()

wxDECLARE_APP(MyApp);

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame(wxT("wxWidgets Learn."));
    frame->Show(true);
    return true;
}


void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg;
    msg.Printf(wxT("Hello and welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, wxT("About wxLearn"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

MyFrame::MyFrame(const wxString& title):wxFrame(NULL, wxID_ANY, title)
{
    //SetIcon(wxIcon(mondrian_xpm));

    wxMenu *fileMenu = new wxMenu;
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"), wxT("Show about dialog"));
    fileMenu->Append(wxID_EXIT, wxT("E&xit...\tAlt-X"), wxT("Quit this program"));

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Help");
    SetMenuBar(menuBar);

    CreateStatusBar(2);
    SetStatusText(wxT("Welcome to wxWidgets!"));
}


