#pragma once
#include "contracts.h"
#include <windows.h>
#include <winhttp.h>
#include <string>
#include <vector>
#include <iostream>

namespace ai {

// ============================================================================
// 1. AI Service Interface
// ============================================================================

class IAIService {
public:
    virtual ~IAIService() = default;
    virtual AIResponse Chat(const AIRequest& request) = 0;
};

// ============================================================================
// 2. OpenRouter AI Service Implementation (WinHTTP)
// ============================================================================

class OpenRouterAIService : public IAIService {
private:
    std::wstring apiKey_;
    std::wstring modelName_;

    std::wstring GetIniPath() {
        wchar_t path[MAX_PATH] = {};
        GetModuleFileNameW(nullptr, path, MAX_PATH);
        std::wstring ws(path);
        size_t pos = ws.find_last_of(L"\\/");
        if (pos != std::wstring::npos) {
            return ws.substr(0, pos) + L"\\settings.ini";
        }
        return L"settings.ini";
    }

    std::wstring ReadIniString(const wchar_t* key, const wchar_t* fallback) {
        std::wstring iniPath = GetIniPath();
        wchar_t buf[512] = {};
        GetPrivateProfileStringW(L"Settings", key, fallback, buf, 512, iniPath.c_str());
        return buf;
    }

    std::wstring UnescapeJSONString(const std::wstring& str) {
        std::wstring res;
        res.reserve(str.size());
        for (size_t i = 0; i < str.size(); ++i) {
            if (str[i] == L'\\' && i + 1 < str.size()) {
                wchar_t next = str[i + 1];
                if (next == L'n') { res += L'\n'; i++; }
                else if (next == L't') { res += L'\t'; i++; }
                else if (next == L'"') { res += L'"'; i++; }
                else if (next == L'\\') { res += L'\\'; i++; }
                else { res += str[i]; }
            } else {
                res += str[i];
            }
        }
        return res;
    }

    std::wstring ExtractJSONValue(const std::wstring& json, const std::wstring& key) {
        size_t pos = json.find(L"\"" + key + L"\"");
        if (pos == std::wstring::npos) return L"";
        pos = json.find(L":", pos);
        if (pos == std::wstring::npos) return L"";
        pos = json.find(L"\"", pos);
        if (pos == std::wstring::npos) return L"";
        size_t endPos = json.find(L"\"", pos + 1);
        while (endPos != std::wstring::npos && json[endPos - 1] == L'\\') {
            endPos = json.find(L"\"", endPos + 1);
        }
        if (endPos == std::wstring::npos) return L"";
        return json.substr(pos + 1, endPos - pos - 1);
    }

    std::string Utf16ToUtf8(const std::wstring& wstr) {
        if (wstr.empty()) return "";
        int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> buf(len);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buf.data(), len, nullptr, nullptr);
        return buf.data();
    }

public:
    OpenRouterAIService() {
        apiKey_ = ReadIniString(L"OpenRouter.ApiKey", L"");
        modelName_ = ReadIniString(L"OpenRouter.Model", L"google/gemini-flash-1.5-free");
    }

    AIResponse Chat(const AIRequest& request) override {
        AIResponse response;

        // If no API key is set, run in Mock Mode for development/testing
        if (apiKey_.empty()) {
            response.text = L"OpenRouter API Key is missing in settings.ini. (Running in offline developer mock mode). Response to: " + request.prompt;
            return response;
        }

        // Construct simple chat JSON payload
        std::string payload = "{\"model\":\"" + Utf16ToUtf8(modelName_) + "\",";
        payload += "\"messages\":[{\"role\":\"user\",\"content\":\"" + Utf16ToUtf8(request.prompt) + "\"}]}";

        // WinHTTP Setup
        HINTERNET hSession = WinHttpOpen(L"DynamicIslandVoiceAssistant/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
        if (!hSession) {
            response.text = L"Error: Failed to open WinHTTP session.";
            return response;
        }

        HINTERNET hConnect = WinHttpConnect(hSession, L"openrouter.ai", INTERNET_DEFAULT_HTTPS_PORT, 0);
        if (!hConnect) {
            response.text = L"Error: Failed to connect to OpenRouter server.";
            WinHttpCloseHandle(hSession);
            return response;
        }

        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/api/v1/chat/completions", nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
        if (!hRequest) {
            response.text = L"Error: Failed to open HTTPS request.";
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return response;
        }

        // Headers
        std::wstring headers = L"Content-Type: application/json\r\n";
        headers += L"Authorization: Bearer " + apiKey_ + L"\r\n";
        headers += L"HTTP-Referer: https://github.com/Aonikyadav/window-dynamic-island\r\n";
        headers += L"X-Title: Dynamic Island for Windows\r\n";

        BOOL bResults = WinHttpSendRequest(hRequest, headers.c_str(), (DWORD)headers.length(), (LPVOID)payload.c_str(), (DWORD)payload.length(), (DWORD)payload.length(), 0);
        if (bResults) {
            bResults = WinHttpReceiveResponse(hRequest, nullptr);
        }

        std::wstring responseJson;
        if (bResults) {
            DWORD dwSize = 0;
            do {
                DWORD dwDownloaded = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;
                if (dwSize == 0) break;

                std::vector<char> buffer(dwSize + 1);
                if (WinHttpReadData(hRequest, (LPVOID)buffer.data(), dwSize, &dwDownloaded)) {
                    buffer[dwDownloaded] = '\0';
                    int wlen = MultiByteToWideChar(CP_UTF8, 0, buffer.data(), -1, nullptr, 0);
                    if (wlen > 0) {
                        std::vector<wchar_t> wbuf(wlen);
                        MultiByteToWideChar(CP_UTF8, 0, buffer.data(), -1, wbuf.data(), wlen);
                        responseJson += wbuf.data();
                    }
                }
            } while (dwSize > 0);
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);

        if (responseJson.empty()) {
            response.text = L"Error: Empty response from OpenRouter.";
        } else {
            // Extract choice content string
            std::wstring content = ExtractJSONValue(responseJson, L"content");
            if (content.empty()) {
                // If content is empty, check for error key
                std::wstring errMsg = ExtractJSONValue(responseJson, L"message");
                if (!errMsg.empty()) {
                    response.text = L"API Error: " + errMsg;
                } else {
                    response.text = L"Error: Failed to parse OpenRouter response JSON: " + responseJson;
                }
            } else {
                response.text = UnescapeJSONString(content);
            }
        }

        return response;
    }
};

} // namespace ai
