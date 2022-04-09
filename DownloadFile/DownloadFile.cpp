#include <iostream>
#include <windows.h>

#define CURL_STATICLIB
#include <curl.h>
#pragma comment(lib, "libcurl_a.lib")
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Crypt32.Lib")
#pragma comment(lib, "Wldap32.Lib")

double ProcessBar = 0;

int DrawProcessBar(int BarSize) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    static double LastProgress = 0, ProgresBarAdv = 0;
    if (LastProgress != ProcessBar) {
        ProgresBarAdv = round((ProcessBar/100) * BarSize);

        std::cout << "\r ";
        SetConsoleTextAttribute(hConsole, 160);
        std::cout << "Progress : [ ";

        if (ProcessBar < 10) {
            std::cout << "0" << ProcessBar << " %]";
        }
        else {
            std::cout << ProcessBar << " %] ";
        }

        SetConsoleTextAttribute(hConsole, 15);
        std::cout << " [";
        SetConsoleTextAttribute(hConsole, 10);

        for (int i = 0; i <= ProgresBarAdv; i++) {
            std::cout << "#";
        }

        SetConsoleTextAttribute(hConsole, 15);
        for (int i = 0; i < BarSize - ProgresBarAdv; i++) {
            std::cout << ".";
        }

        std::cout << "]";
        LastProgress = ProcessBar;
    }
    return 0;
}

int CalculateBar(void* bar, double t, double d) {
    ProcessBar = round(d / t * 100);
    DrawProcessBar(40);
    return 0;
}

bool DownloadFile(std::string Url, std::string Path) {
    bool TempReturn = false;
    CURL* Curl = curl_easy_init();
    CURLcode ExitCode;
    FILE* File;
    if (Curl) {
        File = fopen(Path.c_str(), "wb");
        curl_easy_setopt(Curl, CURLOPT_URL, Url.c_str());
        curl_easy_setopt(Curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(Curl, CURLOPT_WRITEDATA, File);
        curl_easy_setopt(Curl, CURLOPT_NOPROGRESS, FALSE);
        curl_easy_setopt(Curl, CURLOPT_PROGRESSFUNCTION, CalculateBar);
        ExitCode = curl_easy_perform(Curl);
        fclose(File);
        if (ExitCode == CURLE_OK)
        {
            TempReturn = true;
        }
        curl_easy_cleanup(Curl);
    }
    return TempReturn;
}

int main()
{
    DownloadFile("https://lg.comnet.com.tr/500MB.test", "500MB.test");
    return 0;
}